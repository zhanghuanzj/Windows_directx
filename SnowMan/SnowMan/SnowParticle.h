#pragma once
#include "Direct3D.h"
#include "Vertex.h"
#include "CommonUtil.h"
#define  NUMBER		25000   
#define  LENGTH_X	600   
#define  WIDTH_Z	600   
#define  HEIGHT_Y   200  
struct  Particle
{
	float x, y, z;      
	float rotationY;         
	float rotationX;       
	float fallSpeedy;    
	float fallSpeedx;
	float fallSpeedz;
	float rotationSpeed;     
	int   textureIndex;   
};

class SnowParticle
{
private:
	LPDIRECT3DDEVICE9				pdev;			
	Particle						snows[NUMBER];    
	LPDIRECT3DVERTEXBUFFER9			vertexBuffer = nullptr;
	LPDIRECT3DTEXTURE9				textures[6];  

public:
	SnowParticle(LPDIRECT3DDEVICE9 pd3dDevice):pdev(pd3dDevice)
	{
		for (int i = 0; i < 5; i++)
			textures[i] = nullptr;

		srand(GetTickCount());
		for (int i = 0; i < NUMBER; i++)
		{
			snows[i].x = float(rand() % LENGTH_X - LENGTH_X / 2);
			snows[i].z = float(rand() % WIDTH_Z - WIDTH_Z / 2)+100;
			snows[i].y = float(rand() % HEIGHT_Y);
			snows[i].rotationY = (rand() % 100) / 50.0f*D3DX_PI;
			snows[i].rotationX = (rand() % 100) / 50.0f*D3DX_PI;
			snows[i].fallSpeedy = (300.0f + rand() % 500)/10000;
			snows[i].fallSpeedx = (300.0f + rand() % 500) / 10000;
			snows[i].fallSpeedz = (300.0f + rand() % 500) / 10000;
			snows[i].rotationSpeed = (5.0f + rand() % 10 / 10.0f)/1000;
			snows[i].textureIndex = rand() % 6;
		}



		pdev->CreateVertexBuffer(4 * sizeof(PointVertex), 0,
			FVF_PVERTEX, D3DPOOL_MANAGED, &vertexBuffer, nullptr);


		PointVertex vertices[] =
		{
			{ -3.0f, 0.0f, 0.0f,   0.0f, 1.0f, },
			{ -3.0f, 6.0f, 0.0f,   0.0f, 0.0f, },
			{ 3.0f, 0.0f, 0.0f,   1.0f, 1.0f, },
			{ 3.0f, 6.0f, 0.0f,   1.0f, 0.0f, }
		};

		VOID* pVertices;
		vertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0);

		memcpy(pVertices, vertices, sizeof(vertices));

		vertexBuffer->Unlock();

		D3DXCreateTextureFromFile(pdev, "Textures\\snow1.jpg", &textures[0]);
		D3DXCreateTextureFromFile(pdev, "Textures\\snow2.jpg", &textures[1]);
		D3DXCreateTextureFromFile(pdev, "Textures\\snow3.jpg", &textures[2]);
		D3DXCreateTextureFromFile(pdev, "Textures\\snow4.jpg", &textures[3]);
		D3DXCreateTextureFromFile(pdev, "Textures\\snow5.jpg", &textures[4]);
		D3DXCreateTextureFromFile(pdev, "Textures\\snow6.jpg", &textures[5]);

	}
	~SnowParticle()
	{
		SAFE_RELEASE(vertexBuffer);

		for (int i = 0; i < 3; i++)
		{
			SAFE_RELEASE(textures[i]);
		}
	}
	void updateSnowParticle(float fElapsedTime)
	{
		for (int i = 0; i < NUMBER; i++)
		{
			snows[i].y -= snows[i].fallSpeedy*fElapsedTime;
			snows[i].x += snows[i].fallSpeedx*fElapsedTime;
			snows[i].z -= snows[i].fallSpeedz*fElapsedTime;
			if (snows[i].y < -10.0f)
			{
				snows[i].x = float(rand() % LENGTH_X - LENGTH_X / 2);
				snows[i].z = float(rand() % WIDTH_Z - WIDTH_Z / 2)+100;
				snows[i].y = HEIGHT_Y;
			}
			snows[i].rotationY += snows[i].rotationSpeed * fElapsedTime;
			snows[i].rotationX += snows[i].rotationSpeed * fElapsedTime;

		}
	}
	void renderSnowParticle()
	{
		//pdev->SetRenderState(D3DRS_LIGHTING, false);

		//pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  
		//pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);  
		//pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  
		//pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); 

																	 
		pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);   
		pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);  
		pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); 

															 
		pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		for (int i = 0; i < NUMBER; i++)
		{

			static D3DXMATRIX matYaw, matPitch, matTrans, matWorld,matScale;
			float scale = 0.1f;
			D3DXMatrixRotationY(&matYaw, snows[i].rotationY);
			D3DXMatrixRotationX(&matPitch, snows[i].rotationX);
			D3DXMatrixTranslation(&matTrans, snows[i].x, snows[i].y, snows[i].z);
			D3DXMatrixScaling(&matScale, scale, scale, scale);
			matWorld = matScale*matYaw * matPitch * matTrans;
			pdev->SetTransform(D3DTS_WORLD, &matWorld);

			pdev->SetTexture(0, textures[snows[i].textureIndex]);
			pdev->SetStreamSource(0, vertexBuffer, 0, sizeof(PointVertex));  
			pdev->SetFVF(FVF_PVERTEX);  
			pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);  

		}

		pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pdev->SetRenderState(D3DRS_LIGHTING, true);
	}
};