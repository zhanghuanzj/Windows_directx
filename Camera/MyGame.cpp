
#include "MyDirectX.h"
#include "DirecXInput.h"
#include "Camera.h"
using namespace std;

const string APPTITLE = "Stock Mesh Demo";
const int SCREENW = 1024;
const int SCREENH = 768;

DWORD screentimer = timeGetTime();

ID3DXMesh *mesh=NULL;                          //............................
ID3DXMesh *wall=nullptr;
MyDirectXInput *myInput;
Camera *myCamera=nullptr;
//new objects...........................
DWORD nums;
D3DMATERIAL9 *mats=nullptr;
D3DMATERIAL9 wallMaterial;
LPDIRECT3DTEXTURE9 *textures=nullptr;
LPD3DXFONT font = nullptr;
string s;
D3DXMATRIX g_matWorld;
//new objects...........................


void View_Change()
{
	//移动
	if (myInput->Key_Down(DIK_A))   myCamera->MoveAlongRightVec(-0.3f);
	if (myInput->Key_Down(DIK_D))   myCamera->MoveAlongRightVec(0.3f);
	if (myInput->Key_Down(DIK_W))   myCamera->MoveAlongLookVec(0.3f);
	if (myInput->Key_Down(DIK_S))   myCamera->MoveAlongLookVec(-0.3f);
	if (myInput->Key_Down(DIK_Z))   myCamera->MoveAlongUpVec(0.3f);
	if (myInput->Key_Down(DIK_X))   myCamera->MoveAlongUpVec(-0.3f);
	//旋转
	if (myInput->Key_Down(DIK_UP))	   myCamera->RotationRightVec(-0.003f);
	if (myInput->Key_Down(DIK_DOWN))   myCamera->RotationRightVec(0.003f);
	if (myInput->Key_Down(DIK_LEFT))   myCamera->RotationUpVec(-0.003f);
	if (myInput->Key_Down(DIK_RIGHT))  myCamera->RotationUpVec(0.003f); 
	if (myInput->Key_Down(DIK_U))      myCamera->RotationLookVec(0.003f);
	if (myInput->Key_Down(DIK_I))      myCamera->RotationLookVec(-0.003f);
	myCamera->SetViewMatrix(nullptr);


	if (myInput->Key_Down(DIK_J))
	{
		d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

	}
	else if (myInput->Key_Down(DIK_K))
	{
		d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);

	}
	else if (myInput->Key_Down(DIK_1))
	{
		for (int i=0;i<nums;i++)
		{
			mats[i].Diffuse.a-=0.01f;
		}
	}
	else if (myInput->Key_Down(DIK_2))
	{
		for (int i=0;i<nums;i++)
		{
			mats[i].Diffuse.a+=0.01f;
		}
	}
}


