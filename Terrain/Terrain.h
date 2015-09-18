#ifndef TERRAIN_H
#define TERRAIN_H


#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>
#include <string>
#define SAFE_RELEASE(device) {if(device){(device)->Release();(device)=nullptr;}}
using namespace std;
class Terrain
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;			
	LPDIRECT3DTEXTURE9				m_pTexture;				
	LPDIRECT3DINDEXBUFFER9			m_pIndexBuffer;			
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;		

	int								m_nCellsPerRow;		
	int								m_nCellsPerCol;			
	int								m_nVertsPerRow;		
	int								m_nVertsPerCol;			
	int								m_nNumVertices;		
	FLOAT							m_fTerrainWidth;		
	FLOAT							m_fTerrainDepth;		
	FLOAT							m_fCellSpacing;			
	FLOAT							m_fHeightScale;			
	std::vector<FLOAT>				m_vHeightInfo;			


	struct TERRAINVERTEX
	{
		FLOAT _x, _y, _z;
		FLOAT _u, _v;
		TERRAINVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) 
			:_x(x), _y(y), _z(z), _u(u), _v(v) {}
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};

public:
	Terrain(IDirect3DDevice9 *pd3dDevice); 
	virtual ~Terrain(void);		

public:
	BOOL LoadTerrainFromFile(string pRawFileName, string pTextureFile);		
	BOOL InitTerrain(INT nRows, INT nCols, FLOAT fSpace, FLOAT fScale);  
	BOOL RenderTerrain(D3DXMATRIX *pMatWorld, BOOL bDrawFrame=FALSE);  
};

#endif // !TERRAIN_H
