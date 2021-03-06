#pragma once
#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }      
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }  
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef WORLD_MATRIX
#define WORLD_MATRIX "WorldMatrix"
#endif

#ifndef VIEW_MATRIX
#define VIEW_MATRIX "ViewMatrix"
#endif

#ifndef PROJ_MATRIX
#define PROJ_MATRIX "ProjMatrix"
#endif

#ifndef VIEW_LIGHT_PROJ_MATRIX
#define VIEW_LIGHT_PROJ_MATRIX "ViewToLightProjMatrix"
#endif

#ifndef TEXTURE
#define TEXTURE "ColorTexture"
#endif

#ifndef SHADOW
#define SHADOW "ShadowTexture"
#endif

#ifndef MATERIAL
#define MATERIAL "Material"
#endif

#ifndef LIGHTDIR
#define LIGHTDIR "LightDir"
#endif

#ifndef LIGHTDIFFUSE
#define LIGHTDIFFUSE "LightDiffuse"
#endif

#ifndef LIGHTAMBIENT
#define LIGHTAMBIENT "LightAmbient"
#endif



struct Object
{
	LPD3DXMESH mesh;
	D3DMATERIAL9 material;
	D3DXMATRIX matrix;
	LPDIRECT3DTEXTURE9 texture;
};
