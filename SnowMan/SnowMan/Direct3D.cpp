#include "Direct3D.h"
LPD3DXEFFECT pEffect;

LPD3DXMESH texMesh;
LPD3DXMESH mesh;
int DirectX::n = 1;
D3DXVECTOR4 DirectX::LightAmbient = D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f);
D3DXVECTOR4 DirectX::LightDiffuse = D3DXVECTOR4(1.1f, 1.1f, 1.1f, 1.0f);
D3DXVECTOR4 DirectX::LightDirection = D3DXVECTOR4(1.0f, -1.0f, 0.0f, 0.0f);
D3DMATERIAL9 snowMtrl;
bool DirectX::initialDirectX(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	//1.�����ӿ�
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//2.��ȡӲ����Ϣ��ȷ�����㴦��ʽ
	D3DCAPS9 caps;
	int vertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);    //(��ǰ�Կ���Ӳ���豸)
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.��дD3D����
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4.�����豸
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vertexProcessing, &d3dpp, &pD3DXDevice);
	if (pD3DXDevice == nullptr)
		cout << "ERROR" << endl;
	d3d9->Release();

	//������Ļ���ߡ�Z��������С
	width_ = width;
	height_ = height;

	//Camera
	pCamera = new Camera(pD3DXDevice,width,height);

	pDirectInput = new DirectInput();
	pDirectInput->init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//Font setting
	font = nullptr;
	D3DXCreateFont(pD3DXDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "΢���ź�", &font);
	GetClientRect(hwnd, &formatRect);

	snowMan1 = new SnowMan(pD3DXDevice);
	snowMan2 = new SnowMan(pD3DXDevice,1.5);
	cube = new Cube(pD3DXDevice);
	terrian = new Terrian(pD3DXDevice);
	terrian->load_terrain_from_file("Textures\\terrian.raw", "Textures\\land.jpg");
	terrian->init_terrain(200, 200, 300.0f, 18.0f);

	skyBox = new SkyBox(pD3DXDevice,600);
	string picture = "snow.jpg";
	skyBox->loadSkyTextureFromFile("Textures\\front"+picture,"Textures\\back"+picture,"Textures\\left" + picture,"Textures\\right" + picture, "Textures\\top" + picture);

	pSnowParticle = new SnowParticle(pD3DXDevice);

	pTree = new Model(pD3DXDevice,"Textures\\Tree.X");
	pTree1 = new Model(pD3DXDevice, "Textures\\Tree1.X");
	pHouse = new Model(pD3DXDevice,"Textures\\House.X");

	caculate_light_matrix();
	::ZeroMemory(&snowMtrl, sizeof(snowMtrl));
	snowMtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	snowMtrl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	snowMtrl.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pD3DXDevice->SetMaterial(&snowMtrl);

	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	pD3DXDevice->SetLight(0, &light);
	pD3DXDevice->LightEnable(0, true);

	pD3DXDevice->SetRenderState(D3DRS_LIGHTING, true);
	pD3DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pD3DXDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	pD3DXDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));   //����һ�»�����
	pD3DXDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);

	pD3DXDevice->CreateTexture(ShadowMap_SIZE, ShadowMap_SIZE,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&ShadowMap,
		NULL);
	//D3DMULTISAMPLE_TYPE
	pD3DXDevice->CreateDepthStencilSurface(ShadowMap_SIZE,
		ShadowMap_SIZE,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&DSShadow,
		NULL);
	LPD3DXBUFFER pCode = nullptr;
	D3DXCreateEffectFromFile(pD3DXDevice, "VertexShader.fx",nullptr, nullptr, 0, nullptr, &pEffect, &pCode);
	pEffect->SetVector(LIGHTDIFFUSE, &LightDiffuse);
	pEffect->SetVector(LIGHTAMBIENT, &LightAmbient);

	return true;
}

void DirectX::update(float time)
{
	pDirectInput->get_input();
	pCamera->update(pDirectInput);
	pSnowParticle->updateSnowParticle(time);
	// �۲����
	D3DXMATRIXA16 matView;
	pD3DXDevice->GetTransform(D3DTS_VIEW, &matView);
	// Light Direction 
	D3DXVECTOR4 Dir;
	D3DXVec4Transform(&Dir, &LightDirection, &matView);
	D3DXVec4Normalize(&Dir, &Dir);
	pEffect->SetVector(LIGHTDIR,&Dir);
}

