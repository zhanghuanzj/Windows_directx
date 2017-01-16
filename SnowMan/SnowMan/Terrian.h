#pragma once

#include "Direct3D.h"
#include "Window.h"
#include "Vertex.h"

using namespace std;
class Terrian
{
private:
	LPDIRECT3DDEVICE9			pdev;			//D3D设备
	LPDIRECT3DTEXTURE9			texture;				//纹理
	LPDIRECT3DINDEXBUFFER9		indexBuffer;			//顶点缓存
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;		//索引缓存
	D3DMATERIAL9				terrianMaterial;

	int							cellsPerRow;		// 每行的单元格数
	int							cellsPerCol;			// 每列的单元格数
	int							vertsPerRow;		// 每行的顶点数
	int							vertsPerCol;			// 每列的顶点数
	int							numVertices;		// 顶点总数
	float						terrainWidth;		// 地形的宽度
	float						terrainDepth;		// 地形的深度
	float						cellSpacing;			// 单元格的间距
	float						heightScale;			// 高度缩放系数
	vector<float>				heightInfo;			// 用于存放高度信息


public:
	Terrian(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		pdev = pd3dDevice;
		texture = nullptr;
		indexBuffer = nullptr;
		vertexBuffer = nullptr;
		cellsPerRow = 0;
		cellsPerCol = 0;
		vertsPerRow = 0;
		vertsPerCol = 0;
		numVertices = 0;
		terrainWidth = 0.0f;
		terrainDepth = 0.0f;
		cellSpacing = 0.0f;
		heightScale = 0.0f;
		::ZeroMemory(&terrianMaterial, sizeof(terrianMaterial));
		terrianMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		terrianMaterial.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		terrianMaterial.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}
	virtual ~Terrian(void)
	{
		SAFE_RELEASE(texture);
		SAFE_RELEASE(indexBuffer);
		SAFE_RELEASE(vertexBuffer);
	}

public:
	bool load_terrain_from_file(const string &rawFileName, const string &textureFile)
	{
		cout << rawFileName << endl;
		ifstream inFile;
		inFile.open(rawFileName, ios::binary); 

		inFile.seekg(0, ios::end);							
		vector<BYTE> inData(inFile.tellg());			

		inFile.seekg(ios::beg);								
		inFile.read((char*)&inData[0], inData.size());	
		inFile.close();												

		heightInfo.resize(inData.size());					
		for (unsigned int i = 0; i<inData.size(); i++)
			heightInfo[i] = inData[i];

		if (FAILED(D3DXCreateTextureFromFile(pdev, textureFile.c_str(), &texture)))
			return false;
		return true;
	}
	bool init_terrain(int nRows, int nCols, float fSpace, float fScale)
	{
		cellsPerRow = nRows;  
		cellsPerCol = nCols;  
		cellSpacing = fSpace;	
		heightScale = fScale; 
		terrainWidth = nRows * fSpace;  
		terrainDepth = nCols * fSpace;  
		vertsPerRow = cellsPerCol + 1;  
		vertsPerCol = cellsPerRow + 1; 
		numVertices = vertsPerRow * vertsPerCol;  
									
		for (unsigned int i = 0; i<heightInfo.size(); i++)
			heightInfo[i] *= heightScale;

		//1.vertex buffer
		if (FAILED(pdev->CreateVertexBuffer(numVertices * sizeof(Vertex),
			D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &vertexBuffer, 0)))
			return false;
		//2.lock
		Vertex *pVertices = nullptr;
		vertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
		//3.fill
		float fStartX = -terrainWidth / 2.0f, fEndX = terrainWidth / 2.0f;		
		float fStartZ = terrainDepth / 2.0f, fEndZ = -terrainDepth / 2.0f;	
		float fCoordU = 5.0f / (float)cellsPerRow;     
		float fCoordV = 5.0f / (float)cellsPerCol;		

		int nIndex = 0, i = 0, j = 0;
		for (float z = fStartZ; z > fEndZ; z -= cellSpacing, i++)	
		{
			j = 0;
			for (float x = fStartX; x < fEndX; x += cellSpacing, j++)	
			{
				nIndex = i * cellsPerRow + j;	
				pVertices[nIndex] = Vertex(x, heightInfo[nIndex], z, j*fCoordU, i*fCoordV);
				//cout << x << " " << heightInfo[nIndex] << " " << z << endl;
				nIndex++;										
			}
		}
		

		//1.index buffer
		if (FAILED(pdev->CreateIndexBuffer(numVertices * 6 * sizeof(WORD),
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, 0)))
			return FALSE;
		//2.lock
		WORD* pIndices = nullptr;
		indexBuffer->Lock(0, 0, (void**)&pIndices, 0);
		//3.fill
		nIndex = 0;
		for (int row = 0; row < cellsPerRow - 1; row++)  
		{
			for (int col = 0; col < cellsPerCol - 1; col++) 
			{

				pIndices[nIndex] = row * cellsPerRow + col;			
				pIndices[nIndex + 1] = row * cellsPerRow + col + 1;  
				pIndices[nIndex + 2] = (row + 1) * cellsPerRow + col;	
				compute_normal(pVertices[pIndices[nIndex]], pVertices[pIndices[nIndex+1]], pVertices[pIndices[nIndex+2]]);
																		
				pIndices[nIndex + 3] = (row + 1) * cellsPerRow + col;		
				pIndices[nIndex + 4] = row * cellsPerRow + col + 1;		
				pIndices[nIndex + 5] = (row + 1) * cellsPerRow + col + 1;
				compute_normal(pVertices[pIndices[nIndex+3]], pVertices[pIndices[nIndex + 4]], pVertices[pIndices[nIndex + 5]]);
																		 
				nIndex += 6;  
			}
		}
		//4.unlock
		indexBuffer->Unlock();
		//4.unlock
		vertexBuffer->Unlock();

		return true;
	}

