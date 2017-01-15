#pragma once
#include "Direct3D.h"
#include "Vertex.h"
class SkyBox
{
private:
	LPDIRECT3DDEVICE9			pdev;
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer = nullptr;
	LPDIRECT3DTEXTURE9			texture[5];
	float						length;

public:
	SkyBox(LPDIRECT3DDEVICE9  pDevice,float Length):pdev (pDevice),length(Length)
	{
		for (int i = 0; i<5; i++)
			texture[i] = nullptr;

		//1.create vertex buffer
		pdev->CreateVertexBuffer(20 * sizeof(Vertex), 0, FVF_VERTEX, D3DPOOL_MANAGED, &vertexBuffer, 0);

		Vertex vertices[] =
		{
			//front
			{ -length / 2, 0.0f,    length / 2, 0.0f, 1.0f, },
			{ -length / 2, length / 2,   length / 2, 0.0f, 0.0f, },
			{ length / 2, 0.0f,    length / 2, 1.0f, 1.0f, },
			{ length / 2, length / 2,   length / 2, 1.0f, 0.0f, },

			//back
			{ length / 2, 0.0f,   -length / 2, 0.0f, 1.0f, },
			{ length / 2, length / 2,  -length / 2, 0.0f, 0.0f, },
			{ -length / 2, 0.0f,   -length / 2, 1.0f, 1.0f, },
			{ -length / 2, length / 2,  -length / 2, 1.0f, 0.0f, },

			//left
			{ -length / 2, 0.0f,   -length / 2, 0.0f, 1.0f, },
			{ -length / 2, length / 2,  -length / 2, 0.0f, 0.0f, },
			{ -length / 2, 0.0f,    length / 2, 1.0f, 1.0f, },
			{ -length / 2, length / 2,   length / 2, 1.0f, 0.0f, },

			//right
			{ length / 2, 0.0f,   length / 2, 0.0f, 1.0f, },
			{ length / 2, length / 2,  length / 2, 0.0f, 0.0f, },
			{ length / 2, 0.0f,  -length / 2, 1.0f, 1.0f, },
			{ length / 2, length / 2, -length / 2, 1.0f, 0.0f, },

			//top
			{ length / 2, length / 2, -length / 2, 1.0f, 0.0f, },
			{ length / 2, length / 2,  length / 2, 1.0f, 1.0f, },
			{ -length / 2, length / 2, -length / 2, 0.0f, 0.0f, },
			{ -length / 2, length / 2,  length / 2, 0.0f, 1.0f, },

		};

		void* pVertices;
		//2.lock
		vertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
		//3.fill
		memcpy(pVertices, vertices, sizeof(vertices));
		//4.unlock
		vertexBuffer->Unlock();
	}

	~SkyBox(void)
	{
		SAFE_RELEASE(vertexBuffer);
		for (int i = 0; i<5; i++)
		{
			SAFE_RELEASE(texture[i]);
		}
	}

public:
	bool loadSkyTextureFromFile(const string &pFrontTextureFile, const string &pBackTextureFile, const string &pLeftTextureFile, const string &pRightTextureFile, const string &pTopTextureFile)
	{
		D3DXCreateTextureFromFile(pdev, pFrontTextureFile.c_str(), &texture[0]);  
		D3DXCreateTextureFromFile(pdev, pBackTextureFile.c_str(), &texture[1]);
		D3DXCreateTextureFromFile(pdev, pLeftTextureFile.c_str(), &texture[2]);
		D3DXCreateTextureFromFile(pdev, pRightTextureFile.c_str(), &texture[3]);
		D3DXCreateTextureFromFile(pdev, pTopTextureFile.c_str(), &texture[4]);
		return true;
	}
	void renderSkyBox(LPD3DXEFFECT pEffect = nullptr)
	{
		D3DXMATRIX worldMatrix;
		pdev->GetTransform(D3DTS_WORLD, &worldMatrix);
		if (pEffect != nullptr)
		{
			pEffect->SetTexture(TEXTURE, Texture::Instance(pdev).get_box());
			pEffect->SetMatrix(WORLD_MATRIX, &worldMatrix);
			pEffect->SetVector(MATERIAL, new D3DXVECTOR4(1, 1, 1,1));
			UINT iPass, cPasses;
			pEffect->Begin(&cPasses, 0);
			for (iPass = 0; iPass < cPasses; iPass++)
			{
				pEffect->BeginPass(iPass);
				pdev->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
				pdev->SetFVF(FVF_VERTEX);

				for (int i = 0; i<5; i++)
				{
					pdev->SetTexture(0, texture[i]);
					pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
				}
				pEffect->EndPass();
			}
			pEffect->End();
		}
		else
		{
			pdev->SetTransform(D3DTS_WORLD, &worldMatrix);
			pdev->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
			pdev->SetFVF(FVF_VERTEX);

			for (int i = 0; i<5; i++)
			{
				pdev->SetTexture(0, texture[i]);
				pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
			}
		}
		
	}

};