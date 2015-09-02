#include <windows.h>
#include <iostream>
#include <time.h>
using namespace std;

const string ProgramTitle = "Hello Windows";
bool gameOver = false;
HWND hwnd;
HDC device;

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
	MessageBox(NULL,"hello","jack",MB_OK);
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
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		NULL, 
		hInstance,
		NULL); 

	if (hwnd==0) return false;

	//display the window
	ShowWindow (hwnd, iCmdShow);
	UpdateWindow (hwnd);

	device = GetDC(hwnd);
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
		PostQuitMessage (0);
		break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);

}

void DrawBitmap(char *filename,int x,int y)
{
	//绘图四步曲
	//获取图像句柄
	HBITMAP hImage = (HBITMAP)LoadImage(0,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	BITMAP bm;
	GetObject(hImage,sizeof(BITMAP),&bm);
	//建立兼容图像缓存内存
	HDC hdcImage = CreateCompatibleDC(device);
	//将图片与缓存相关联
	SelectObject(hdcImage,hImage);    
	//将缓存图片贴到真正DC上
	BitBlt(
		device,                      //目标DC
		x,y,bm.bmWidth,bm.bmHeight,  //贴图起始及大小
		hdcImage,0,0,                //原设备及取图位置
		SRCCOPY);                    //贴图方式
	Sleep(1000);
	UpdateWindow(hwnd);
	//delete the object
	DeleteDC(hdcImage);
	DeleteObject(hImage);
}
bool Game_Init()
{
	srand(time(nullptr));
	return true;
}

void Game_Run()
{
	if (gameOver)
	{
		return;
	}
	RECT rect;
	GetClientRect(hwnd,&rect);
	int x = rand()%(rect.right-rect.left);
	int y = rand()%(rect.bottom-rect.top);
	DrawBitmap("heart.bmp",x,y);
}

void Game_End()
{
	ReleaseDC(hwnd,device);
}
