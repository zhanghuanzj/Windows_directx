//-----------------------------------------------------------------------------
// �ļ���EmptyProject.cpp
// ���ܣ�D3D ��ѧ����
// ��� 
// �汾��1.0.0.0
// ����޶����ڣ�2011��10��30��
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

//-----------------------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // D3D �ӿ�
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // �豸�ӿ�
LPD3DXMESH              g_pMesh = NULL; // ģ�ͽӿ�
LPDIRECT3DTEXTURE9      g_pMeshTextures = NULL; // ����ӿ�
LPD3DXEFFECT            g_pEffect = NULL; // Ч���ӿ�
										  //-----------------------------------------------------------------------------
										  // ������InitD3D()
										  // ���ܣ���ʼ�� D3D �� 3D �豸
										  // ��������ʼ��������ȫ�ֱ��� g_pD3D g_pd3dDevice
										  //-----------------------------------------------------------------------------

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v) :pos(x, y, z), norm(x, y, z), tu(u), tv(v) {}
	D3DXVECTOR3 pos;     // Vertex position
	D3DXVECTOR3 norm;    // Vertex normal
	float tu;            // Texture coordinate u
	float tv;            // Texture coordinate v
};

#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1

struct PointVertex
{
	float x, y, z;
	float u, v;
};
#define FVF_PVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)
LPD3DXMESH texMesh;
LPD3DXMESH mesh;
HRESULT InitD3D(HWND hWnd)
{
	// ���� 3D ����.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))       return E_FAIL;
	// ����ϵͳ������Ҫ�����Ĳ������������ D3DPRESENT_PARAMETERS �Ĳ���
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	// �����豸
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))  return E_FAIL;


	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(250, 250, 250));  
	g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.7f, 1.0f);
	mtrl.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	mtrl.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	mtrl.Emissive = D3DXCOLOR(0.3f, 0.0f, 0.1f, 1.0f);
	g_pd3dDevice->SetMaterial(&mtrl);

	D3DXCreateSphere(g_pd3dDevice, 7, 10, 10, &mesh, nullptr);
	// Get a copy of the sphere mesh
	mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, g_pd3dDevice, &texMesh);
	// Release original mesh
	mesh->Release();
	// add texture coordinates
	Vertex* pVerts;
	if (SUCCEEDED(texMesh->LockVertexBuffer(0, (void**)&pVerts)))
	{
		// Get vertex count
		int numVerts = texMesh->GetNumVertices();
		for (int i = 0; i < numVerts; ++i)
		{
			// Calculates texture coordinates
			pVerts->tu = asinf(pVerts->norm.x) / D3DX_PI + 0.5f; //(-1, 1)-->(0, 1)
			pVerts->tv = asinf(pVerts->norm.y) / D3DX_PI + 0.5f;
			++pVerts;
		}
		// Unlock the vertex buffer
		texMesh->UnlockVertexBuffer();
	}
	return S_OK;
}

//-----------------------------------------------------------------------------
// ������InitGeometry()
// ���ܣ�����Ŀ¼��װ��ģ�͡�����Ч���ļ�������Ч��������HLSL
// ��������ʼ������������ȫ�ֱ��� g_pMesh g_pEffect g_pMeshTextures
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	HRESULT hr;
	// ����Ŀ¼
	TCHAR szAppPath[MAX_PATH];
	TCHAR szRcPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szAppPath);
	// 
	lstrcpy(szRcPath, szAppPath);
	lstrcat(szRcPath, L"\\ResFile_1_Model");
	SetCurrentDirectory(szRcPath);
	// װ��ģ��
	if (FAILED(hr = D3DXLoadMeshFromX(L"Model.x", D3DXMESH_MANAGED,
		g_pd3dDevice, NULL, NULL, NULL, NULL, &g_pMesh))) return hr;
	// װ��Ч���ļ�
	LPD3DXBUFFER pCode = NULL;
	if (FAILED(hr = D3DXCreateEffectFromFile(g_pd3dDevice, L"FileFX.fx",
		NULL, NULL, 0, NULL, &g_pEffect, &pCode)))
	{
		LPVOID pBufErr = pCode->GetBufferPointer();
		char* ErrInfo = (char*)pBufErr;
		pCode->Release();
		return hr;
	}
	// װ������ע��ֻװ��һ������
	if (FAILED(hr = D3DXCreateTextureFromFileEx(g_pd3dDevice, L"ColorMap.jpg",
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pMeshTextures))) return hr;
	// ָ��Ч���ļ�
	if (FAILED(hr = g_pEffect->SetTechnique("DefaultTech"))) return hr;