bool Game_Init(HWND window)
{
	srand( (int)time(NULL) );
	//initialize Direct3D
	if (!Direct3D_Init(window, SCREENW, SCREENH, false))
	{
		MessageBox(window,"Error initializing Direct3D",APPTITLE.c_str(),0);
		return false;
	}

	//initialize DirectInput
	myInput = new MyDirectXInput();
	if (!myInput->DirectInput_Init(window))
	{
		MessageBox(window, "Error initializing DirectInput", APPTITLE.c_str(), 0);
		return false;
	}

	//initialize DirectSound
	if (!DirectSound_Init(window))
	{
		MessageBox(window, "Error initializing DirectSound", APPTITLE.c_str(), 0);
		return false;
	}


	//set the camera position
	myCamera = new Camera(d3ddev);
	D3DXVECTOR3 *position = new D3DXVECTOR3(0.0f, 0.0f, -500.0f);
	myCamera->SetCameraPosition(position);
	myCamera->SetViewMatrix(nullptr);           //相机两大视角，View&Project
	myCamera->SetProjMatrix(nullptr);

	//use ambient lighting and z-buffering
	/*d3ddev->SetRenderState(D3DRS_ZENABLE, true);*/
	//new initialize.......................................................................
	font=MakeFont("Times New Roman",40);
	if (font==nullptr)
	{
		MessageBox(window, "Error initializing DirectSound", APPTITLE.c_str(), 0);
		return false;
	}
	LPD3DXBUFFER adjBuffer=nullptr;
	LPD3DXBUFFER matBuffer=nullptr;
	D3DXLoadMeshFromX("mesh/miki.X",D3DXMESH_MANAGED,d3ddev,&adjBuffer,&matBuffer,NULL,&nums,&mesh);

	D3DXMATERIAL *materials = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
	mats = new D3DMATERIAL9[nums];
	textures = new LPDIRECT3DTEXTURE9[nums];
	for (int i=0;i<nums;i++)
	{
		mats[i]=materials[i].MatD3D;
		mats[i].Ambient=mats[i].Diffuse;

		textures[i]=NULL;
		string path="mesh/";
		path = path+materials[i].pTextureFilename;
		D3DXCreateTextureFromFileA(d3ddev,path.c_str(),&textures[i]);
	}
	s = materials[0].pTextureFilename;
	SAFE_RELEASE(adjBuffer);
	SAFE_RELEASE(matBuffer);

	//wall
	D3DXCreateBox(d3ddev, 100.0f, 100.0f, 10.0f, &wall, NULL);
	wallMaterial.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	wallMaterial.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	wallMaterial.Specular = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
	//Alpha 融合
	//d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//d3ddev->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	//new initialize...................................................................

	

	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Ambient = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
	light.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	light.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	light.Position = D3DXVECTOR3(0.0f,200.0f,0.0f);
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Range = 300.0f;

	d3ddev->SetLight(0,&light);
	d3ddev->LightEnable(0,true);
	d3ddev->SetRenderState(D3DRS_LIGHTING, true); 
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	d3ddev->SetRenderState(D3DRS_SPECULARENABLE,true);
	//d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//d3ddev->SetRenderState(D3DRS_SPECULARENABLE, true);
	//d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//d3ddev->SetRenderState(D3DRS_ZENABLE, true);   
	//d3ddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);   
	//d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, true); 
	//D3DMATERIAL9 material;
	//ZeroMemory(&material,sizeof(material));
	//material.Ambient = D3DXCOLOR(0.5f,0.5f,0.7f,1.0f);
	//material.Diffuse = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
	//material.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,0.3f);
	//material.Emissive = D3DXCOLOR(0.3f,0.0f,0.1f,1.0f);
	//d3ddev->SetMaterial(&material);

	return true;
}

void Game_Run(HWND window)
{
	UINT numPasses = 0;
	static float y = 0.0;

	if (!d3ddev) return;
	myInput->DirectInput_UpdateState();
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL , D3DCOLOR_XRGB(100, 0, 0), 1.0f, 0);

	// slow rendering to approximately 60 fps
	if (timeGetTime() > screentimer + 14)
	{
		screentimer = GetTickCount();
		//start rendering
		if (d3ddev->BeginScene())
		{		
			//Change the camera view
			View_Change();
			//concrete rendering............................................................
			D3DXMATRIX matWorld;
			D3DXMatrixTranslation(&matWorld, 0.0f,0.0f,0.0f);
			d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
			for (int i=0;i<nums;i++)
			{
				d3ddev->SetMaterial(&mats[i]);
				d3ddev->SetTexture(0,textures[i]);
				mesh->DrawSubset(i);	
			}
			//font
			RECT rect={0,0,0,0};
			char strInfo[30];
			sprintf_s(strInfo, "(%.2f, %.2f, %.2f)", g_matWorld._41, g_matWorld._42, g_matWorld._43);
			string message = strInfo;
			font->DrawTextA(nullptr,message.c_str(),message.length(),&rect,DT_SINGLELINE | DT_NOCLIP | DT_LEFT,D3DCOLOR_XRGB(255,0,0));
			//wall
		
			D3DXMatrixTranslation(&matWorld, 0.0f,0.0f,0.0f);
			d3ddev->SetTransform(D3DTS_WORLD, &matWorld);
			d3ddev->SetMaterial(&wallMaterial);
			wall->DrawSubset(0);
			


			d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			d3ddev->SetRenderState( D3DRS_STENCILENABLE,   false);
			d3ddev->SetRenderState(D3DRS_CULLMODE,         D3DCULL_CCW);

			//concrete rendering............................................................

			//stop rendering
			d3ddev->EndScene();
			d3ddev->Present(NULL, NULL, NULL, NULL);
		}
	}


	if (myInput->Key_Down(DIK_ESCAPE)) gameOver = true;
}

void Game_End()
{
	SAFE_RELEASE(mesh);
	delete mats;
	delete textures;
	DirectSound_ShutDown();
	if (myInput)
	{
		delete myInput;
	}
	Direct3D_ShutDown();
}