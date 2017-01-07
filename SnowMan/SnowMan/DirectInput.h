#pragma once
#include <dinput.h>
#include "Direct3D.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
class DirectInput
{
public:
	DirectInput()
	{
		ZeroMemory(keys,sizeof(char)*256);
		ZeroMemory(&mouseState,sizeof(mouseState));
	}

	HRESULT init(HWND hWnd, HINSTANCE hInstance, DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
	{
		DirectInput8Create(hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&directInput, NULL);

		//keyboard init
		directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
		keyboard->SetCooperativeLevel(hWnd, keyboardCoopFlags);
		keyboard->SetDataFormat(&c_dfDIKeyboard);
		keyboard->Acquire();
		keyboard->Poll();

		//进行鼠标设备的初始化
		directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
		mouse->SetCooperativeLevel(hWnd, mouseCoopFlags);
		mouse->SetDataFormat(&c_dfDIMouse);
		mouse->Acquire();
		keyboard->Poll();

		return S_OK;
	}

	void get_input()
	{
		HRESULT hr = keyboard->GetDeviceState(sizeof(keys), (void**)&keys);
		if (hr)
		{
			keyboard->Acquire();
			keyboard->GetDeviceState(sizeof(keys), (LPVOID)keys);
		}

		hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&mouseState);
		if (hr)
		{
			mouse->Acquire();
			mouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&mouseState);
		}
	}

	bool is_key_down(int iKey)
	{
		if (keys[iKey] & 0x80)
			return true;
		else
			return false;
	}

	bool is_mouseButtion_down(int button)
	{
		return (mouseState.rgbButtons[button] & 0x80) != 0;
	}

	float mouseDX()
	{
		return (float)mouseState.lX;
	}

	float mouseDY()
	{
		return (float)mouseState.lY;
	}

	float mouseDZ()
	{
		return (float)mouseState.lZ;
	}

	~DirectInput(void)
	{
		if (keyboard != nullptr)
			keyboard->Unacquire();
		if (mouse != nullptr)
			mouse->Unacquire();
		SAFE_RELEASE(keyboard);
		SAFE_RELEASE(mouse);
		SAFE_RELEASE(directInput);
	}
private:
	LPDIRECTINPUT8			directInput = nullptr;   
	LPDIRECTINPUTDEVICE8	keyboard = nullptr;  
	char					keys[256]; 
	LPDIRECTINPUTDEVICE8	mouse = nullptr;  
	DIMOUSESTATE			mouseState;
};