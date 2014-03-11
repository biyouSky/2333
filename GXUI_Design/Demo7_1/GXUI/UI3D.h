
#pragma once

#include "UIUtil.h"


class UICamera
{
public:
	static UICamera* GetSingleton();
	static void Destroy();

	bool SetCameraView();																		// 设置摄像头视角
	void SetCameraFixed( D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 up );					// 设置固定的摄像头
	
	bool SetPerspective( float aspect, float angle=D3DX_PI/2.f, float zn=1, float zf=1000 );	// 设置投影柱体

	struct CameraMath
	{
		CameraMath();
		~CameraMath();

		void strafe( float units );						// left/_right
		void fly( float units );						// _up/down
		void walk( float units );						// forward/backward
		void pitch( float angle );						// rotate on _right vector
		void yaw( float angle );						// rotate on _up vector
		void roll( float angle );						// rotate on _look vector

		void GetViewMatrix( D3DXMATRIX* pViewMatrix );

		D3DXVECTOR3 _pos;
		D3DXVECTOR3 _right;
		D3DXVECTOR3 _up;
		D3DXVECTOR3 _look;
	};
	CameraMath _camerMath;																		// 设置摄像头视角// 摄像机位置信息

private:
	// 构造 析构
	UICamera();
	~UICamera();

	static UICamera* _pSingleton;
};




struct UI3DCubeTest
{
	struct Vertex
	{
		Vertex(){}
		Vertex( float x, float y, float z, D3DCOLOR rgb=D3DCOLOR_ARGB(125, 255, 255, 255) )
		{
			_x = x;  _y = y;  _z = z;
			_color = rgb;
		}
		float _x, _y, _z;
		D3DCOLOR _color;
	};

	UI3DCubeTest()
	{
		vBuf = NULL;
		iBuf = NULL;

		Init();
	}
	~UI3DCubeTest()
	{
		if( vBuf!=NULL ) vBuf->Release();
		if( iBuf!=NULL ) iBuf->Release();
	}

	bool Init()
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

	bool Draw()
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

	static const DWORD _FVF;

	IDirect3DVertexBuffer9* vBuf;
	IDirect3DIndexBuffer9*  iBuf;
};