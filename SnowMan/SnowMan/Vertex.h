#pragma once
#include "Direct3D.h"

struct Vertex
{
	Vertex() {}
	Vertex(float x,float y,float z, float u,float v):pos(x,y,z),norm(x,y,z),tu(u),tv(v){}
	D3DXVECTOR3 pos;     // Vertex position
	D3DXVECTOR3 norm;    // Vertex normal
	float tu;            // Texture coordinate u
	float tv;            // Texture coordinate v
};

#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1

struct PointVertex
{
	float x, y, z;    
	float u, v;		  
};
#define FVF_PVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)