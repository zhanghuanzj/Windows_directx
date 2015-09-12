
#include "MyDirectX.h"
#include "DirecXInput.h"
using namespace std;

const string APPTITLE = "Stock Mesh Demo";
const int SCREENW = 1024;
const int SCREENH = 768;

DWORD screentimer = timeGetTime();

ID3DXMesh *mesh=NULL;                          //............................
MyDirectXInput *myInput;
//new objects...........................
DWORD nums;
D3DMATERIAL9 *mats;
LPDIRECT3DTEXTURE9 *textures;
LPD3DXFONT font = nullptr;
string s;
D3DXMATRIX g_matWorld;
//new objects...........................


void View_Change()
{
	static FLOAT fPosX = 0.0f, fPosY = 0.0f, fPosZ = 0.0f;

	
	if (myInput->Key_Down(DIK_A)) fPosX -= 0.05f;
	if (myInput->Key_Down(DIK_D)) fPosX += 0.05f;
	if (myInput->Key_Down(DIK_W)) fPosY += 0.05f;
	if (myInput->Key_Down(DIK_S)) fPosY -= 0.05f;

	D3DXMatrixTranslation(&g_matWorld, fPosX, fPosY, fPosZ);


	static float fAngleX = 0, fAngleY =0;

	
	if (myInput->Key_Down(DIK_UP)) fAngleX += 0.05f;
	if (myInput->Key_Down(DIK_DOWN)) fAngleX -= 0.05f;
	if (myInput->Key_Down(DIK_LEFT)) fAngleY -= 0.05f;
	if (myInput->Key_Down(DIK_RIGHT)) fAngleY += 0.05f;


	D3DXMATRIX Rx, Ry;
	D3DXMatrixRotationX(&Rx, fAngleX);
	D3DXMatrixRotationY(&Ry, fAngleY);

	g_matWorld = Rx * Ry * g_matWorld;
	d3ddev->SetTransform(D3DTS_WORLD, &g_matWorld);
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
	SetCamera( 0.0f, 0.0f, -300.0f );

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
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// slow rendering to approximately 60 fps
	if (timeGetTime() > screentimer + 14)
	{
		screentimer = GetTickCount();

		//start rendering
		if (d3ddev->BeginScene())
		{
			//rotate the view
			View_Change();

			if (myInput->Key_Down(DIK_J))
			{
				d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
				
			}
			else if (myInput->Key_Down(DIK_K))
			{
				d3ddev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		
			}

			//concrete rendering............................................................
			for (int i=0;i<nums;i++)
			{
				d3ddev->SetMaterial(&mats[i]);
				d3ddev->SetTexture(0,textures[i]);
				mesh->DrawSubset(i);	
			}
			RECT rect={0,0,0,0};
			font->DrawTextA(nullptr,s.c_str(),s.length(),&rect,DT_SINGLELINE | DT_NOCLIP | DT_LEFT,D3DCOLOR_XRGB(255,0,0));
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