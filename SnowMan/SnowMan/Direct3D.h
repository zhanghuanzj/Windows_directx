#ifndef DIRECT3D_H_
#define DIRECT3D_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <iostream>
#include "SnowMan.h"
#include "Cube.h"


using namespace std;

class DirectX
{
public:
	//单例获取
	static DirectX& instance()
	{
		static DirectX* pDirectX = new DirectX();
		return *pDirectX;
	}
	//初始化DirectX
	bool initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height);
	void transformSetting();
	void snowmanRender();
	//析构
	~DirectX();

private:
	DirectX() :pD3DXDevice(nullptr) {}

	IDirect3DDevice9* pD3DXDevice;

	int width_;
	int height_;
	LPD3DXFONT font;
	RECT formatRect;
	LPD3DXMESH box;
	
	SnowMan *snowMan1 = nullptr;
	SnowMan *snowMan2 = nullptr;
	Cube *cube = nullptr;
};


#endif