#ifndef DIRECTXINPUT_H
#define DIRECTXINPUT_H

#define SAFE_RELEASE(device) {if(device){(device)->Release();(device)=nullptr;}}
#include <dinput.h>
#include <Xinput.h>
#include <windows.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

class MyDirectXInput
{
public:
	MyDirectXInput();
	~MyDirectXInput();
	//DirectInput functions
	bool DirectInput_Init(HWND hwnd);
	void DirectInput_UpdateState();
	//Input state check
	int Key_Down(int);
	int Mouse_Button(int);
	int Mouse_X();
	int Mouse_y();

private:
	LPDIRECTINPUT8 dinput;
    LPDIRECTINPUTDEVICE8 dimouse;
    LPDIRECTINPUTDEVICE8 dikeyBoard;
	DIMOUSESTATE mouse_state;
	char keys[256];
};




#endif // !DIRECTXINPUT_H
