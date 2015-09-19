
#include "MyDirectX.h"
#include "DirecXInput.h"
#include "Camera.h"
#include "Terrain.h"
#include "SkyBox.h"
using namespace std;

const string APPTITLE = "Stock Mesh Demo";
const int SCREENW = 1024;
const int SCREENH = 768;
const float PERSTEP = 100.0f;
const float PERANG = 0.01f;

DWORD screentimer = timeGetTime();

ID3DXMesh *mesh=NULL;                          //............................
ID3DXMesh *cylinder=nullptr;
MyDirectXInput *myInput;
Camera *myCamera=nullptr;
Terrain *myTerrain=nullptr;
SkyBox *mySkyBox=nullptr;
//new objects...........................
DWORD nums;
D3DMATERIAL9 *mats=nullptr;
D3DMATERIAL9 cylinderMat;
LPDIRECT3DTEXTURE9 *textures=nullptr;
LPD3DXFONT font = nullptr;
string s;
D3DXMATRIX g_matWorld;
//new objects...........................


void View_Change()
{
	//移动
	if (myInput->Key_Down(DIK_A))   myCamera->MoveAlongRightVec(-PERSTEP);
	if (myInput->Key_Down(DIK_D))   myCamera->MoveAlongRightVec(PERSTEP);
	if (myInput->Key_Down(DIK_W))   myCamera->MoveAlongLookVec(PERSTEP);
	if (myInput->Key_Down(DIK_S))   myCamera->MoveAlongLookVec(-PERSTEP);
	if (myInput->Key_Down(DIK_Z))   myCamera->MoveAlongUpVec(PERSTEP);
	if (myInput->Key_Down(DIK_X))   myCamera->MoveAlongUpVec(-PERSTEP);
	//旋转
	if (myInput->Key_Down(DIK_UP))	   myCamera->RotationRightVec(-PERANG);
	if (myInput->Key_Down(DIK_DOWN))   myCamera->RotationRightVec(PERANG);
	if (myInput->Key_Down(DIK_LEFT))   myCamera->RotationUpVec(-PERANG);
	if (myInput->Key_Down(DIK_RIGHT))  myCamera->RotationUpVec(PERANG); 
	if (myInput->Key_Down(DIK_U))      myCamera->RotationLookVec(PERANG);
	if (myInput->Key_Down(DIK_I))      myCamera->RotationLookVec(-PERANG);
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
	D3DXVECTOR3 *position = new D3DXVECTOR3(0.0f, 12000.0f, -30000.0f);
	myCamera->SetCameraPosition(position);
	myCamera->SetTargetPosition(&D3DXVECTOR3(0.0f,10000.0f,0.0f));
	myCamera->SetViewMatrix(nullptr);           //相机两大视角，View&Project
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)((double)SCREENW/SCREENH), 1.0f, 3000000.0f);
	myCamera->SetProjMatrix(&matProj);

	//new initialize.......................................................................
	//font
	font=MakeFont("Times New Roman",40);
	if (font==nullptr)
	{
		MessageBox(window, "Error initializing DirectSound", APPTITLE.c_str(), 0);
		return false;
	}

	//role modle
	LPD3DXBUFFER adjBuffer=nullptr;
	LPD3DXBUFFER matBuffer=nullptr;
	D3DXLoadMeshFromX("mesh/bee.X",D3DXMESH_MANAGED,d3ddev,&adjBuffer,&matBuffer,NULL,&nums,&mesh);
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


	//cylinder
	D3DXCreateCylinder(d3ddev, 8000.0f, 100.0f, 50000.0f, 60, 60,  &cylinder, 0);
	cylinderMat.Ambient  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);  
	cylinderMat.Diffuse  = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);  
	cylinderMat.Specular = D3DXCOLOR(0.5f, 0.0f, 0.3f, 0.3f);  
	cylinderMat.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//terrain
	myTerrain= new Terrain(d3ddev);   
	myTerrain->LoadTerrainFromFile("mesh/heighmap.raw", "mesh/green.jpg");		
	myTerrain->InitTerrain(200, 200, 300.0f, 70.0f);  
	
	//SkyBox
	mySkyBox = new SkyBox(d3ddev);
	mySkyBox->LoadSkyTextureFromFile("sky/frontsnow1.jpg","sky/backsnow1.jpg","sky/leftsnow1.jpg","sky/rightsnow1.jpg","sky/topsnow1.jpg");
	mySkyBox->InitSkyBox(2000000);

	D3DLIGHT9 light;
	ZeroMemory(&light,sizeof(light));
	light.Type          = D3DLIGHT_DIRECTIONAL;  
	light.Ambient       = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);  
	light.Diffuse       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  
	light.Specular      = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);  
	light.Direction     = D3DXVECTOR3(1.0f, 1.0f, 1.0f); 


	d3ddev->SetLight(0,&light);
	d3ddev->LightEnable(0,true);
	d3ddev->SetRenderState(D3DRS_LIGHTING, true); 
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS,true);
	d3ddev->SetRenderState(D3DRS_SPECULARENABLE,true);
	

	return true;
}

void Game_Run(HWND window)
{

	if (!d3ddev) return;
	myInput->DirectInput_UpdateState();
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL , D3DCOLOR_XRGB(0, 108, 255), 1.0f, 0);

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

			D3DXMATRIX mScal,mRot1,mRot2,mTrans,mFinal;   
			D3DXMatrixScaling(&mScal,20.0f,20.0f,20.0f);  
			D3DXMatrixTranslation(&mTrans,0,8000,0);
			D3DXMatrixRotationX(&mRot1, D3DX_PI/2); 
			D3DXMatrixRotationY(&mRot2, D3DX_PI/2); 
			mFinal=mScal*mRot1*mRot2*mTrans*matWorld;
			d3ddev->SetTransform(D3DTS_WORLD, &mFinal);
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

			//Terrain
			myTerrain->RenderTerrain(&matWorld, false);

			//SkyBox
			D3DXMATRIX matSky,matTransSky,matRotSky;
			D3DXMatrixTranslation(&matTransSky,0.0f,-3500.0f,0.0f);
			D3DXMatrixRotationY(&matRotSky,-0.000005f*timeGetTime());
			matSky = matRotSky*matTransSky;
			mySkyBox->RenderSkyBox(&matSky,FALSE);
			//Cylinder
			D3DXMATRIX TransMatrix, RotMatrix, FinalMatrix;
			D3DXMatrixRotationX(&RotMatrix, -D3DX_PI * 0.5f);
			d3ddev->SetMaterial(&cylinderMat);
			for(int i = 0; i < 4; i++)
			{
				D3DXMatrixTranslation(&TransMatrix, -10000.0f, 0.0f, -15000.0f + (i * 20000.0f));
				FinalMatrix = RotMatrix * TransMatrix ;
				d3ddev->SetTransform(D3DTS_WORLD, &FinalMatrix);
				cylinder->DrawSubset(0);

				D3DXMatrixTranslation(&TransMatrix, 10000.0f, 0.0f, -15000.0f + (i * 20000.0f));
				FinalMatrix = RotMatrix * TransMatrix ;
				d3ddev->SetTransform(D3DTS_WORLD, &FinalMatrix);
				cylinder->DrawSubset(0);
			}
		


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