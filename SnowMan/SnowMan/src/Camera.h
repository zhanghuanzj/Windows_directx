#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "DirectInput.h"

class Camera
{
public:
	Camera(LPDIRECT3DDEVICE9 dev, int width, int height,
		D3DXVECTOR3 rightVector_ = D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3 upVector_ = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3 lookVector_ = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		D3DXVECTOR3 position_ = D3DXVECTOR3(0.0f, -10.0f, -30.f),
		D3DXVECTOR3 targetPosition_ = D3DXVECTOR3(0.0f, -10.0f, 0.0f)) :pdev(dev), WIDTH(width), HEIGHT(height), rightVector(rightVector_), upVector(upVector_), lookVector(lookVector_), position(position_)
	{}

	//Combine the translation and rotate Matrix
	void calculate_viewMatrix(D3DXMATRIX *pMatrix);

	void set_targetPosition(D3DXVECTOR3 *tPosition);

	void set_position(D3DXVECTOR3 *pos);



	void move_alongRV(float units);

	void move_alongUV(float units);

	void move_alongLV(float units);

	void rotate_RV(float angle);

	void rotate_UV(float angle);

	void rotate_LV(float angle);

	void update(DirectInput *pDirectInput);

	void set_transform();

	D3DXVECTOR3 get_position();

	~Camera() {}

private:

	void normalize();

private:
	LPDIRECT3DDEVICE9		pdev;

	D3DXMATRIX				viewMatrix;
	D3DXMATRIX				projMatrix;

	D3DXVECTOR3				targetPosition;
	D3DXVECTOR3				rightVector;
	D3DXVECTOR3				upVector;
	D3DXVECTOR3				lookVector;
	D3DXVECTOR3				position;

	const float step = 0.3f*20;
	const int	originZ = -30.0f;
	const int	WIDTH;
	const int	HEIGHT;
};
