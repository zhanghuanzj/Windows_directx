#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

class Model
{
private:
	LPDIRECT3DDEVICE9				pdev;			
	LPD3DXMESH						mesh;       
	DWORD							numMaterials; 
	D3DMATERIAL9*					materials;    
	LPDIRECT3DTEXTURE9 *			textures;      

public:
	Model(LPDIRECT3DDEVICE9 pd3dDevice, const string &fileName):pdev(pd3dDevice)
	{
		LPD3DXBUFFER pAdjacencyBuffer = nullptr;
		LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;
		D3DXLoadMeshFromX(fileName.c_str(),D3DXMESH_MANAGED,pd3dDevice,&pAdjacencyBuffer,&pD3DXMtrlBuffer,nullptr,&numMaterials,&mesh);

		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		materials = new D3DMATERIAL9[numMaterials];
		textures = new LPDIRECT3DTEXTURE9[numMaterials];
		//Get materials and textures
		for (DWORD i=0;i<numMaterials;i++) 
		{
			materials[i] = d3dxMaterials[i].MatD3D;
			materials[i].Ambient = materials[i].Diffuse;

			textures[i] = nullptr;
			
			if (d3dxMaterials[i].pTextureFilename != nullptr && strlen(d3dxMaterials[i].pTextureFilename)>0)
			{
				//textures[i] = Texture::Instance(pd3dDevice)->get_texture(d3dxMaterials[i].pTextureFilename);
				if((textures[i]=Texture::Instance(pd3dDevice)->get_texture(d3dxMaterials[i].pTextureFilename))==nullptr)
				{
					cout << d3dxMaterials[i].pTextureFilename << endl;
					cout << "Sorry can't find the texture file!" << endl;
					MessageBox(nullptr,"Sorry can't find the texture file!","Model reading error",MB_OK);
				}
			}
		}

		if (!(mesh->GetFVF()&D3DFVF_NORMAL))
		{
			LPD3DXMESH tempMesh = nullptr;
			mesh->CloneMeshFVF(D3DXMESH_MANAGED, mesh->GetFVF() | D3DFVF_NORMAL, pdev, &tempMesh);
			D3DXComputeNormals(tempMesh, 0);
			mesh->Release();
			mesh = tempMesh;
		}
		//Optimize
		mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_STRIPREORDER,
			(DWORD*)pAdjacencyBuffer->GetBufferPointer(), nullptr, nullptr, nullptr);
	}
	~Model(void)
	{
		SAFE_DELETE_ARRAY(materials);

		if (textures)
		{
			for (DWORD i = 0; i < numMaterials; i++)
			{
				SAFE_RELEASE(textures[i]);
			}
			SAFE_DELETE_ARRAY(textures);
		}
		SAFE_RELEASE(mesh);
	}

public: 
	void renderModel(LPD3DXEFFECT pEffect = nullptr)
	{
		//pEffect = nullptr;
		D3DXMATRIX worldMatrix;
		pdev->GetTransform(D3DTS_WORLD, &worldMatrix);
		if (pEffect != nullptr)
		{
			pEffect->SetMatrix(WORLD_MATRIX, &worldMatrix);
			for (DWORD i = 0; i<numMaterials; i++)
			{
				pEffect->SetTexture(TEXTURE, textures[i]);
				pEffect->SetVector(MATERIAL, new D3DXVECTOR4(materials[i].Diffuse.r, materials[i].Diffuse.g, materials[i].Diffuse.b, materials[i].Diffuse.a));
				UINT iPass, cPasses;
				pEffect->Begin(&cPasses, 0);
				for (iPass = 0; iPass < cPasses; iPass++)
				{
					pEffect->BeginPass(iPass);
					mesh->DrawSubset(i);
					pEffect->EndPass();
				}
				pEffect->End();
			}
		}
		else
		{
			for (DWORD i = 0; i<numMaterials; i++)
			{
				//cout << materials[i].Diffuse.r<<" "<<materials[i].Diffuse.g <<""<<materials[i].Diffuse.b<< endl;
				//pdev->SetMaterial(&terrianMaterial);
				pdev->SetMaterial(&materials[i]);
				pdev->SetTexture(0, textures[i]);
				mesh->DrawSubset(i);
			}
		}
		
		
	}

};