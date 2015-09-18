

#ifndef CAMERA_H
#define CAMERA_H

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
private:
	D3DXVECTOR3				m_vRightVector;        //x
	D3DXVECTOR3				m_vUpVector;           //y
	D3DXVECTOR3				m_vLookVector;         //z
	D3DXVECTOR3				m_vCameraPosition;        
	D3DXVECTOR3				m_vTargetPosition;       
	D3DXMATRIX				m_matView;            //view matrix(camera)
	D3DXMATRIX				m_matProj;            //project matrix
	LPDIRECT3DDEVICE9		m_pd3dDevice;  

public:

	VOID CalculateViewMatrix(D3DXMATRIX *pMatrix);    


	VOID GetProjMatrix(D3DXMATRIX *pMatrix)  { *pMatrix = m_matProj; }  
	VOID GetCameraPosition(D3DXVECTOR3 *pVector)  { *pVector = m_vCameraPosition; } 
	VOID GetLookVector(D3DXVECTOR3 *pVector) { *pVector = m_vLookVector; }  


	VOID SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);  
	VOID SetCameraPosition(D3DXVECTOR3 *pVector = NULL); 
	VOID SetViewMatrix(D3DXMATRIX *pMatrix = NULL);  
	VOID SetProjMatrix(D3DXMATRIX *pMatrix = NULL);  

public:

	VOID MoveAlongRightVec(FLOAT fUnits);   
	VOID MoveAlongUpVec(FLOAT fUnits);      
	VOID MoveAlongLookVec(FLOAT fUnits);   


	VOID RotationRightVec(FLOAT fAngle);  
	VOID RotationUpVec(FLOAT fAngle);      
	VOID RotationLookVec(FLOAT fAngle);     

public:

	Camera(IDirect3DDevice9 *pd3dDevice); 
	virtual ~Camera(void);  

};

#endif // !CAMERA_H
