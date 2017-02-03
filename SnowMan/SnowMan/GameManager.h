#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <iostream>
#include <vector>

#include "CommonUtil.h"
#include "SnowMan.h"
#include "Cube.h"
#include "Terrian.h"
#include "Camera.h"
#include "SkyBox.h"
#include "SnowParticle.h"
#include "DirectInput.h"
#include "Model.h"

class GameManager
{
	SnowMan *snowMan1 = nullptr;
	SnowMan *snowMan2 = nullptr;
	Cube *cube = nullptr;
	Terrian *terrian = nullptr;
	SkyBox *skyBox = nullptr;

	SnowParticle *pSnowParticle = nullptr;
	DirectInput *pDirectInput = nullptr;
	Model *pTree = nullptr;
	Model *pTree1 = nullptr;
	Model *pHouse = nullptr;

	Camera *pCamera = nullptr;
	Camera *pLCamera = nullptr;
	static int n;
	static D3DXVECTOR4 LightDiffuse;
	static D3DXVECTOR4 LightAmbient;
	static D3DXVECTOR4 LightDirection;
};
