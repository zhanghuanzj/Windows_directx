#pragma once
#include "Direct3D.h"


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
			//cout << d3dxMaterials[i].pTextureFilename << endl;
			if (d3dxMaterials[i].pTextureFilename != nullptr && strlen(d3dxMaterials[i].pTextureFilename)>0)
			{
				if (FAILED(D3DXCreateTextureFromFileA(pd3dDevice,d3dxMaterials[i].pTextureFilename,&textures[i])))
				{
					MessageBox(nullptr,"Sorry can't find the texture file!","Model reading error",MB_OK);
				}
			}
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
	void renderModel()
	{
		for (DWORD i=0;i<numMaterials;i++) 
		{
			pdev->SetMaterial(&materials[i]);
			pdev->SetTexture(0, textures[i]);
			mesh->DrawSubset(i);
		}
	}

};