#include "MyDirectX.h"
#include "Sprite.h"

const string APPTITLE = "Animate of Sprite";
const int SCREENW = 1024;
const int SCREENH = 768;

//declare some font objects
LPD3DXFONT fontArial24 = NULL;
LPD3DXFONT fontGaramond36 = NULL;
LPD3DXFONT fontTimesNewRoman40 = NULL;


bool Game_Init(HWND hwnd)
{
	srand((unsigned int)time(NULL));
	Direct3D_Init(hwnd,SCREENW,SCREENH,false);
	DirectInput_Init(hwnd);

	//create some fonts
	fontArial24 = MakeFont("Arial",24);
	fontGaramond36 = MakeFont("Garamond",36);
	fontTimesNewRoman40 = MakeFont("Times New Roman", 40);

	return true;
}

void Game_Run(HWND window)
{
	//make sure the Direct3D device is valid
	if (!d3ddev) return;
	//update input devices
	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(0,0,100), 1.0f, 0);


	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//demonstrate font output
		PrintFont(fontArial24, 60, 50, "This is the Arial 24 font printed with ID3DXFont");

		PrintFont(fontGaramond36, 60, 100, "The text can be printed in any color like this magenta!", D3DCOLOR_XRGB(255,0,255));

		PrintFont(fontTimesNewRoman40, 60, 150, "Or how about bright green instead?", D3DCOLOR_XRGB(0,255,0));


		//demonstrate text wrapping inside a rectangular region
		RECT rect = { 60, 250, 350, 700 };
		D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);
		string text = "This is a long string that will be ";
		text += "wrapped inside a rectangle.";
		fontTimesNewRoman40->DrawText( spriteobj, text.c_str(), text.length(), &rect, DT_WORDBREAK, white); 

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
	if (fontArial24) fontArial24->Release(); 
	if (fontGaramond36) fontGaramond36->Release();
	if (fontTimesNewRoman40) fontTimesNewRoman40->Release();
	DirectInput_ShutDown();
	Direct3D_ShutDown();
}