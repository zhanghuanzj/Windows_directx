#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Vertex.h"
#include "CommonUtil.h"

using namespace std;

class SnowMan
{
public:
	SnowMan(LPDIRECT3DDEVICE9 dev, float radius = 1.0f, int slices = 100, int stacks = 100) :pdev(dev), r(radius), objects(9)
	{

		body = get_sphere(size*radius);
		head = get_sphere(radius);
		eye = get_cylinder(r*0.1, r*0.1, r*0.1);
		nose = get_cylinder(r*0.04, r*0.12, r*0.7);
		arm = get_cylinder(r*0.04, r*0.06, r);
		hand = get_cylinder(r*0.03, r*0.06, 0.2*r);
		::ZeroMemory(&snowMtrl, sizeof(snowMtrl));
		snowMtrl.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		snowMtrl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		snowMtrl.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		::ZeroMemory(&eyeMtrl, sizeof(eyeMtrl));
		eyeMtrl.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		eyeMtrl.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		eyeMtrl.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		D3DXMATRIX matrix;
		D3DXMatrixTranslation(&matrix, 0.0f, 0.0f, 0.0f);
		D3DXMATRIX bodyMatrix;
		D3DXMatrixTranslation(&bodyMatrix, 0.0f, -(size + 0.3)*r, 0.0f);
		D3DXMATRIX eyeMatrixL, eyeMatrixR;
		D3DXMatrixTranslation(&eyeMatrixL, -0.4*r, 0.4*r, -0.9*r);
		D3DXMatrixTranslation(&eyeMatrixR, 0.4*r, 0.4*r, -0.9*r);
		D3DXMATRIX noseMatrix;
		D3DXMatrixTranslation(&noseMatrix, 0, 0, -1.3*r);
		D3DXMATRIX treeMatrix11;
		D3DXMatrixTranslation(&treeMatrix11, -1.5*r, -1 * r, 0);
		D3DXMATRIX armMatrix1;
		D3DXMATRIX handMatrix1;
		D3DXMatrixRotationAxis(&armMatrix1, new D3DXVECTOR3(1, 1.5, 0), D3DX_PI / 2);
		D3DXMATRIX treeMatrix12;
		D3DXMatrixTranslation(&treeMatrix12, -1.7*r, -0.95 * r, 0);
		D3DXMatrixRotationAxis(&handMatrix1, new D3DXVECTOR3(0, 1, 0), D3DX_PI / 2);

		D3DXMATRIX handMatrix2;
		D3DXMATRIX armMatrix2;
		D3DXMATRIX treeMatrix21;
		D3DXMATRIX treeMatrix22;
		D3DXMatrixTranslation(&treeMatrix21, 1.5*r, -1 * r, 0);
		D3DXMatrixRotationAxis(&armMatrix2, new D3DXVECTOR3(-1, 1.5, 0), -D3DX_PI / 2);

		D3DXMatrixTranslation(&treeMatrix22, 1.7*r, -0.75 * r, 0);
		D3DXMatrixRotationAxis(&handMatrix2, new D3DXVECTOR3(1, 0, 0), D3DX_PI / 2);
		objects[0] = {head,snowMtrl,matrix,Texture::Instance(dev).get_snow()};
		objects[1] = {body,snowMtrl,bodyMatrix,Texture::Instance(dev). get_snow()};
		objects[2] = { eye,eyeMtrl,eyeMatrixL,Texture::Instance(dev).get_black()};
		objects[3] = { eye,eyeMtrl,eyeMatrixR,Texture::Instance(dev).get_black()};
		objects[4] = { nose,eyeMtrl,noseMatrix,Texture::Instance(dev).get_orange() };
		objects[5] = { arm,eyeMtrl,armMatrix1*treeMatrix11,Texture::Instance(dev).get_tree() };
		objects[6] = { hand,eyeMtrl,handMatrix1*treeMatrix12,Texture::Instance(dev).get_tree() };
		objects[7] = { arm,eyeMtrl,armMatrix2*treeMatrix21,Texture::Instance(dev).get_tree() };
		objects[8] = { hand,eyeMtrl,handMatrix2*treeMatrix22,Texture::Instance(dev).get_tree() };
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
		return texMesh;
	}

	LPD3DXMESH get_cylinder(float front_radius,float back_radius,float lenght)
	{
		LPD3DXMESH texMesh;
		LPD3DXMESH mesh;
		D3DXCreateCylinder(pdev, front_radius, back_radius, lenght, 10, 10, &mesh, nullptr);

		// Get a copy of the sphere mesh
		mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pdev, &texMesh);
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
				pVerts->tu = pVerts->norm.x / 2 + 0.5f; //(-1, 1)-->(0, 1)
				pVerts->tv = pVerts->norm.y / 2 + 0.5f;
				++pVerts;
			}
			// Unlock the vertex buffer
			texMesh->UnlockVertexBuffer();
		}
		return texMesh;
	}

	void draw_snowMan(LPD3DXEFFECT pEffect = nullptr)
	{
		D3DXMATRIX worldMatrix;
		pdev->GetTransform(D3DTS_WORLD, &worldMatrix);
		if (pEffect==nullptr)
		{
			
			draw_body(worldMatrix);
			draw_eyes(worldMatrix);
			draw_nose(worldMatrix);
			draw_hands(worldMatrix);
		}
		else
		{
			for (int i=0;i<9;++i)
			{
				auto obj = objects[i];
				D3DXMATRIX matrix = obj.matrix*worldMatrix;
				pEffect->SetMatrix(WORLD_MATRIX, &matrix);
				pEffect->SetTexture(TEXTURE, obj.texture);
				pEffect->SetVector(MATERIAL, new D3DXVECTOR4(obj.material.Diffuse.r, obj.material.Diffuse.g, obj.material.Diffuse.b, obj.material.Diffuse.a));
				UINT iPass, cPasses;
				pEffect->Begin(&cPasses, 0);
				for (iPass = 0; iPass < cPasses; iPass++)
				{
					pEffect->BeginPass(iPass);
					obj.mesh->DrawSubset(0);
					pEffect->EndPass();
				}
				pEffect->End();
			}
		}
	}


	~SnowMan()
	{
		SAFE_RELEASE(head);
		SAFE_RELEASE(eye);
		SAFE_RELEASE(body);
		SAFE_RELEASE(hand);
		SAFE_RELEASE(nose);
		SAFE_RELEASE(arm);
	}

