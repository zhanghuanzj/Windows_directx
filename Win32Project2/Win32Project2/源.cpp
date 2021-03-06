//-----------------------------------------------------------------------------
// 文件：EmptyProject.cpp
// 功能：D3D 教学入门
// 李健波 
// 版本：1.0.0.0
// 最后修订日期：2011年10月30日
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

//-----------------------------------------------------------------------------
// 全局变量
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // D3D 接口
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // 设备接口
LPD3DXMESH              g_pMesh = NULL; // 模型接口
LPDIRECT3DTEXTURE9      g_pMeshTextures = NULL; // 纹理接口
LPD3DXEFFECT            g_pEffect = NULL; // 效果接口
										  //-----------------------------------------------------------------------------
										  // 函数：InitD3D()
										  // 功能：初始化 D3D 和 3D 设备
										  // 参数：初始化了两个全局变量 g_pD3D g_pd3dDevice
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
	// 创建 3D 对象.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))       return E_FAIL;
	// 告诉系统设置需要创建的参数，就是填充 D3DPRESENT_PARAMETERS 的参数
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	// 创建设备
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
// 函数：InitGeometry()
// 功能：设置目录、装入模型、纹理、效果文件、传递效果参数到HLSL
// 参数：初始化了其他三个全局变量 g_pMesh g_pEffect g_pMeshTextures
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	HRESULT hr;
	// 设置目录
	TCHAR szAppPath[MAX_PATH];
	TCHAR szRcPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szAppPath);
	// 
	lstrcpy(szRcPath, szAppPath);
	lstrcat(szRcPath, L"\\ResFile_1_Model");
	SetCurrentDirectory(szRcPath);
	// 装入模型
	if (FAILED(hr = D3DXLoadMeshFromX(L"Model.x", D3DXMESH_MANAGED,
		g_pd3dDevice, NULL, NULL, NULL, NULL, &g_pMesh))) return hr;
	// 装入效果文件
	LPD3DXBUFFER pCode = NULL;
	if (FAILED(hr = D3DXCreateEffectFromFile(g_pd3dDevice, L"FileFX.fx",
		NULL, NULL, 0, NULL, &g_pEffect, &pCode)))
	{
		LPVOID pBufErr = pCode->GetBufferPointer();
		char* ErrInfo = (char*)pBufErr;
		pCode->Release();
		return hr;
	}
	// 装入纹理，注意只装入一个纹理
	if (FAILED(hr = D3DXCreateTextureFromFileEx(g_pd3dDevice, L"ColorMap.jpg",
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pMeshTextures))) return hr;
	// 指定效果文件
	if (FAILED(hr = g_pEffect->SetTechnique("DefaultTech"))) return hr;
//	if (FAILED(hr = g_pEffect->SetTexture("g_ColorTexture", g_pMeshTextures))) return hr;
	// 可以正确返回了
	return S_OK;
}

//-----------------------------------------------------------------------------
// 函数：Cleanup()
// 功能：释放所有的接口
// 参数：无
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
// 函数：SetupMatrices()
// 功能：将 4 个变量传递给效果文件，由效果文件来计算
// 参数：无，本模块涉及全局变量：效果文件接口
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
	HRESULT hr;
	D3DXMATRIXA16 matWorld;
	float fTime = timeGetTime() / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fTime);
	hr = g_pEffect->SetFloat("g_fTime", fTime);
	// 世界矩阵
	hr = g_pEffect->SetMatrix("g_matWorld", &matWorld);
	// 观察矩阵
	D3DXVECTOR3 vEyePt(0.0f, 7.0f, -20.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	hr = g_pEffect->SetMatrix("g_matView", &matView);
	// 投影矩阵
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 64.0 / 41.0f, 1.0, 100.0f);
	hr = g_pEffect->SetMatrix("g_matProject", &matProj);
	g_pEffect->SetVector("color",new D3DXVECTOR4(1,1,0,1));
}

//-----------------------------------------------------------------------------
// 函数：Render()
// 功能：渲染模型
// 参数：无，使用全局变量
//-----------------------------------------------------------------------------
VOID Render()
{
	HRESULT hr;
	// 清除背景缓冲区的深度缓冲区
	hr = g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DXCOLOR(0.0f, 0.25f, 0.25f, 0.55f), 1.0f, 0);
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 设置变换，实际上是设置实际矩阵绕 Y 轴旋转
		SetupMatrices();
		// 按效果文件的设置渲染
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
	// 将渲染的结果切换到前景缓冲区
	hr = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------------------
// 函数：MsgProc()
// 功能：消息循环的处理
// 参数：无，是 Window 窗口的一部分
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
// 函数：WinMain()
// 功能：应用程序入口点
// 参数：实例句柄，命令行参数
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	// 定义下窗口类和窗口标题
#define CLASSNAME L"EmptyProject"
#define TITILNAME L"EmptyProject_2"
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , MsgProc , 0L , 0L ,
		GetModuleHandle(NULL) , NULL , LoadCursor(NULL, IDC_ARROW) , NULL , NULL ,
		CLASSNAME , NULL };
	RegisterClassEx(&wc);
	// 创建窗口
	HWND hWnd = CreateWindow(CLASSNAME, TITILNAME, WS_OVERLAPPEDWINDOW, 100, 100, 640, 410,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);
	// 初始化 D3D 和 3D 设备
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// 装载模型
		if (SUCCEEDED(InitGeometry()))
		{
			// 显示窗口
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);
			// 进入消息循环
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



