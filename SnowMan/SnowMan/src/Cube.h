#pragma once
#include <d3d9.h>
#include <d3dx9.h>
//#include "CommonUtil.h"
#include "Vertex.h"
class Cube
{
public:
	Cube(LPDIRECT3DDEVICE9 dev)
	{
		pdev = dev;
		//创建顶端缓存  
		dev->CreateVertexBuffer(
			24 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX,
			D3DPOOL_MANAGED,
			&vb,
			0);

		Vertex* v;
		//数据锁定，写入  
		vb->Lock(0, 0, (void**)&v, 0);

		// 正面数据  
		v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f);
		v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f);
		v[2] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f);
		v[3] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f);

		//背面数据  
		v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
		v[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
		v[6] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		v[7] = Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f);

		// 上面数据  
		v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
		v[9] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
		v[10] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		v[11] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f);

		// 下面数据  
		v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f);
		v[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
		v[14] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		v[15] = Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f);

		// 左面数据  
		v[16] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
		v[17] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
		v[18] = Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f);
		v[19] = Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f);

		// 右面数据  
		v[20] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f);
		v[21] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f);
		v[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		v[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f);

		//解锁  
		vb->Unlock();

		//创建索引缓存  
		dev->CreateIndexBuffer(36 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&ib,
			0);

		WORD* i = 0;
		//锁定，数据写入  
		ib->Lock(0, 0, (void**)&i, 0);

		// 正面  
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// 背面  
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		//上面  
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// 下面  
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// 左面  
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		//右面  
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		//解锁  
		ib->Unlock();

		::ZeroMemory(&material, sizeof(material));
		material.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		material.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}

	void draw_cube(LPD3DXEFFECT pEffect = nullptr)
	{
		D3DXMATRIX worldMatrix;
		pdev->GetTransform(D3DTS_WORLD, &worldMatrix);
		if (pEffect!=nullptr)
		{
			pEffect->SetTexture(TEXTURE, Texture::Instance(pdev)->get_box());
			pEffect->SetMatrix(WORLD_MATRIX, &worldMatrix);
			pEffect->SetVector(MATERIAL, new D3DXVECTOR4(material.Diffuse.r, material.Diffuse.g, material.Diffuse.b,material.Diffuse.a));
			UINT iPass, cPasses;
			pEffect->Begin(&cPasses, 0);
			for (iPass = 0; iPass < cPasses; iPass++)
			{
				pEffect->BeginPass(iPass);
				pdev->SetStreamSource(0, vb, 0, sizeof(Vertex));
				pdev->SetIndices(ib);
				//设置灵活顶点格式  
				pdev->SetFVF(FVF_VERTEX);
				//绘制   
				pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,24,0,12);
				pEffect->EndPass();
			}
			pEffect->End();
		}
		else
		{
			pdev->SetTransform(D3DTS_WORLD, &worldMatrix);
			pdev->SetTexture(0, Texture::Instance(pdev)->get_box());
			//设置数据来源  
			pdev->SetStreamSource(0, vb, 0, sizeof(Vertex));
			pdev->SetIndices(ib);
			//设置灵活顶点格式  
			pdev->SetFVF(FVF_VERTEX);
			//绘制   
			pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
		}
		
	}
private:
	IDirect3DVertexBuffer9* vb;         //顶点缓存  
	IDirect3DIndexBuffer9*  ib;         //索引缓存  
	LPDIRECT3DDEVICE9 pdev;
	D3DMATERIAL9 material;
};
