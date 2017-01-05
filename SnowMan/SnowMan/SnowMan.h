#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

struct VERTEX
{
	D3DXVECTOR3 pos;     // Vertex position
	D3DXVECTOR3 norm;    // Vertex normal
	float tu;            // Texture coordinate u
	float tv;            // Texture coordinate v
};

#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1

class SnowMan
{
public:
	SnowMan(LPDIRECT3DDEVICE9 dev,float radius=1.0f,int slices = 100,int stacks = 100)
	{
		pdev = dev;
		r = radius;
		D3DXCreateTextureFromFile(dev, "snow.jpg", &snow);
		D3DXCreateTextureFromFile(dev, "eye.jpg", &black);
		body = get_sphere(size*radius);
		head = get_sphere(radius);
		eye = get_eye();
	}

	LPD3DXMESH get_sphere(float radius = 1.0f, int slices = 100, int stacks = 100)
	{
		LPD3DXMESH texMesh;
		LPD3DXMESH mesh;
		D3DXCreateSphere(pdev, radius, slices, stacks, &mesh, nullptr);

		// Get a copy of the sphere mesh
		mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pdev, &texMesh);

		// Release original mesh
		mesh->Release();

		// add texture coordinates
		VERTEX* pVerts;
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
		return texMesh;
	}

	LPD3DXMESH get_eye()
	{
		LPD3DXMESH texMesh;
		LPD3DXMESH mesh;
		D3DXCreateCylinder(pdev, r*0.1, r*0.1, r*0.1, 10, 10, &mesh, nullptr);

		// Get a copy of the sphere mesh
		mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pdev, &texMesh);
		// Release original mesh
		mesh->Release();
		// add texture coordinates
		VERTEX* pVerts;
		if (SUCCEEDED(texMesh->LockVertexBuffer(0, (void**)&pVerts)))
		{
			// Get vertex count
			int numVerts = texMesh->GetNumVertices();
			for (int i = 0; i < numVerts; ++i)
			{
				// Calculates texture coordinates
				pVerts->tu = pVerts->norm.x / 2 + 0.5f; //(-1, 1)-->(0, 1)
				pVerts->tv = pVerts->norm.y / 2 + 0.5f;
				++pVerts;
			}
			// Unlock the vertex buffer
			texMesh->UnlockVertexBuffer();
		}
		return texMesh;
	}
	void draw_snowMan()
	{
		D3DXMATRIX worldMatrix;
		pdev->GetTransform(D3DTS_WORLD,&worldMatrix);

		pdev->SetTexture(0, snow);
		head->DrawSubset(0);

		D3DXMATRIX bodyMatrix;
		D3DXMatrixTranslation(&bodyMatrix, 0.0f, -(size+0.3)*r, 0.0f);
		bodyMatrix *= worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &bodyMatrix);
		body->DrawSubset(0);

		pdev->SetTexture(0, black);
		D3DXMATRIX eyeMatrixL;
		D3DXMatrixTranslation(&eyeMatrixL, -0.5*r, 0.4*r, -0.8*r);
		eyeMatrixL *= worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &eyeMatrixL);
		eye->DrawSubset(0);

		D3DXMATRIX eyeMatrixR;
		D3DXMatrixTranslation(&eyeMatrixR, 0.5*r, 0.4*r, -0.8*r);
		eyeMatrixR *= worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &eyeMatrixR);
		eye->DrawSubset(0);

		pdev->SetTransform(D3DTS_WORLD, &worldMatrix);
	}

	~SnowMan()
	{
		if (head != nullptr)
		{
			head->Release();
		}
		if (body != nullptr)
		{
			body->Release();
		}
	}
	LPD3DXMESH head;
	LPD3DXMESH body;
	LPD3DXMESH eye;

	LPDIRECT3DDEVICE9 pdev;
	LPDIRECT3DTEXTURE9 snow;
	LPDIRECT3DTEXTURE9 black;
	const float size = 1.5f;
	float r;
};