	void compute_normal(Vertex &v1,  Vertex &v2,  Vertex &v3)
	{
		D3DXVECTOR3 vector1 = v2.pos - v1.pos;
		D3DXVECTOR3 vector2 = v3.pos - v1.pos;
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal,&vector1,&vector2);
		D3DXVec3Normalize(&normal, &normal);
		v1.norm = normal;
		v2.norm = normal;
		v3.norm = normal;
		//cout << normal.x << " " << normal.y << " " << normal.z << endl;
	}

	bool render_terrain(LPD3DXEFFECT pEffect = nullptr)
	{
		D3DXMATRIX worldMatrix;
		pdev->GetTransform(D3DTS_WORLD, &worldMatrix);
		D3DXMATRIX terrianMatrix;
		D3DXMatrixScaling(&terrianMatrix, 0.01f, 0.01f, 0.01f);
		D3DXMATRIX terrianMatrix1;
		D3DXMatrixTranslation(&terrianMatrix1, 2500, -2000.0f, -2000);
		terrianMatrix = (*worldMatrix)*terrianMatrix1*terrianMatrix;
		//pEffect = nullptr;
		if (pEffect != nullptr)
		{
			pEffect->SetTexture(TEXTURE, texture);
			pEffect->SetMatrix(WORLD_MATRIX, &terrianMatrix);
			pEffect->SetVector(MATERIAL, new D3DXVECTOR4(terrianMaterial.Diffuse.r, terrianMaterial.Diffuse.g, terrianMaterial.Diffuse.b, terrianMaterial.Diffuse.a));
			UINT iPass, cPasses;
			pEffect->Begin(&cPasses, 0);
			for (iPass = 0; iPass < cPasses; iPass++)
			{
				pEffect->BeginPass(iPass);
				pdev->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
				pdev->SetFVF(FVF_VERTEX);
				pdev->SetIndices(indexBuffer);
				pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
					numVertices, 0, numVertices * 2);
				pEffect->EndPass();
			}
			pEffect->End();
		}
		else
		{
			pdev->SetMaterial(&terrianMaterial);
			pdev->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
			pdev->SetFVF(FVF_VERTEX);
			pdev->SetIndices(indexBuffer);
			pdev->SetTexture(0, texture);

			pdev->SetTransform(D3DTS_WORLD, &terrianMatrix);
			pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
				numVertices, 0, numVertices * 2);
		}
		
		return true;
	}
};
