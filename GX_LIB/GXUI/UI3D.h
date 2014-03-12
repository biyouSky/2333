
#pragma once

#include "UIUtil.h"


class UICamera
{
public:
	static UICamera* GetSingleton();
	static void Destroy();
	static void Assign( UICamera* pPara );

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

	UI3DCubeTest();
	~UI3DCubeTest();

	bool Init();
	bool Draw();

	static const DWORD _FVF;

	IDirect3DVertexBuffer9* vBuf;
	IDirect3DIndexBuffer9*  iBuf;
};