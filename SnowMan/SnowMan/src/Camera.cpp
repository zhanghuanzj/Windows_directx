#include "Camera.h"

//Combine the translation and rotate Matrix
void Camera::calculate_viewMatrix(D3DXMATRIX *pMatrix)
{
	normalize();

	pMatrix->_11 = rightVector.x;           // Rx
	pMatrix->_12 = upVector.x;              // Ux
	pMatrix->_13 = lookVector.x;            // Lx
	pMatrix->_14 = 0.0f;

	pMatrix->_21 = rightVector.y;           // Ry
	pMatrix->_22 = upVector.y;              // Uy
	pMatrix->_23 = lookVector.y;            // Ly
	pMatrix->_24 = 0.0f;

	pMatrix->_31 = rightVector.z;           // Rz
	pMatrix->_32 = upVector.z;              // Uz
	pMatrix->_33 = lookVector.z;            // Lz
	pMatrix->_34 = 0.0f;

	pMatrix->_41 = -D3DXVec3Dot(&rightVector, &position);    // -P*R
	pMatrix->_42 = -D3DXVec3Dot(&upVector, &position);       // -P*U
	pMatrix->_43 = -D3DXVec3Dot(&lookVector, &position);     // -P*L
	pMatrix->_44 = 1.0f;
}

void Camera::set_targetPosition(D3DXVECTOR3 *tPosition)
{
	if (tPosition != nullptr)
	{
		targetPosition = *tPosition;
	}
	else
	{
		targetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}

	lookVector = targetPosition - position;

	normalize();
}

void Camera::set_position(D3DXVECTOR3 *pos)
{
	D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, originZ);
	position = pos ? (*pos) : V;
}



void Camera::move_alongRV(float units)
{
	position += rightVector * units;
	targetPosition += rightVector * units;
}

void Camera::move_alongUV(float units)
{
	position += upVector * units;
	targetPosition += upVector * units;
}

void Camera::move_alongLV(float units)
{
	position += lookVector * units;
	targetPosition += lookVector * units;
}

void Camera::rotate_RV(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &rightVector, angle);
	D3DXVec3TransformCoord(&upVector, &upVector, &R);
	D3DXVec3TransformCoord(&lookVector, &lookVector, &R);

	targetPosition = lookVector * D3DXVec3Length(&position);
}

void Camera::rotate_UV(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &upVector, angle);
	D3DXVec3TransformCoord(&rightVector, &rightVector, &R);
	D3DXVec3TransformCoord(&lookVector, &lookVector, &R);

	targetPosition = lookVector * D3DXVec3Length(&position);
}

void Camera::rotate_LV(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &lookVector, angle);
	D3DXVec3TransformCoord(&rightVector, &rightVector, &R);
	D3DXVec3TransformCoord(&upVector, &upVector, &R);

	targetPosition = lookVector * D3DXVec3Length(&position);
}

void Camera::update(DirectInput *pDirectInput)
{
	// camera move by keyboard
	if (pDirectInput->is_key_down(DIK_A))  move_alongRV(-step);
	if (pDirectInput->is_key_down(DIK_D))  move_alongRV(step);
	if (pDirectInput->is_key_down(DIK_W)) move_alongLV(step);
	if (pDirectInput->is_key_down(DIK_S))  move_alongLV(-step);
	if (pDirectInput->is_key_down(DIK_R))  move_alongUV(step);
	if (pDirectInput->is_key_down(DIK_F))  move_alongUV(-step);

	// camera rotate by keyboard
	if (pDirectInput->is_key_down(DIK_LEFT))  rotate_UV(-0.003f);
	if (pDirectInput->is_key_down(DIK_RIGHT))  rotate_UV(0.003f);
	if (pDirectInput->is_key_down(DIK_UP))  rotate_RV(-0.003f);
	if (pDirectInput->is_key_down(DIK_DOWN))  rotate_RV(0.003f);
	if (pDirectInput->is_key_down(DIK_Q)) rotate_LV(0.001f);
	if (pDirectInput->is_key_down(DIK_E)) rotate_LV(-0.001f);

	float s = pDirectInput->mouseDZ()*0.01f;
	move_alongUV(s);
	// camera rotate by mouse
	rotate_RV(pDirectInput->mouseDY()* 0.001f);
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, pDirectInput->mouseDX()* 0.001f);
	D3DXVec3TransformCoord(&lookVector, &lookVector, &R);
	D3DXVec3TransformCoord(&upVector, &upVector, &R);
	D3DXVec3TransformCoord(&rightVector, &rightVector, &R);

	set_transform();
	//cout<<position.x<<" "<<position.y<<" "<<position.z<<endl;
}

void Camera::set_transform()
{
	//1.set view transform
	D3DXMATRIX matView;
	calculate_viewMatrix(&matView);
	pdev->SetTransform(D3DTS_VIEW, &matView);

	//2.set project transform
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)((double)WIDTH / HEIGHT), 1.0f, 100000.0f);
	pdev->SetTransform(D3DTS_PROJECTION, &matProj);

	//3.set viewport transform
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	pdev->SetViewport(&vp);
}

D3DXVECTOR3 Camera::get_position()
{
	return position;
}


void Camera::normalize()
{
	D3DXVec3Normalize(&lookVector, &lookVector);
	D3DXVec3Cross(&upVector, &lookVector, &rightVector);
	D3DXVec3Normalize(&upVector, &upVector);
	D3DXVec3Cross(&rightVector, &upVector, &lookVector);
	D3DXVec3Normalize(&rightVector, &rightVector);
}

