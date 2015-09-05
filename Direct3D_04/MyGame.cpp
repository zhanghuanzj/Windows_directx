#include "MyDirectX.h"

const string APPTITLE = "Animate of Sprite";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3DTEXTURE9 explosion = NULL;
int framNumber = 0;
int startTime = 0;


bool Game_Init(HWND hwnd)
{
	srand((unsigned int)time(NULL));
	Direct3D_Init(hwnd,SCREENW,SCREENH,false);
	DirectInput_Init(hwnd);

	//something new to initialize
	explosion = LoadTexture("explosion_30_128.tga");
	if (!explosion)
	{
		return false;
	}

	return true;
}

void Game_Run(HWND window)
{
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

		UpdateNextFrameNumber(framNumber,0,29,1,startTime,30);
		DrawSpriteFrame2Texture(explosion,200,200,framNumber,128,128,6);

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
	explosion->Release();
	DirectInput_ShutDown();
	Direct3D_ShutDown();
}