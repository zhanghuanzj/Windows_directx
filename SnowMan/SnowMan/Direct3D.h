#ifndef DIRECT3D_H_
#define DIRECT3D_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include "CommonUtil.h"
#include "SnowMan.h"
#include "Cube.h"
#include "Terrian.h"
#include "Camera.h"
#include "SkyBox.h"
#include "SnowParticle.h"
#include "DirectInput.h"
#include "Model.h"


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
	void update(float time);
	void snowmanRender();
	void treeRender(D3DXVECTOR3 pos);

	void light_on();
	void light_off();
	//析构
	~DirectX();

private:
	DirectX() :pD3DXDevice(nullptr),objects(5) {}

	IDirect3DDevice9* pD3DXDevice;

	int width_;
	int height_;
	LPD3DXFONT font;
	RECT formatRect;
	
	vector<Object> objects;
	SnowMan *snowMan1 = nullptr;
	SnowMan *snowMan2 = nullptr;
	Cube *cube = nullptr;
	Terrian *terrian = nullptr;
	SkyBox *skyBox = nullptr;
	Camera *pCamera = nullptr;
	SnowParticle *pSnowParticle = nullptr;
	DirectInput *pDirectInput = nullptr;
	Model *pTree = nullptr;
	Model *pTree1 = nullptr;
	Model *pHouse = nullptr;

	Camera *pLCamera = nullptr;
	static int n;
	static D3DXVECTOR4 LightDiffuse;
	static D3DXVECTOR4 LightAmbient;
};


#endif