#include "Direct3D.h"

bool DirectX::initialDirectX(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	//1.创建接口
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//2.获取硬件信息，确定顶点处理方式
	D3DCAPS9 caps;
	int vertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);    //(当前显卡，硬件设备)
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.填写D3D参数
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

	//4.创建设备
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vertexProcessing, &d3dpp, &pD3DXDevice);
	if (pD3DXDevice == nullptr)
		cout << "ERROR" << endl;
	d3d9->Release();

	//保存屏幕宽、高、Z缓冲区大小
	width_ = width;
	height_ = height;

	//Camera
	pCamera = new Camera(pD3DXDevice,width,height);

	pDirectInput = new DirectInput();
	pDirectInput->init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//Font setting
	font = nullptr;
	D3DXCreateFont(pD3DXDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "微软雅黑", &font);
	GetClientRect(hwnd, &formatRect);

	snowMan1 = new SnowMan(pD3DXDevice);
	snowMan2 = new SnowMan(pD3DXDevice);
	cube = new Cube(pD3DXDevice);
	terrian = new Terrian(pD3DXDevice);
	terrian->load_terrain_from_file("terrian.raw", "land.jpg");
	terrian->init_terrain(200, 200, 300.0f, 18.0f);

	skyBox = new SkyBox(pD3DXDevice,1500);
	skyBox->loadSkyTextureFromFile("Textures\\frontsnow.jpg","Textures\\backsnow.jpg","Textures\\leftsnow.jpg","Textures\\rightsnow.jpg", "Textures\\topsnow.jpg");

	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	pD3DXDevice->SetLight(0, &light);
	pD3DXDevice->LightEnable(0, true);

	pD3DXDevice->SetRenderState(D3DRS_LIGHTING, false);
	pD3DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pD3DXDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	pD3DXDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));   //设置一下环境光
	pD3DXDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);

	return true;
}

void DirectX::update()
{
	pDirectInput->get_input();
	pCamera->update(pDirectInput);
	
}

void DirectX::snowmanRender()
{
	pD3DXDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pD3DXDevice->BeginScene();
	
	update();
	formatRect.top = 100;
	font->DrawText(0, "【致我们永不熄灭的游戏开发梦想】", -1, &formatRect, DT_CENTER, D3DCOLOR_XRGB(68, 139, 256));
	static float n = 1;
	float angle = n*0.03f;
	D3DXMATRIX rotate;
	D3DXMatrixRotationY(&rotate,angle);
	++n;
	if (angle >= 360) n = 0;
	D3DXVECTOR3 pos(-8.0f, -8.0f, 0.0f);
	D3DXMATRIX moveMatrix;
	D3DXMatrixTranslation(&moveMatrix, pos.x, pos.y, pos.z);
	moveMatrix *= rotate;//move then rotate
	pD3DXDevice->SetTransform(D3DTS_WORLD, &moveMatrix);
	snowMan1->draw_snowMan();

	D3DXMATRIX cubeMatrix;
	D3DXMatrixTranslation(&cubeMatrix, pos.x, pos.y-4, pos.z);
	cubeMatrix *= rotate;
	pD3DXDevice->SetTransform(D3DTS_WORLD, &cubeMatrix);
	cube->draw_cube();

	D3DXMATRIX d;
	D3DXMatrixTranslation(&d, pos.x+4, pos.y-2, pos.z+10);
	pD3DXDevice->SetTransform(D3DTS_WORLD, &d);
	snowMan2->draw_snowMan();

	D3DXMATRIX terrianMatrix;
	D3DXMatrixTranslation(&terrianMatrix, 0.0f, 0.0f, 0.0f);
	pD3DXDevice->SetTransform(D3DTS_WORLD, &terrianMatrix);
	terrian->render_terrain(&terrianMatrix);
	D3DMATERIAL9 snowMtrl;
	::ZeroMemory(&snowMtrl, sizeof(snowMtrl));
	snowMtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	snowMtrl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	snowMtrl.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	pD3DXDevice->SetMaterial(&snowMtrl);
	D3DXMATRIX skyBoxMatrix;
	D3DXMatrixTranslation(&skyBoxMatrix, 0.0f, -30.0f, 0.0f);
	skyBox->renderSkyBox(&skyBoxMatrix);

	pD3DXDevice->EndScene();
	pD3DXDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
DirectX::~DirectX()
{
	SAFE_RELEASE(pD3DXDevice);
	SAFE_RELEASE(font);
	delete snowMan1;
	delete snowMan2;
	delete pCamera;
	delete cube;
}