private:

	void draw_body(D3DXMATRIX worldMatrix)
	{
		D3DXMATRIX bodyMatrix;
		D3DXMatrixTranslation(&bodyMatrix, 0.0f, -(size + 0.3)*r, 0.0f);
		bodyMatrix *= worldMatrix;	

		pdev->SetMaterial(&snowMtrl);
		pdev->SetTexture(0, Texture::Instance(pdev).get_snow());
		head->DrawSubset(0);
		pdev->SetTransform(D3DTS_WORLD, &bodyMatrix);
		body->DrawSubset(0);
		pdev->SetTransform(D3DTS_WORLD, &worldMatrix);
	}

	void draw_eyes(D3DXMATRIX worldMatrix)
	{
		D3DXMATRIX eyeMatrixL,eyeMatrixR;
		D3DXMatrixTranslation(&eyeMatrixL, -0.4*r, 0.4*r, -0.9*r);
		D3DXMatrixTranslation(&eyeMatrixR, 0.4*r, 0.4*r, -0.9*r);
		eyeMatrixL *= worldMatrix;
		eyeMatrixR *= worldMatrix;
		
		pdev->SetMaterial(&eyeMtrl);
		pdev->SetTexture(0, Texture::Instance(pdev).get_black());
		//Left
		pdev->SetTransform(D3DTS_WORLD, &eyeMatrixL);
		eye->DrawSubset(0);
		//Right
		pdev->SetTransform(D3DTS_WORLD, &eyeMatrixR);
		eye->DrawSubset(0);
	}

	void draw_nose(D3DXMATRIX worldMatrix)
	{
		D3DXMATRIX noseMatrix;
		D3DXMatrixTranslation(&noseMatrix, 0, 0, -1.3*r);
		noseMatrix *= worldMatrix;
		
		pdev->SetTexture(0, Texture::Instance(pdev).get_orange());
		pdev->SetTransform(D3DTS_WORLD, &noseMatrix);
		nose->DrawSubset(0);
		
	}

	void draw_hands(D3DXMATRIX worldMatrix)
	{
		pdev->SetTexture(0, Texture::Instance(pdev).get_tree());
		D3DXMATRIX treeMatrix;
		D3DXMatrixTranslation(&treeMatrix, -1.5*r, -1 * r, 0);
		D3DXMATRIX armMatrix;
		D3DXMatrixRotationAxis(&armMatrix, new D3DXVECTOR3(1, 1.5, 0), D3DX_PI / 2);
		treeMatrix = armMatrix*treeMatrix*worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &treeMatrix);
		arm->DrawSubset(0);

		D3DXMATRIX handMatrix;
		D3DXMatrixTranslation(&treeMatrix, -1.7*r, -0.95 * r, 0);
		D3DXMatrixRotationAxis(&handMatrix, new D3DXVECTOR3(0, 1, 0), D3DX_PI / 2);
		treeMatrix = handMatrix*treeMatrix*worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &treeMatrix);
		hand->DrawSubset(0);

		D3DXMatrixTranslation(&treeMatrix, 1.5*r, -1 * r, 0);
		D3DXMatrixRotationAxis(&armMatrix, new D3DXVECTOR3(-1, 1.5, 0), -D3DX_PI / 2);
		treeMatrix = armMatrix*treeMatrix*worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &treeMatrix);
		arm->DrawSubset(0);

		D3DXMatrixTranslation(&treeMatrix, 1.7*r, -0.75 * r, 0);
		D3DXMatrixRotationAxis(&handMatrix, new D3DXVECTOR3(1, 0, 0), D3DX_PI / 2);
		treeMatrix = handMatrix*treeMatrix*worldMatrix;
		pdev->SetTransform(D3DTS_WORLD, &treeMatrix);
		hand->DrawSubset(0);
	}


	LPD3DXMESH head;
	LPD3DXMESH body;
	LPD3DXMESH eye;
	LPD3DXMESH nose;
	LPD3DXMESH arm;
	LPD3DXMESH hand;
	vector<Object> objects;
	LPDIRECT3DDEVICE9 pdev;

	D3DMATERIAL9 snowMtrl;
	D3DMATERIAL9 eyeMtrl;
	const float size = 1.5f;
	float r;
};