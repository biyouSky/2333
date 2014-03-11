
#pragma once

#include "UIUtil.h"


// 顶点类型
struct UI2DVertex
{
	UI2DVertex();
	UI2DVertex( float x, float y, float z, D3DCOLOR rgb=D3DCOLOR_XRGB(255, 0, 0) );

	static const DWORD _FVF;

	float _x, _y, _z, _rhw;
	D3DCOLOR _color;
};




// 画点
struct UI2DPoint
{
	UI2DPoint( int x, int y, float z=0.5 );
	~UI2DPoint();

	void operator()( D3DCOLOR rgb );

	float _x, _y, _z;
};


// 直线  缺失： 虚线 线宽支持
struct UI2DLine
{
	UI2DLine( int beginX, int beginY, int endX, int endY, float z=0.5f );
	UI2DLine( POINT beginPoint, POINT endPoint, float z=0.5f );
	~UI2DLine();

	void operator()( D3DCOLOR rgbS, D3DCOLOR rgbE, UCHAR alphy );			// 颜色渐变线 只支持实线方式
	void operator()( D3DCOLOR rgb, UCHAR alphy=255 );						// 单色直线

	float _beginX, _beginY, _endX, _endY, _z;
};


// 矩形
struct UI2DRect
{
	UI2DRect( int beginX, int beginY, ULONG width, ULONG height, float z=0.5f );
	UI2DRect( RECT& rect, float z=0.5f );
	~UI2DRect();
	
	void operator()( D3DCOLOR rgb );									// 绘制边框矩形
	void operator()( D3DCOLOR rgbUp, D3DCOLOR rgbDown, UCHAR alphy );	// 画矩形 上下渐变色实体矩形
	void operator()( D3DCOLOR rgb, UCHAR alphy );						// 画矩形 单色实体矩形

	float _beginX, _beginY, _width, _height, _z;
};


// 图片支持
/*
	图片编入dll中方法
		1. 如果图是BMP格式, 可以直接导入，会出现在资源管理器的"Bitmap"类型下面;
		2. 如果是其它格式, 先导入, 类型写RCDATA。
		   然后打开.rc文件, 把类型修改成"RCDATA DISCARDABLE"。
		   例如： IDR_JPG1    RCDATA DISCARDABLE    "texture.JPG"
*/
struct UI2DImage
{
	UI2DImage();
	UI2DImage( std::string imagePath, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );				// 绘制图片文件
	UI2DImage( std::string resDLLPath, UINT id, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );	// 从dll中获取图片数据 并绘制
	~UI2DImage();

	void Init( std::string imagePath, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void Init( std::string resDLLPath, UINT id, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );

	// 
	void operator()( RECT& srcRect, RECT& dstRect, UCHAR alphy=255 );	
	void operator()( RECT& srcRect, ULONG dstBeginX, ULONG dstBeginY, UCHAR alphy=255 );
	void operator()( RECT& dstRect, float scale=1.f, UCHAR alphy=255 );
	void operator()( ULONG dstCenterX, ULONG dstCenterY, float scale=1.f, UCHAR alphy=255 );

	// 纹理信息
	IDirect3DTexture9* _pITex;
	D3DXIMAGE_INFO _srcInfo;
	D3DCOLOR _colorKey;

	// 2D精灵
	ID3DXSprite* _pISprite;

	// 
	float _z;
};


/*
	框架背景图绘制助手类：
	---------------------------------
	| 1 |			2			| 3 |
	|--------------------------------
	|	|						|	|
	|	|						|	|
	|	|						|	|
	| 7	|			9			| 8	|
	|	|						|	|
	|	|						|	|
	|	|						|	|
	|--------------------------------
	| 4	|			5			| 6 |									
	---------------------------------
	绘制时分为9个区域
	序号	高										宽
	1		topBarHeight							leftBarWidth
	2		topBarHeight							width-leftBarWidth-rightBarWidth
	3		topBarHeight							rightBarWidth
	4		bottomBarHeight							leftBarWidth
	5		bottomBarHeight							width-leftBarWidth-rightBarWidth
	6		bottomBarHeight							rightBarWidth
	7		height-topBarHeight-bottomBarHeight		leftBarWidth
	8		height-topBarHeight-bottomBarHeight		rightBarWidth
	9		height-topBarHeight-bottomBarHeight		width-leftBarWidth-rightBarWidth
*/
class DrawFrameHelp
{
public:
	DrawFrameHelp( std::string imagePath, int topBarHeight, int bottomBarHeight, int leftBarWidth, int rightBarWidth, 
				   float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	DrawFrameHelp( std::string resDLLPath, UINT id, int topBarHeight, int leftBarWidth, int rightBarWidth, int leftRightBarWidth, 
				   float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );

	void operator()( ULONG dstBeginX, ULONG dstBeginY, ULONG width, ULONG height, UCHAR alphy=255 );
	void operator()( RECT& dstRC, UCHAR alphy=255 );

private:
	UI2DImage _image;

	int _topBarHeight;
	int _bottomBarHeight;
	int _leftBarWidth;
	int _rightBarWidth;
	int _alphaValue;
};


// 2D区域裁剪支持
struct UI2DClipRect
{
	void BeginClipRect( RECT& clipRC );
	void EndClipRect();

	static int _count;	
	RECT _preClipRC;
};


// 字体支持
class UI2DFont
{
public:
	UI2DFont( float z=0.5f, LONG height=20, LONG weight=400, tstring name=_T("Times New Roman") );
	~UI2DFont();

	SIZE GetDrawAreaSize( std::wstring str );
	SIZE GetDrawAreaSize( std::string str );

	void operator()( std::string str, RECT& rc, DWORD pos=FONT_CENTER, D3DCOLOR color=_BLACK_ );
	void operator()( std::wstring str, RECT& rc, DWORD pos=FONT_CENTER, D3DCOLOR color=_BLACK_ );

	enum FontPos
	{
		FONT_LEFT = DT_LEFT|DT_VCENTER,
		FONT_RIGHT = DT_RIGHT|DT_VCENTER,
		FONT_CENTER = DT_CENTER|DT_VCENTER
	};

private:
	ID3DXSprite* _pISprite;
	ID3DXFont* _pIFont;

	D3DXFONT_DESC  _font;	
	float _z;
};