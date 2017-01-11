#ifndef DIRECT3D_H_
#define DIRECT3D_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <iostream>
#include "SnowMan.h"
#include "Cube.h"
#include "Terrian.h"
#include "Camera.h"
#include "SkyBox.h"
#include "SnowParticle.h"
#include "DirectInput.h"


using namespace std;

class DirectX
{
public:
	//������ȡ
	static DirectX& instance()
	{
		static DirectX* pDirectX = new DirectX();
		return *pDirectX;
	}
	//��ʼ��DirectX
	bool initialDirectX(HINSTANCE hInstance, HWND hWnd, int width, int height);
	void update(float time);
	void snowmanRender();
	//����
	~DirectX();

private:
	DirectX() :pD3DXDevice(nullptr) {}

	IDirect3DDevice9* pD3DXDevice;

	int width_;
	int height_;
	LPD3DXFONT font;
	RECT formatRect;
	
	SnowMan *snowMan1 = nullptr;
	SnowMan *snowMan2 = nullptr;
	Cube *cube = nullptr;
	Terrian *terrian = nullptr;
	SkyBox *skyBox = nullptr;
	Camera *pCamera = nullptr;
	SnowParticle *pSnowParticle = nullptr;
	DirectInput *pDirectInput = nullptr;
};


#endif