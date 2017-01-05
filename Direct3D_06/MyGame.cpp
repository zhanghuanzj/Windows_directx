#include "MyDirectX.h"
#include "Sprite.h"

const string APPTITLE = "Animate of Sprite";
const int SCREENW = 1024;
const int SCREENH = 768;

Sprite ship, asteroid1, asteroid2;
Sprite hero;

LPDIRECT3DTEXTURE9 imgShip = NULL;
LPDIRECT3DTEXTURE9 imgAsteroid = NULL;


bool Game_Init(HWND hwnd)
{
	srand((unsigned int)time(NULL));
	Direct3D_Init(hwnd,SCREENW,SCREENH,false);
	DirectInput_Init(hwnd);

	//load the sprite textures
	imgShip = LoadTexture("fatship.tga");
	if (!imgShip) return false;
	imgAsteroid = LoadTexture("asteroid.tga");
	if (!imgAsteroid) return false;

	//set properties for sprites
	ship.x = 450;
	ship.y = 300;
	ship.width = ship.height = 128;

	asteroid1.x = 50;
	asteroid1.y = 200;
	asteroid1.width = asteroid1.height = 60;
	asteroid1.columns = 8;
	asteroid1.startframe = 0;
	asteroid1.endframe = 63;
	asteroid1.velx = -2.0f;

	asteroid2.x = 900;
	asteroid2.y = 500;
	asteroid2.width = asteroid2.height = 60;
	asteroid2.columns = 8;
	asteroid2.startframe = 0;
	asteroid2.endframe = 63;
	asteroid2.velx = 2.0f;

	return true;
}

void Game_Run(HWND window)
{
	//make sure the Direct3D device is valid
	if (!d3ddev) return;
	//update input devices
	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET , D3DCOLOR_XRGB(0,0,100), 1.0f, 0);

	//move the ship up/down with arrow keys
	if (Key_Down(DIK_UP))
	{
		ship.y -= 1.0f;
		if (ship.y < 0) ship.y = 0;
	}

	if (Key_Down(DIK_DOWN)) 
	{
		ship.y += 1.0f;
		if (ship.y > SCREENH - ship.height)
			ship.y = SCREENH - ship.height;
	}

	//move and animate the asteroids
	asteroid1.x += asteroid1.velx;
	if (asteroid1.x < 0 || asteroid1.x > SCREENW-asteroid1.width)
		asteroid1.velx *= -1;
	UpdateNextFrameNumber(asteroid1.frame, asteroid1.startframe, asteroid1.endframe, asteroid1.direction, asteroid1.starttime, asteroid1.delay);

	asteroid2.x += asteroid2.velx;
	if (asteroid2.x < 0 || asteroid2.x > SCREENW-asteroid2.width)
		asteroid2.velx *= -1;
	UpdateNextFrameNumber(asteroid2.frame, asteroid2.startframe, asteroid2.endframe, asteroid2.direction, asteroid2.starttime, asteroid2.delay);

	//test for collisions
	if (ship.collision(asteroid1))
		asteroid1.velx *= -1;

	if (ship.collision(asteroid2))
		asteroid2.velx *= -1;


	//start rendering
	if (d3ddev->BeginScene())
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		DrawSpriteTransform(imgShip, ship.x, ship.y, ship.width, ship.height, ship.frame, ship.columns);

		DrawSpriteTransform(imgAsteroid, asteroid1.x, asteroid1.y, asteroid1.width, asteroid1.height, asteroid1.frame, asteroid1.columns);

		DrawSpriteTransform(imgAsteroid, asteroid2.x, asteroid2.y, asteroid2.width, asteroid2.height, asteroid2.frame, asteroid2.columns);

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
	if (imgShip) imgShip->Release();
	if (imgAsteroid) imgAsteroid->Release();
	DirectInput_ShutDown();
	Direct3D_ShutDown();
}