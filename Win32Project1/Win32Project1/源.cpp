#pragma once

//win32
//#pragma comment(lib,"comctl32.lib")
//
////d3d
//#pragma comment(lib,"dxerr.lib")
//#pragma comment(lib,"d3dx9.lib")
//
////dxut
//#pragma comment(lib,"dxut.lib")

//dxut

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"

HRESULT CALLBACK OnDeviceCreate(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext);

void    CALLBACK OnDeviceDestroyed(
	void* pUserContext);

void    CALLBACK OnFrameRender(
	IDirect3DDevice9* pd3dDevice,
	double fTime,
	float fElapsedTime,
	void* pUserContext);

void SetUpMatrix(
	IDirect3DDevice9* pd3dDevice);

void SetupLight(
	IDirect3DDevice9* pd3dDevice);

//TODO: global variables.
ID3DXMesh* g_pd3dMesh = NULL;

INT WINAPI wWinMain(
	HINSTANCE,
	HINSTANCE,
	LPWSTR,
	INT)
{
	DXUTSetCallbackD3D9DeviceCreated(
		OnDeviceCreate,
		NULL);

	DXUTSetCallbackD3D9DeviceDestroyed(
		OnDeviceDestroyed,
		NULL);

	DXUTSetCallbackD3D9FrameRender(
		OnFrameRender,
		NULL);

	//0.dxut init.
	DXUTInit(
		true,
		true,
		NULL,
		true);

	//1.create window
	DXUTCreateWindow(
		L"dxut window",
		GetModuleHandle(NULL),
		NULL,
		NULL,
		CW_USEDEFAULT,
		CW_USEDEFAULT);

	//2.create device.
	DXUTCreateDevice(
		true,
		640,
		480);

	//3.main loop
	DXUTMainLoop();

	//exit.
	return DXUTGetExitCode();
}

HRESULT CALLBACK OnDeviceCreate(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	//TODO: create d3d device.
	D3DXCreateTeapot(pd3dDevice, &g_pd3dMesh, NULL);

	return S_OK;
}

void    CALLBACK OnDeviceDestroyed(
	void* pUserContext)
{
	//TODO:destroy d3d device.
	if (g_pd3dMesh != NULL)
	{
		g_pd3dMesh->Release();
		g_pd3dMesh = NULL;
	}
}

void    CALLBACK OnFrameRender(
	IDirect3DDevice9* pd3dDevice,
	double fTime,
	float fElapsedTime,
	void* pUserContext)
{
	pd3dDevice->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255),
		1.0f,
		0);

	SetUpMatrix(pd3dDevice);

	SetupLight(pd3dDevice);

	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{
		//TODO:render d3d scene.
		g_pd3dMesh->DrawSubset(0);

		pd3dDevice->EndScene();
	}

	pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void SetUpMatrix(
	IDirect3DDevice9* pd3dDevice)
{
	D3DXMATRIXA16 viewMatrix;
	D3DXVECTOR3 vEye(0.0f, 5.0f, -3.0f);
	D3DXVECTOR3 vLookAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &vEye, &vLookAt, &vUp);
	pd3dDevice->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMATRIXA16 projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void SetupLight(
	IDirect3DDevice9* pd3dDevice)
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient.r = mtrl.Diffuse.r = 1.0f;
	mtrl.Ambient.g = mtrl.Diffuse.g = 1.0f;
	mtrl.Ambient.b = mtrl.Diffuse.b = 0.0f;
	mtrl.Ambient.a = mtrl.Diffuse.a = 1.0f;
	pd3dDevice->SetMaterial(&mtrl);

	D3DLIGHT9 light;
	D3DXVECTOR3 dir(1.0f, 0.0f, 0.0f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 0.0f);
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = color*0.6f;
	light.Diffuse = color;
	light.Specular = color*0.6f;
	light.Direction = dir;
	pd3dDevice->SetLight(0, &light);
	pd3dDevice->LightEnable(0, TRUE);

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
}