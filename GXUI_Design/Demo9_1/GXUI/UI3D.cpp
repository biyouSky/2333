#include "stdafx.h"

#include "UI3D.h"
#include "UIRender.h"
using namespace std;


UICamera* UICamera::_pSingleton = NULL;
const DWORD UI3DCubeTest::_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

UICamera* UICamera::GetSingleton()
{
	if( _pSingleton==NULL )  
		_pSingleton = new UICamera;

	return _pSingleton; 
}

void UICamera::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

UICamera::UICamera()
{
}

UICamera::~UICamera()
{
}

UICamera::CameraMath::CameraMath()
{
	_pos   = D3DXVECTOR3( 0.0f, 0.0f, -5.f );
	_right = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	_up    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	_look  = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
}

UICamera::CameraMath::~CameraMath() 
{
}

void UICamera::CameraMath::strafe( float units ) 
{ 
	_pos += _right*units; 
}		

void UICamera::CameraMath::fly( float units ) 
{ 
	_pos += _up*units; 
}			

void UICamera::CameraMath::walk( float units ) 
{ 
	_pos += _look*units; 
}		

void UICamera::CameraMath::pitch( float angle )									// rotate on _right vector
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &_right, angle );

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord( &_up, &_up, &T );
	D3DXVec3TransformCoord( &_look, &_look, &T );
}
void UICamera::CameraMath::yaw( float angle )										// rotate on _up vector
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &_up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord( &_right,&_right, &T );
	D3DXVec3TransformCoord( &_look,&_look, &T );
}
void UICamera::CameraMath::roll( float angle )									// rotate on _look vector
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis( &T, &_look, angle);

	// rotate _up and _right around _look vector
	D3DXVec3TransformCoord( &_right,&_right, &T );
	D3DXVec3TransformCoord( &_up,&_up, &T );
}

void UICamera::CameraMath::GetViewMatrix( D3DXMATRIX* pViewMatrix )
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize( &_look, &_look );

	D3DXVec3Cross( &_right, &_up, &_look );
	D3DXVec3Normalize( &_right, &_right );

	D3DXVec3Cross( &_up, &_look, &_right );
	D3DXVec3Normalize( &_up, &_up );

	// Build the view matrix:
	float x = -D3DXVec3Dot( &_right, &_pos );
	float y = -D3DXVec3Dot( &_up, &_pos );
	float z = -D3DXVec3Dot( &_look, &_pos );

	(*pViewMatrix)(0,0) = _right.x; (*pViewMatrix)(0, 1) = _up.x; (*pViewMatrix)(0, 2) = _look.x; (*pViewMatrix)(0, 3) = 0.0f;
	(*pViewMatrix)(1,0) = _right.y; (*pViewMatrix)(1, 1) = _up.y; (*pViewMatrix)(1, 2) = _look.y; (*pViewMatrix)(1, 3) = 0.0f;
	(*pViewMatrix)(2,0) = _right.z; (*pViewMatrix)(2, 1) = _up.z; (*pViewMatrix)(2, 2) = _look.z; (*pViewMatrix)(2, 3) = 0.0f;
	(*pViewMatrix)(3,0) = x;        (*pViewMatrix)(3, 1) = y;     (*pViewMatrix)(3, 2) = z;       (*pViewMatrix)(3, 3) = 1.0f;
}

bool UICamera::SetCameraView()
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	D3DXMATRIX v;
	_camerMath.GetViewMatrix( &v );
	pIDevice->SetTransform( D3DTS_VIEW, &v );
	return true;
}

void UICamera::SetCameraFixed( D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 up )
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	_camerMath._pos = pos;
	_camerMath._look = look;
	_camerMath._up = up;

	D3DXMATRIX v;
	D3DXMatrixLookAtLH( &v, &_camerMath._pos, &_camerMath._look, &_camerMath._up );
	pIDevice->SetTransform( D3DTS_VIEW, &v );
}

// ����ͶӰ���� ���ÿ������� ӳ�䵽����������    ע�⣺ͶӰ��Ϊz=1
// ��Ϊ�Ƕ�Ϊ��ֱ����ĽǶ� ���zn��zf������� �����ݿ�߱ȼ������ �����������弴��ȷ��
// ע�⣺���ۿ����������Ĵ�СΪ���� ����ӳ�䵽���������� ���Ϊ�˲���������Ч�� ��߱�����뻺����һ��
/*   
				/|
			   / |
			  /  |
			 /|  |
			/ |  |
		   /  |  |
		  /   |  |
		 /    |  |
Camera ./_____|__|
		|-zn- |
		| - zf - | 
*/
bool UICamera::SetPerspective( float aspect, float angle, float zn, float zf )
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( 
			&proj,
			angle,					// ��ֱ�����ϽǶ�( һ��Ϊy�᷽�� )
			aspect,					// ��߱�
			zn,						// ǰ�ü��������������
			zf );					// ��ü��������������
	pIDevice->SetTransform( D3DTS_PROJECTION, &proj );
	return true;
}