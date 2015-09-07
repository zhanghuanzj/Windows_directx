#ifndef MYDIRECTX_H
#define MYDIRECTX_H

#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <io.h>
#include <algorithm>
#include "DirectSound.h"
using namespace std;

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxerr.lib")

extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameOver;
extern CSoundManager *dsound;

//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE spriteobj;

//Direct3D functions
bool Direct3D_Init(HWND hwnd,int width,int height,bool fullScreen);
void Direct3D_ShutDown();
LPDIRECT3DSURFACE9 LoadSurface(string fileName);
LPDIRECT3DTEXTURE9 LoadTexture(string fileName,D3DCOLOR transColor=D3DCOLOR_XRGB(0,0,0));
LPD3DXFONT MakeFont(string fontName,int size);
D3DXVECTOR2 GetBitmapSize(string fileName); 
void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 src);
void DrawSpriteFrame2Texture(LPDIRECT3DTEXTURE9 texture,int destx,int dexty,int framenum,int frameW,int frameH,int columns);
void DrawSpriteTransform(LPDIRECT3DTEXTURE9 texture,int x,int y,int width,int height,int frame=0,int columns=1,float rotation=0.0f,float scaling=1.0f,D3DCOLOR color=D3DCOLOR_XRGB(255,255,255));
void UpdateNextFrameNumber(int &frameNumber,int startFrame,int endFrame,int direction,int &startTime,int delay);
void PrintFont(LPD3DXFONT font,int x,int y,string text,D3DCOLOR color=D3DCOLOR_XRGB(255,255,255));


//DirectInput objects,devices,and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyBoard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];

//DirectInput functions
bool DirectInput_Init(HWND hwnd);
void DirectInput_Update();
void DirectInput_ShutDown();
//Input state check
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_y();
void XInput_Vibrate(int countNum = 0,int amount = 65535);
bool XInput_Controller_Found();

//DirectSound function
bool DirectSound_Init(HWND hwnd);
void DirectSound_ShutDown();
CSound* LoadSound(string fileName);
void PlaySound(CSound *sound);
void LoopSound(CSound *sound);
void StopSound(CSound *sound);

//Game functions
bool Game_Init(HWND hwnd);
void Game_Run(HWND hwnd);
void Game_End();

//3D Mesh code added in chapter 13

//define the MODEL struct
struct MODEL
{
	LPD3DXMESH mesh;
	D3DMATERIAL9* materials;
	LPDIRECT3DTEXTURE9* textures;
	DWORD material_count;
	D3DXVECTOR3 translate;
	D3DXVECTOR3 rotate;
	D3DXVECTOR3 scale;

	MODEL()
	{
		material_count = 0;
		mesh = NULL;
		materials = NULL;
		textures = NULL;
		translate = D3DXVECTOR3(0.0f,0.0f,0.0f);
		rotate = D3DXVECTOR3(0.0f,0.0f,0.0f);
		scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	}
};

//3D mesh function prototypes
void DrawModel(MODEL *model);
void DeleteModel(MODEL *model);
MODEL *LoadModel(string filename);
bool FindFile(string *filename);
bool DoesFileExist(const string &filename);
void SplitPath(const string& inputPath, string* pathOnly, string* filenameOnly);
void SetCamera(float posx, float posy, float posz, float lookx = 0.0f, float looky=0.0f, float lookz=0.0f);

#endif // !MYDIRECTX_H
