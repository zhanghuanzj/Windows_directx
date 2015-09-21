#ifndef SKYBOX_H
#define SKYBOX_H

#include "MyDirectX.h"
#include <string>
using namespace std;

struct SKYBOXVERTEX
{
	float	x,y,z;
	float	u,v;
};
#define D3DFVF_SKYBOX D3DFVF_XYZ|D3DFVF_TEX1


class SkyBox
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;	
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;		
	LPDIRECT3DTEXTURE9				m_pTexture[5];			
	float							m_Length;				

public:
	SkyBox( LPDIRECT3DDEVICE9  pDevice );		
	virtual ~SkyBox(void);								

public:
	BOOL	InitSkyBox( float Length );   
	BOOL	LoadSkyTextureFromFile(string pFrontTextureFile, string pBackTextureFile,string pLeftTextureFile, string pRightTextureFile,string pTopTextureFile);  
	VOID	RenderSkyBox( D3DXMATRIX *pMatWorld, BOOL bRenderFrame ); 

};

#endif 
