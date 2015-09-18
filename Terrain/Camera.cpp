#include "Camera.h"

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	800						
#define WINDOW_HEIGHT	600					
#endif


Camera::Camera(IDirect3DDevice9 *pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vRightVector  = D3DXVECTOR3(1.0f, 0.0f, 0.0f);   
	m_vUpVector     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  
	m_vLookVector   = D3DXVECTOR3(0.0f, 0.0f, 1.0f);   
	m_vCameraPosition  = D3DXVECTOR3(0.0f, 0.0f, -250.0f);   
	m_vTargetPosition    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


VOID Camera::CalculateViewMatrix(D3DXMATRIX *pMatrix) 
{
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);  
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);   
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);               
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);    
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);          


	pMatrix->_11 = m_vRightVector.x;         
	pMatrix->_12 = m_vUpVector.x;           
	pMatrix->_13 = m_vLookVector.x;         
	pMatrix->_14 = 0.0f;

	pMatrix->_21 = m_vRightVector.y;           
	pMatrix->_22 = m_vUpVector.y;              
	pMatrix->_23 = m_vLookVector.y;           
	pMatrix->_24 = 0.0f;

	pMatrix->_31 = m_vRightVector.z;           // Rz
	pMatrix->_32 = m_vUpVector.z;              // Uz
	pMatrix->_33 = m_vLookVector.z;            // Lz
	pMatrix->_34 = 0.0f;

	pMatrix->_41 = -D3DXVec3Dot(&m_vRightVector, &m_vCameraPosition);    // -P*R
	pMatrix->_42 = -D3DXVec3Dot(&m_vUpVector, &m_vCameraPosition);       // -P*U
	pMatrix->_43 = -D3DXVec3Dot(&m_vLookVector, &m_vCameraPosition);     // -P*L
	pMatrix->_44 = 1.0f;
}



VOID Camera::SetTargetPosition(D3DXVECTOR3 *pLookat) 
{

	if (pLookat != NULL)  m_vTargetPosition = (*pLookat);
	else m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_vLookVector = m_vTargetPosition - m_vCameraPosition;
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);

	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);
}

VOID Camera::SetCameraPosition(D3DXVECTOR3 *pVector) 
{
	D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	m_vCameraPosition = pVector ? (*pVector) : V;
}


VOID Camera::SetViewMatrix(D3DXMATRIX *pMatrix) 
{
	if (pMatrix) 
	{
		m_matView = *pMatrix;
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);

		m_vRightVector = D3DXVECTOR3(m_matView._11, m_matView._12, m_matView._13);
		m_vUpVector    = D3DXVECTOR3(m_matView._21, m_matView._22, m_matView._23);
		m_vLookVector  = D3DXVECTOR3(m_matView._31, m_matView._32, m_matView._33);
	}
	else 
	{
		CalculateViewMatrix(&m_matView);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	}
}


VOID Camera::SetProjMatrix(D3DXMATRIX *pMatrix) 
{
	if (pMatrix != NULL) m_matProj = *pMatrix;
	else D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)((double)WINDOW_WIDTH/WINDOW_HEIGHT), 1.0f, 30000.0f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}


VOID Camera::MoveAlongRightVec(FLOAT fUnits) 
{
	m_vCameraPosition += m_vRightVector * fUnits;
	m_vTargetPosition   += m_vRightVector * fUnits;
}

VOID Camera::MoveAlongUpVec(FLOAT fUnits) 
{
	m_vCameraPosition += m_vUpVector * fUnits;
	m_vTargetPosition   += m_vUpVector * fUnits;
}

VOID Camera::MoveAlongLookVec(FLOAT fUnits) 
{
	
	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition   += m_vLookVector * fUnits;
}


VOID Camera::RotationRightVec(FLOAT fAngle) 
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vRightVector, fAngle);    //�����������
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);

	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);  //����ȷ���۲�Ŀ���
}


VOID Camera::RotationUpVec(FLOAT fAngle) 
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vUpVector, fAngle);
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);

	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}


VOID Camera::RotationLookVec(FLOAT fAngle) 
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vLookVector, fAngle);
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);

	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);
}


Camera::~Camera(void)
{
}
