//#include "MyDirectX.h"
//#include "Sprite.h"
//
//const string APPTITLE = "Animate of Sprite";
//const int SCREENW = 1024;
//const int SCREENH = 768;
//
//D3DXMATRIX mProj, mView, mWorld, mViewProj;
//D3DXMATRIX mTrans, mRot, mScale;
//D3DXVECTOR3 vTrans, vRot, vScale;
//LPD3DXMESH torus = NULL;
//ID3DXEffect *shader1 = NULL;
//
//
//void SetCamera(float x, float y, float z)
//{
//	double p_fov = D3DX_PI / 4.0;
//	double p_aspectRatio = 1024 / 768;
//	double p_nearRange = 1.0;
//	double p_farRange = 2000.0;
//	D3DXVECTOR3 p_updir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//	D3DXVECTOR3 p_position = D3DXVECTOR3(x, y, z);
//	D3DXVECTOR3 p_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3 p_target = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//	//set the camera's view and perspective matrix
//	D3DXMatrixPerspectiveFovLH(&mProj, 
//		(float)p_fov, 
//		(float)p_aspectRatio, 
//		(float)p_nearRange, 
//		(float)p_farRange);
//
//	D3DXMatrixLookAtLH(&mView, &p_position, &p_target, &p_updir);
//
//	//optimization
//	mViewProj = mView * mProj;
//}
//
//bool Game_Init(HWND hwnd)
//{
//	srand((unsigned int)time(NULL));
//	Direct3D_Init(hwnd,SCREENW,SCREENH,false);
//	DirectInput_Init(hwnd);
//	DirectSound_Init(hwnd);
//
//	// create a torus mesh
//	D3DXCreateTorus(d3ddev, 0.5f, 1.0f, 40, 40, &torus, NULL);
//
//
//	//set the camera position
//	SetCamera(0.0,1.0,-20.0f);
//
//	
//
//	return true;
//}
//
//void Game_Run(HWND window)
//{
//	UINT numPasses = 0;
//	static float y = 0.0;
//
//	if (!d3ddev) return;
//	DirectInput_Update();
//	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(250,250, 250), 1.0f, 0);
//
//	//transform the mesh
//	y += 0.001;
//	D3DXMatrixRotationYawPitchRoll(&mRot, y, 0.0f, 0.0f);
//	D3DXMatrixTranslation(&mTrans, 0.0f, 0.0f, 0.0f);
//	D3DXMatrixScaling(&mScale, 4.0f, 4.0f, 4.0f);
//
//	//pass the matrix to the shader via a parameter
//	mWorld = mRot * mScale * mTrans;
//
//
//	//rendering
//	if (d3ddev->BeginScene())
//	{
//			
//		torus->DrawSubset(0);
//
//		d3ddev->EndScene();
//	}
//	d3ddev->Present(NULL, NULL, NULL, NULL);
//
//
//	//controller Back button also ends
//	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
//		gameOver = true;
//}
//
//
//
//void Game_End()
//{
//	torus->Release();
//	DirectSound_ShutDown();
//	DirectInput_ShutDown();
//	Direct3D_ShutDown();
//}

// Beginning Game Programming
// MyGame.cpp

#include "MyDirectX.h"
using namespace std;

const string APPTITLE = "Stock Mesh Demo";
const int SCREENW = 1024;
const int SCREENH = 768;

DWORD screentimer = timeGetTime();

MODEL *mesh=NULL;





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
	if (!DirectInput_Init(window))
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
	SetCamera( 0.0f, -800.0f, -200.0f );

	//use ambient lighting and z-buffering
	d3ddev->SetRenderState(D3DRS_ZENABLE, true);
	d3ddev->SetRenderState(D3DRS_LIGHTING, false); 

	//load the mesh file
	mesh = LoadModel("Fokker.X");
	if (mesh == NULL)
	{
		MessageBox(window, "Error loading mesh", APPTITLE.c_str(), MB_OK);
		return 0;
	}

	return true;
}

void Game_Run(HWND window)
{
	UINT numPasses = 0;
	static float y = 0.0;

	if (!d3ddev) return;
	DirectInput_Update();
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// slow rendering to approximately 60 fps
	if (timeGetTime() > screentimer + 14)
	{
		screentimer = GetTickCount();

		//start rendering
		if (d3ddev->BeginScene())
		{
			//rotate the view
			D3DXMATRIX matWorld;
			D3DXMatrixRotationY(&matWorld, timeGetTime()/1000.0f);
			d3ddev->SetTransform(D3DTS_WORLD, &matWorld);

			//draw the model
			DrawModel(mesh);

			//stop rendering
			d3ddev->EndScene();
			d3ddev->Present(NULL, NULL, NULL, NULL);
		}
	}


	if (Key_Down(VK_ESCAPE)) gameOver = true;
}

void Game_End()
{
	DeleteModel(mesh);
	DirectSound_ShutDown();
	DirectInput_ShutDown();
	Direct3D_ShutDown();
}