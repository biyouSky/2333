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

void UICamera::Assign( UICamera* pPara )
{
	_pSingleton = pPara;
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

// 设置投影矩阵 设置可视柱体 映射到整个缓存面    注意：投影面为z=1
// 因为角度为垂直方向的角度 结合zn和zf计算出高 最后根据宽高比计算出宽 整个可视柱体即可确定
// 注意：不论可视柱体计算的大小为多少 都会映射到整个缓存面 因此为了不产生伸缩效果 宽高比最好与缓存面一致
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
			angle,					// 竖直方向上角度( 一般为y轴方向 )
			aspect,					// 宽高比
			zn,						// 前裁减面离摄像机距离
			zf );					// 后裁减面离摄像机距离
	pIDevice->SetTransform( D3DTS_PROJECTION, &proj );
	return true;
}











UI3DCubeTest::UI3DCubeTest()
{
	vBuf = NULL;
	iBuf = NULL;

	Init();
}
UI3DCubeTest::~UI3DCubeTest()
{
	if( vBuf!=NULL ) vBuf->Release();
	if( iBuf!=NULL ) iBuf->Release();
}

bool UI3DCubeTest::Init()
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	// 计算并初始化顶点
	pIDevice->CreateVertexBuffer(	8 * sizeof(Vertex), 
									D3DUSAGE_WRITEONLY,
									_FVF,
									D3DPOOL_MANAGED,
									&vBuf,
									0 );
	//
	Vertex* vertices;
	vBuf->Lock( 0, 0, (void**)&vertices, 0 );
	vertices[0] = Vertex( -1.0f, -1.0f, -1.0f, D3DCOLOR_ARGB(125, 255, 0, 0) );
	vertices[1] = Vertex( -1.0f,  1.0f, -1.0f, D3DCOLOR_ARGB(125, 0, 255, 0) );
	vertices[2] = Vertex(  1.0f,  1.0f, -1.0f, D3DCOLOR_ARGB(125, 0, 0, 255) );
	vertices[3] = Vertex(  1.0f, -1.0f, -1.0f, D3DCOLOR_ARGB(125, 255, 0, 0) );
	vertices[4] = Vertex( -1.0f, -1.0f,  1.0f, D3DCOLOR_ARGB(125, 255, 0, 0) );
	vertices[5] = Vertex( -1.0f,  1.0f,  1.0f, D3DCOLOR_ARGB(125, 255, 0, 0) );
	vertices[6] = Vertex(  1.0f,  1.0f,  1.0f, D3DCOLOR_ARGB(125, 255, 0, 0) );
	vertices[7] = Vertex(  1.0f, -1.0f,  1.0f, D3DCOLOR_ARGB(125, 255, 0, 0) );
	vBuf->Unlock();

	// 计算并初始化索引
	pIDevice->CreateIndexBuffer(	36 * sizeof(WORD),
									D3DUSAGE_WRITEONLY,
									D3DFMT_INDEX16,
									D3DPOOL_MANAGED,
									&iBuf,
									0 );
	//
	WORD* indices = 0;
	iBuf->Lock( 0, 0, (void**)&indices, 0 );		
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;	// front side
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;		
	indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;	// back side
	indices[9]  = 4; indices[10] = 7; indices[11] = 6;		
	indices[12] = 4; indices[13] = 5; indices[14] = 1;	// left side
	indices[15] = 4; indices[16] = 1; indices[17] = 0;		
	indices[18] = 3; indices[19] = 2; indices[20] = 6;	// _right side
	indices[21] = 3; indices[22] = 6; indices[23] = 7;
	indices[24] = 1; indices[25] = 5; indices[26] = 6;	// top
	indices[27] = 1; indices[28] = 6; indices[29] = 2;
	indices[30] = 4; indices[31] = 0; indices[32] = 3;	// bottom
	indices[33] = 4; indices[34] = 3; indices[35] = 7;
	iBuf->Unlock();	

	// 绘制效果
	pIDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pIDevice->SetRenderState( D3DRS_LIGHTING, false );

	// 开启颜色alpha透明
	pIDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	pIDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pIDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return true;
}

bool UI3DCubeTest::Draw()
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	// 旋转计算
	D3DXMATRIX rx, ry;
	// 绕x轴旋转
	D3DXMatrixRotationX( &rx, D3DX_PI/3.f );
	// 绕y轴旋转
	static float y = 0.0f;
	D3DXMatrixRotationY( &ry, y );
	y += 0.1f;
	if( y >= 6.28f )
		y = 0.0f;
	// 设置箱子对应的世界坐标系
	D3DXMATRIX p = rx * ry;
	pIDevice->SetTransform( D3DTS_WORLD, &p );

	// 绘制
	pIDevice->SetStreamSource( 0, vBuf, 0, sizeof( Vertex ) );
	pIDevice->SetIndices( iBuf );
	pIDevice->SetFVF( _FVF );

	pIDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	pIDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12 );

	return true;
}