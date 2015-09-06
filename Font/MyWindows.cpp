#include "MyDirectX.h"

bool gameOver = false;
HWND hwnd;
string ProgramTitle = "MyWindowClass";

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
{
	switch (message)
	{
	case WM_DESTROY:
		gameOver = true;
		break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
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
	hwnd = CreateWindow (
		ProgramTitle.c_str(), 
		"MyWindow",
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		SCREENW, 
		SCREENH, 
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

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//register window and create one
	RegisterMyWindowClass(hInstance);
	if (!CreateMyWindowInstance(hInstance,iCmdShow))
	{
		return 0;
	}

	if (!Game_Init(hwnd))
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
		Game_Run(hwnd);
	}
	Game_End();
	return msg.wParam;
}
