#include "MyDirectX.h"

//Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPD3DXSPRITE spriteobj = NULL;

//DirectInput objects
LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyBoard;
DIMOUSESTATE mouse_state;
XINPUT_GAMEPAD controllers[4];
char keys[256];

//Direct3D initialization
bool Direct3D_Init(HWND hwnd,int width,int height,bool fullScreen)
{
	//设备接口初始化
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d==NULL)
	{
		MessageBox(hwnd,"Error initializing Direct3D","Error",MB_OK);
		return false;
	}
	
	//Direct3D表示方式参数设定
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = !fullScreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow = hwnd;
	//创建Direct3D设备接口
	d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
	if (d3ddev==NULL)
	{
		MessageBox(hwnd,"Error creating Direct3D device","Error",MB_OK);
		return false;
	}

	//获取backbuffer&surface
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);

	//精灵初始化
	D3DXCreateSprite(d3ddev,&spriteobj);
	
	return true;
}

//Direct shutDown
void Direct3D_ShutDown()
{
	if (d3ddev)
	{
		d3ddev->Release();
		d3ddev=NULL;
	}
	if (d3d)
	{
		d3d->Release();
		d3d=NULL;
	}
	if (spriteobj)
	{
		spriteobj->Release();
		spriteobj = NULL;
	}
}

//Draw surface to the backbuffer
void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 src)
{
	D3DSURFACE_DESC desc;
	src->GetDesc(&desc);

	RECT src_rect = {0,0,(long)desc.Width,(long)desc.Height};
	RECT dest_rect = {(long)x,(long)y,(long)x+desc.Width,(long)y+desc.Height};

	d3ddev->StretchRect(src,&src_rect,dest,&dest_rect,D3DTEXF_NONE);
}

//Draw the spriteframe to the texture
void DrawSpriteFrame2Texture(LPDIRECT3DTEXTURE9 texture,int destx,int dexty,int framenum,int frameW,int frameH,int columns)
{
	D3DXVECTOR3 position((float)destx,(float)dexty,0);
	D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);
	RECT rect;
	rect.left = (framenum%columns)*frameW;
	rect.top = (framenum/columns)*frameH;
	rect.right = rect.left+frameW;
	rect.bottom = rect.top+frameH;
	spriteobj->Draw(texture,&rect,NULL,&position,white);
}

//Update the next frame of the sprite
void UpdateNextFrameNumber(int &frameNumber,int startFrame,int endFrame,int direction,int &startTime,int delay)
{
	if ((int)GetTickCount()>startTime+delay)
	{
		startTime = GetTickCount();
		frameNumber += direction;
		if (frameNumber>endFrame)
		{
			frameNumber = startFrame;
		}
		if (frameNumber<startFrame)
		{
			frameNumber = endFrame;
		}
	}
}

//Load an image to the surface and return it
LPDIRECT3DSURFACE9 LoadSurface(string fileName)
{
	LPDIRECT3DSURFACE9 image = NULL;

	//get the image information
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(fileName.c_str(),&info)                    ;
	if (result!=D3D_OK)
	{
		return NULL;
	}

	//Create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);
	if (result!=D3D_OK)
	{
		return NULL;
	}

	//Load the image to the surface
	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		fileName.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0,0,0),
		NULL);
	if (result!=D3D_OK)
	{
		return NULL;
	}

	return image;
}

//Load an image to the texture and return it
LPDIRECT3DTEXTURE9 LoadTexture(string fileName,D3DCOLOR transColor)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(fileName.c_str(),&info);
	if (result!=D3D_OK)
	{
		return NULL;
	}
	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx( 
		d3ddev,                //Direct3D device object
		fileName.c_str(),      //bitmap filename
		info.Width,            //bitmap image width
		info.Height,           //bitmap image height
		1,                     //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,       //the type of surface (standard)
		D3DFMT_UNKNOWN,        //surface format (default)
		D3DPOOL_DEFAULT,       //memory class for the texture
		D3DX_DEFAULT,          //image filter
		D3DX_DEFAULT,          //mip filter
		transColor,            //color key for transparency
		&info,                 //bitmap file info (from loaded file)
		NULL,                  //color palette
		&texture );            //destination texture

	//make sure the bitmap textre was loaded correctly
	if (result != D3D_OK) return NULL;

	return texture;

}

D3DXVECTOR2 GetBitmapSize(string fileName)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f,0.0f);
	HRESULT result = D3DXGetImageInfoFromFile(fileName.c_str(),&info);
	if (result!=D3D_OK)
	{
		size = D3DXVECTOR2((float)info.Width,(float)info.Height);
	}
	else
	{
		size = D3DXVECTOR2((float)info.Width,(float)info.Height);
	}
	return size;
}

//DirectInput initialization
bool DirectInput_Init(HWND hwnd)
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
	d3ddev->ShowCursor(false);

	return true;
}

//DirectInput update
void DirectInput_Update()
{
	dimouse->GetDeviceState(sizeof(mouse_state),(LPVOID)&mouse_state);
	dikeyBoard->GetDeviceState(sizeof(keys),(LPVOID)&keys);

	for (int i=0;i<4;i++)
	{
		ZeroMemory(&controllers[i],sizeof(XINPUT_STATE));
		XINPUT_STATE state;
		DWORD result = XInputGetState(i,&state);
		if (result==0)
		{
			controllers[i] = state.Gamepad;
		}
	}
}

// Return mouse x movement
int Mouse_X()
{
	return mouse_state.lX;
}

// Return mouse y movement
int Mouse_Y()
{
	return mouse_state.lY;
}

// Return mouse button state
int Mouse_Button(int button)
{
	return mouse_state.rgbButtons[button] & 0x80;
}

// Return key press state
int Key_Down(int key)
{
	return (keys[key] & 0x80);
}

// DirectInput shutdown
void DirectInput_ShutDown()
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
}

// Returns true if controller is plugged in
bool XInput_Controller_Found()
{
	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
	if (caps.Type != 0) return false;

	return true;
}

// Vibrates the controller
void XInput_Vibrate(int contNum, int amount)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	vibration.wLeftMotorSpeed = amount;
	vibration.wRightMotorSpeed = amount; 
	XInputSetState( contNum, &vibration );
}