void DirectX::caculate_light_matrix()
{
	D3DXVECTOR3 rightVector = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 upVector = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	D3DXVECTOR3 lookVector = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	D3DXVECTOR3 position = D3DXVECTOR3(-250.0f, 250.0f, 0.0f);
	Camera light_camera(pD3DXDevice, ShadowMap_SIZE, ShadowMap_SIZE, rightVector, upVector, lookVector,position);
	light_camera.calculate_viewMatrix(&light_view_matrix);
	//D3DXMatrixOrthoOffCenterLH(&light_proj_matrix, -1000,1000,-1000,1000, 1.0f, 10000.0f);
	D3DXMatrixPerspectiveFovLH(&light_proj_matrix, D3DX_PI*0.5f, (float)ShadowMap_SIZE / (float)ShadowMap_SIZE, 1.0f, 10000.0f);
	//D3DXMatrixOrthoLH(&light_proj_matrix,D3DX_PI*0.5f,(float)ShadowMap_SIZE/(float)ShadowMap_SIZE,1.0f,10000.0f);
}
void DirectX::render()
{
	//Save old RenderTarget and replace with the shadowMap surface
	LPDIRECT3DSURFACE9 pOldRT = nullptr;
	pD3DXDevice->GetRenderTarget(0, &pOldRT); 
	LPDIRECT3DSURFACE9 pShadowSurface;
	ShadowMap->GetSurfaceLevel(0, &pShadowSurface);
	pD3DXDevice->SetRenderTarget(0, pShadowSurface);
	SAFE_RELEASE(pShadowSurface);
	//Save old DepthStentilSuface and replace with the shadowSurface
	LPDIRECT3DSURFACE9 pOldDSS = nullptr;
	pD3DXDevice->GetDepthStencilSurface(&pOldDSS);
	pD3DXDevice->SetDepthStencilSurface(DSShadow);

	//1.Render shadowMap
	render_scene(true);

	//recover
	pD3DXDevice->SetDepthStencilSurface(pOldDSS);
	pD3DXDevice->SetRenderTarget(0,pOldRT);
	SAFE_RELEASE(pOldRT);
	SAFE_RELEASE(pOldDSS);

	//
	pEffect->SetTexture(SHADOW, ShadowMap);
	D3DXMATRIX viewToLightProjMatrix;
	pCamera->calculate_viewMatrix(&viewToLightProjMatrix);
	D3DXMatrixInverse(&viewToLightProjMatrix, nullptr, &viewToLightProjMatrix);
	viewToLightProjMatrix = viewToLightProjMatrix*light_view_matrix*light_proj_matrix;
	pEffect->SetMatrix(VIEW_LIGHT_PROJ_MATRIX, &viewToLightProjMatrix);
	render_scene(false);
	pEffect->SetTexture(SHADOW, nullptr);
	
}

