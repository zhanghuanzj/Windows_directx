#include "MyDirectX.h"

const string APPTITLE = "Animate of Sprite";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3DTEXTURE9 sunflower = NULL;
D3DCOLOR color;
int framNumber = 0;
int startTime = 0;


bool Game_Init(HWND hwnd)
{
	srand((unsigned int)time(NULL));
	Direct3D_Init(hwnd,SCREENW,SCREENH,false);
	DirectInput_Init(hwnd);

	//something new to initialize
	sunflower = LoadTexture("sunflower.bmp");
	if (!sunflower)
	{
		return false;
	}

	return true;
}

void Game_Run(HWND window)
{
	static float scale = 0.001f;
	static float r = 0;
	static float s = 1.0f;
	//make sure the Direct3D device is valid
	if (!d3ddev) return;
	//update input devices
	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0);

	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		r = timeGetTime()/600.0f;
		s += scale;
		if (s<0.1||s>1.25f)
		{
			scale *=-1;
		}
		color = D3DCOLOR_XRGB(255,255,255);
		DrawSpriteTransform(sunflower,300,150,512,512,framNumber,1,r,s,color);

		//stop drawing
		spriteobj->End();

		//stop rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//Escape key ends program
	if (Key_Down(VK_ESCAPE)) gameOver = true;

	//controller Back button also ends
	if (controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
		gameOver = true;
}

void Game_End()
{
	//free memory and shut down
	sunflower->Release();
	DirectInput_ShutDown();
	Direct3D_ShutDown();
}