#include "Direct3D.h"

bool DirectX::initialDirectX(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	//1.创建接口
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	//2.获取硬件信息，确定顶点处理方式
	D3DCAPS9 caps;
	int vertexProcessing;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);    //(当前显卡，硬件设备)
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.填写D3D参数
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4.创建设备
	d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vertexProcessing, &d3dpp, &pD3DXDevice);
	if (pD3DXDevice == nullptr)
		cout << "ERROR" << endl;
	d3d9->Release();

	//保存屏幕宽、高、Z缓冲区大小
	width_ = width;
	height_ = height;

	//Font setting
	font = nullptr;
	D3DXCreateFont(pD3DXDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, "微软雅黑", &font);
	GetClientRect(hwnd, &formatRect);

	snowMan = new SnowMan(pD3DXDevice);
	//D3DXCreateBox(pD3DXDevice, 2, 2, 2, &box, NULL);

	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mtrl.Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	mtrl.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	//mtrl.Emissive = D3DXCOLOR(0.3f, 0.0f, 0.1f, 1.0f);
	pD3DXDevice->SetMaterial(&mtrl);

	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	pD3DXDevice->SetLight(0, &light);
	pD3DXDevice->LightEnable(0, true);
	// 设置渲染状态
	//pD3DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pD3DXDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pD3DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pD3DXDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	pD3DXDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));   //设置一下环境光


	return true;
}

void DirectX::transformSetting()
{
	//1.set view transform
	D3DXMATRIX matView; 
	D3DXVECTOR3 vEye(0.0f, 0.0f, -15.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f); 
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp); 
	pD3DXDevice->SetTransform(D3DTS_VIEW, &matView); 

	//2.set project transform
	D3DXMATRIX matProj; //定义一个矩阵
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f); //计算投影变换矩阵
	pD3DXDevice->SetTransform(D3DTS_PROJECTION, &matProj);  //设置投影变换矩阵

	//3.set viewport transform
	D3DVIEWPORT9 vp; 
	vp.X = 0;		
	vp.Y = 0;		
	vp.Width = width_;	
	vp.Height = height_; 
	vp.MinZ = 0.0f; 
	vp.MaxZ = 1.0f;	
	pD3DXDevice->SetViewport(&vp); 
}

void DirectX::snowmanRender()
{
	pD3DXDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	pD3DXDevice->BeginScene();

	transformSetting();
	formatRect.top = 100;
	font->DrawText(0, "【致我们永不熄灭的游戏开发梦想】", -1, &formatRect, DT_CENTER, D3DCOLOR_XRGB(68, 139, 256));
	//static float n = 1;
	//float angle = n*0.03f;
	//D3DXMATRIX rotate;
	//D3DXMatrixRotationY(&rotate,angle);
	//++n;
	//if (angle >= 360) n = 0;
	//D3DXMATRIX d;
	//D3DXMatrixTranslation(&d, 3.0f, -3.0f, 0.0f);
	//pD3DXDevice->SetTransform(D3DTS_WORLD, &rotate);
	snowMan->draw_snowMan();

	pD3DXDevice->EndScene();
	pD3DXDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
DirectX::~DirectX()
{
	if (pD3DXDevice != nullptr)
	{
		pD3DXDevice->Release();
	}
	if (box != nullptr)
	{
		box->Release();
	}
	if (font != nullptr)
	{
		font->Release();
	}
}