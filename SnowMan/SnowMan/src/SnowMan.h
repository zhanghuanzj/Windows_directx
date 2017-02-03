#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include "CommonUtil.h"

using namespace std;

class SnowMan
{
public:
	SnowMan(LPDIRECT3DDEVICE9 dev, float radius = 1.0f, int slices = 100, int stacks = 100);

	LPD3DXMESH get_sphere(float radius = 1.0f, int slices = 100, int stacks = 100);

	LPD3DXMESH get_cylinder(float front_radius, float back_radius, float lenght);

	void draw_snowMan(LPD3DXEFFECT pEffect = nullptr);

	~SnowMan();

private:

	void draw_body(D3DXMATRIX worldMatrix);

	void draw_eyes(D3DXMATRIX worldMatrix);

	void draw_nose(D3DXMATRIX worldMatrix);

	void draw_hands(D3DXMATRIX worldMatrix);


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