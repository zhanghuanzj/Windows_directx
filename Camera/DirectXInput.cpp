#include "DirecXInput.h"

MyDirectXInput::MyDirectXInput()
{
	dinput = nullptr;
	dimouse = nullptr;
	dikeyBoard = nullptr;
	ZeroMemory(&mouse_state,sizeof(mouse_state));
	ZeroMemory(keys,sizeof(char)*256);
}

MyDirectXInput::~MyDirectXInput()
{
	if (dikeyBoard) 
	{
		dikeyBoard->Unacquire();
		dikeyBoard->Release();
		dikeyBoard = NULL;
	}
	if (dimouse) 
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
	SAFE_RELEASE(dinput);

}

//DirectInput initialization
bool MyDirectXInput::DirectInput_Init(HWND hwnd)
{
	//initialize the DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		NULL);

	//initialize the keyboard
	dinput->CreateDevice(GUID_SysKeyboard,&dikeyBoard,NULL);
	dikeyBoard->SetDataFormat(&c_dfDIKeyboard);
	dikeyBoard->SetCooperativeLevel(hwnd,DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	dikeyBoard->Acquire();

	//initialize the mouse
	dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	dimouse->SetDataFormat(&c_dfDIMouse);
	dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dimouse->Acquire();


	return true;
}

//DirectInput update
void MyDirectXInput::DirectInput_UpdateState()
{
	dimouse->GetDeviceState(sizeof(mouse_state),(LPVOID)&mouse_state);
	dikeyBoard->GetDeviceState(sizeof(keys),(LPVOID)&keys);
}

// Return mouse x movement
int MyDirectXInput::Mouse_X()
{
	return mouse_state.lX;
}

// Return mouse y movement
int MyDirectXInput::Mouse_y()
{
	return mouse_state.lY;
}

// Return mouse button state
int MyDirectXInput::Mouse_Button(int button)
{
	return mouse_state.rgbButtons[button] & 0x80;
}

// Return key press state
int MyDirectXInput::Key_Down(int key)
{
	return (keys[key] & 0x80);
}
