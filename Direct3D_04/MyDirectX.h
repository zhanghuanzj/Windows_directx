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
using namespace std;

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameOver;

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
D3DXVECTOR2 GetBitmapSize(string fileName);  
void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 src);
void DrawSpriteFrame2Texture(LPDIRECT3DTEXTURE9 texture,int destx,int dexty,int framenum,int frameW,int frameH,int columns);
void UpdateNextFrameNumber(int &frameNumber,int startFrame,int endFrame,int direction,int &startTime,int delay);


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

//Game functions
bool Game_Init(HWND hwnd);
void Game_Run(HWND hwnd);
void Game_End();

#endif // !MYDIRECTX_H