void DirectX::render_scene(bool isRenderShadow)
{
	pD3DXDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (isRenderShadow)
	{
		pEffect->SetTechnique("RenderShadow");
		pEffect->SetMatrix(VIEW_MATRIX, &light_view_matrix);
		pEffect->SetMatrix(PROJ_MATRIX, &light_proj_matrix);
		float angle = (n++)*0.03f;
		if (angle >= 360) n = 0;
	}
	else
	{
		pEffect->SetTechnique("RenderScene");
		D3DXMATRIXA16 matView;
		pD3DXDevice->GetTransform(D3DTS_VIEW, &matView);
		pEffect->SetMatrix(VIEW_MATRIX, &matView);
		D3DXMATRIXA16 matProj;
		pD3DXDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		pEffect->SetMatrix(PROJ_MATRIX, &matProj);
		formatRect.top = 100;
		font->DrawText(0, "������������Ϩ�����Ϸ�������롿", -1, &formatRect, DT_CENTER, D3DCOLOR_XRGB(68, 139, 256));
		
	}
	pD3DXDevice->BeginScene();

	

	D3DXVECTOR3 pos(-8.0f, -8.0f, 0.0f);
	D3DXMATRIX rotate;
	D3DXMatrixRotationY(&rotate, n*0.03f);
	D3DXMATRIX moveMatrix;
	D3DXMatrixTranslation(&moveMatrix, pos.x, pos.y - 0.5, pos.z);
	moveMatrix *= rotate;//move then rotate
	pD3DXDevice->SetTransform(D3DTS_WORLD, &moveMatrix);
	snowMan1->draw_snowMan(pEffect);

	D3DXMATRIX cubeMatrix;
	D3DXMatrixTranslation(&cubeMatrix, pos.x, pos.y - 4.5, pos.z);
	cubeMatrix *= rotate;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &cubeMatrix);
	cube->draw_cube(pEffect);

	D3DXMATRIX d;
	D3DXMatrixTranslation(&d, pos.x + 4, pos.y, pos.z + 10);
	pD3DXDevice->SetTransform(D3DTS_WORLD, &d);
	snowMan2->draw_snowMan(pEffect);

	D3DXMATRIX terrianMatrix;
	D3DXMatrixTranslation(&terrianMatrix, 0.0f, 0.0f, 0.0f);
	pD3DXDevice->SetTransform(D3DTS_WORLD, &terrianMatrix);
	terrian->render_terrain(pEffect);

	if (!isRenderShadow)
	{
		light_off();
		D3DXMATRIX skyBoxMatrix;
		D3DXMatrixTranslation(&skyBoxMatrix, 20.0f, -130.0f, -20.0f);
		pD3DXDevice->SetTransform(D3DTS_WORLD, &skyBoxMatrix);
		skyBox->renderSkyBox(nullptr);
		pSnowParticle->renderSnowParticle();
		light_on();
	}

	treeRender(pos);

	D3DXMATRIX matrix;
	float scale = 0.008f;
	D3DXMatrixScaling(&matrix, scale, scale, scale);
	D3DXMATRIX houseMatrix;
	D3DXMatrixTranslation(&houseMatrix, pos.x, pos.y - 5, pos.z + 80);
	houseMatrix = matrix*houseMatrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &houseMatrix);
	pHouse->renderModel(pEffect);

	
	pD3DXDevice->EndScene();
	pD3DXDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
void DirectX::treeRender(D3DXVECTOR3 pos)
{
	D3DXMATRIX treeMatrix;
	float scale = 0.08f;
	D3DXMatrixScaling(&treeMatrix, scale, scale, scale);

	D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix, pos.x - 6, pos.y - 7, pos.z);
	matrix = treeMatrix*matrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &matrix);
	pTree->renderModel(pEffect);

	D3DXMatrixTranslation(&matrix, pos.x - 60, pos.y + 7, pos.z);
	matrix = treeMatrix*matrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &matrix);
	pTree->renderModel(pEffect);

	D3DXMatrixTranslation(&matrix, pos.x - 60, pos.y-3 , pos.z+60);
	matrix = treeMatrix*matrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &matrix);
	pTree->renderModel(pEffect);


	D3DXMatrixTranslation(&matrix, pos.x + 6, pos.y - 7, pos.z);
	matrix = treeMatrix*matrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &matrix);
	pTree1->renderModel(pEffect);

	D3DXMatrixTranslation(&matrix, pos.x + 60, pos.y+15 , pos.z);
	matrix = treeMatrix*matrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &matrix);
	pTree1->renderModel(pEffect);

	D3DXMatrixTranslation(&matrix, pos.x + 60, pos.y +3, pos.z + 60);
	matrix = treeMatrix*matrix;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &matrix);
	pTree1->renderModel(pEffect);
}

void DirectX::light_on()
{
	pD3DXDevice->SetRenderState(D3DRS_LIGHTING, true);
	if (pEffect)
	{
		pEffect->SetVector(LIGHTDIFFUSE, &LightDiffuse);
		pEffect->SetVector(LIGHTAMBIENT, &LightAmbient);
	}
	
}
void DirectX::light_off()
{
	pD3DXDevice->SetRenderState(D3DRS_LIGHTING, false);
	if (pEffect)
	{
		pEffect->SetVector(LIGHTDIFFUSE, new D3DXVECTOR4(0, 0, 0, 0));
		pEffect->SetVector(LIGHTAMBIENT, new D3DXVECTOR4(1, 1, 1, 1));
	}
}
DirectX::~DirectX()
{
	SAFE_RELEASE(pD3DXDevice);
	SAFE_RELEASE(font);
	SAFE_DELETE(snowMan1);
	SAFE_DELETE(snowMan2);
	SAFE_DELETE(pCamera);
	SAFE_DELETE(cube);
	SAFE_DELETE(terrian);
	SAFE_DELETE(skyBox);
	SAFE_DELETE(pSnowParticle);
	SAFE_DELETE(pDirectInput);
	SAFE_DELETE(pTree);
	SAFE_DELETE(pTree1);
}