#include <windows.h>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
using namespace std;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?true:false)  //判断某个键是否被按下

const string ProgramTitle = "Hello Windows";
bool gameOver = false;
HWND hwnd;
D3DDISPLAYMODE dm;
LPDIRECT3D9 d3d = NULL;            //Direct3d接口
LPDIRECT3DDEVICE9 d3ddev = NULL;   //设备接口
LPDIRECT3DSURFACE9 surface = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool RegisterMyWindowClass(HINSTANCE hInstance);
bool CreateMyWindowInstance(HINSTANCE hInstance,int iCmdShow);
bool Game_Init();
void Game_Run();
void Game_End();

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//register window and create one
	RegisterMyWindowClass(hInstance);
	if (!CreateMyWindowInstance(hInstance,iCmdShow))
	{
		return 0;
	}
	if (!Game_Init())
	{
		return 0;
	}
	
	//main message loop
	MSG          msg;
	while (!gameOver)  //get message
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		Game_Run();
	}
	Game_End();
	return msg.wParam;
}
bool RegisterMyWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX   wndclassex;
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = WndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = ProgramTitle.c_str();
	wndclassex.hIconSm       = wndclassex.hIcon;

	if (!RegisterClassEx (&wndclassex))
	{
		MessageBox (NULL, TEXT ("RegisterClassEx failed!"), ProgramTitle.c_str(), MB_ICONERROR);
		return false;
	}
	return true;
}
bool CreateMyWindowInstance(HINSTANCE hInstance,int iCmdShow)
{
	//create the window 
	hwnd = CreateWindowEx (
		WS_EX_OVERLAPPEDWINDOW, 
		ProgramTitle.c_str(), 
		"MyWindow",
		WS_EX_TOPMOST|WS_POPUP,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		640, 
		480, 
		NULL, 
		NULL, 
		hInstance,
		NULL); 

	if (hwnd==0) return false;

	//display the window
	ShowWindow (hwnd, iCmdShow);
	UpdateWindow (hwnd);

	
	return true;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

{
	HDC hdc;
	PAINTSTRUCT ps;
	const string text = "A window";

	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps);
		TextOut (hdc, 0, 0, text.c_str(), text.length() );
		EndPaint (hwnd, &ps);
		break;

	case WM_DESTROY:
		gameOver = true;
		PostQuitMessage (0);
		break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);

}

bool Game_Init()
{
	srand(time(NULL));
	//设备接口初始化
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d==NULL)
	{
		MessageBox(hwnd,"Error initializing Direct3D","Error",MB_OK);
		return false;
	}
	//获取自适应的显示模式
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dm);
	//Direct3D表示方式参数设定
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = dm.Width;
	d3dpp.BackBufferHeight = dm.Height;
	d3dpp.hDeviceWindow = hwnd;
	//创建Direct3D设备接口
	d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
	if (d3ddev==NULL)
	{
		MessageBox(hwnd,"Error creating Direct3D device","Error",MB_OK);
		return false;
	}

	//设备帧清除
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	//获取backbuffer&surface
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	HRESULT result = d3ddev->CreateOffscreenPlainSurface(
		dm.Width,
		dm.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&surface,
		NULL);
	if (result!=D3D_OK)
	{
		return false;
	}
	//Load the image to surface
	result = D3DXLoadSurfaceFromFile(
		surface,
		NULL,
		NULL,
		"bg.jpg",
		NULL,
		D3DX_DEFAULT,
		0,
		NULL);
	if (result!=D3D_OK)
	{
		return false;
	}
	return true;
}

void Game_Run()
{
	//确保设备接口可用
	if (d3ddev==NULL)
	{
		return;
	}

	
	if (d3ddev->BeginScene())
	{
		//Draw the image
		d3ddev->StretchRect(surface,NULL,backbuffer,NULL,D3DTEXF_NONE);

		d3ddev->EndScene();
		d3ddev->Present(NULL,NULL,NULL,NULL);
	}
	if (KEY_DOWN(VK_ESCAPE))
	{
		//ESC键被按下，发送消息给窗口
		PostMessage(hwnd,WM_DESTROY,0,0);
	}
}

void Game_End()
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
}