//	if (FAILED(hr = g_pEffect->SetTexture("g_ColorTexture", g_pMeshTextures))) return hr;
	// ������ȷ������
	return S_OK;
}

//-----------------------------------------------------------------------------
// ������Cleanup()
// ���ܣ��ͷ����еĽӿ�
// ��������
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (NULL != g_pMeshTextures)   g_pMeshTextures->Release();
	if (NULL != g_pMesh)           g_pMesh->Release();
	if (NULL != g_pd3dDevice)      g_pd3dDevice->Release();
	if (NULL != g_pD3D)            g_pD3D->Release();
	if (NULL != g_pEffect)         g_pEffect->Release();
}

//-----------------------------------------------------------------------------
// ������SetupMatrices()
// ���ܣ��� 4 ���������ݸ�Ч���ļ�����Ч���ļ�������
// �������ޣ���ģ���漰ȫ�ֱ�����Ч���ļ��ӿ�
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
	HRESULT hr;
	D3DXMATRIXA16 matWorld;
	float fTime = timeGetTime() / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fTime);
	hr = g_pEffect->SetFloat("g_fTime", fTime);
	// �������
	hr = g_pEffect->SetMatrix("g_matWorld", &matWorld);
	// �۲����
	D3DXVECTOR3 vEyePt(0.0f, 7.0f, -20.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	hr = g_pEffect->SetMatrix("g_matView", &matView);
	// ͶӰ����
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 64.0 / 41.0f, 1.0, 100.0f);
	hr = g_pEffect->SetMatrix("g_matProject", &matProj);
	g_pEffect->SetVector("color",new D3DXVECTOR4(1,1,0,1));
}

//-----------------------------------------------------------------------------
// ������Render()
// ���ܣ���Ⱦģ��
// �������ޣ�ʹ��ȫ�ֱ���
//-----------------------------------------------------------------------------
VOID Render()
{
	HRESULT hr;
	// �����������������Ȼ�����
	hr = g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DXCOLOR(0.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0);
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// ���ñ任��ʵ����������ʵ�ʾ����� Y ����ת
		SetupMatrices();
		// ��Ч���ļ���������Ⱦ
		UINT iPass, cPasses;
		hr = g_pEffect->Begin(&cPasses, 0);
		for (iPass = 0; iPass < cPasses; iPass++)
		{  
			hr = g_pEffect->BeginPass(iPass);
			hr = g_pMesh->DrawSubset(0);
			texMesh->DrawSubset(0);
			hr = g_pEffect->EndPass();
		}
		hr = g_pEffect->End();
		hr = g_pd3dDevice->EndScene();
	}
	// ����Ⱦ�Ľ���л���ǰ��������
	hr = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
// ������MsgProc()
// ���ܣ���Ϣѭ���Ĵ���
// �������ޣ��� Window ���ڵ�һ����
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// ������WinMain()
// ���ܣ�Ӧ�ó�����ڵ�
// ������ʵ������������в���
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// �����´�����ʹ��ڱ���
#define CLASSNAME L"EmptyProject"
#define TITILNAME L"EmptyProject_2"
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , MsgProc , 0L , 0L ,
		GetModuleHandle(NULL) , NULL , LoadCursor(NULL, IDC_ARROW) , NULL , NULL ,
		CLASSNAME , NULL };
	RegisterClassEx(&wc);
	// ��������
	HWND hWnd = CreateWindow(CLASSNAME, TITILNAME, WS_OVERLAPPEDWINDOW, 100, 100, 640, 410,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);
	// ��ʼ�� D3D �� 3D �豸
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// װ��ģ��
		if (SUCCEEDED(InitGeometry()))
		{
			// ��ʾ����
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);
			// ������Ϣѭ��
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}
	UnregisterClass(CLASSNAME, wc.hInstance);
	return 0;
}



