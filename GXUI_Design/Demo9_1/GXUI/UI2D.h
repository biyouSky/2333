
#pragma once

#include "UIUtil.h"


// ��������
struct UI2DVertex
{
	UI2DVertex();
	UI2DVertex( float x, float y, float z, D3DCOLOR rgb=D3DCOLOR_XRGB(255, 0, 0) );

	static const DWORD _FVF;

	float _x, _y, _z, _rhw;
	D3DCOLOR _color;
};




// ����
struct UI2DPoint
{
	UI2DPoint( int x, int y, float z=0.5 );
	~UI2DPoint();

	void operator()( D3DCOLOR rgb );

	float _x, _y, _z;
};


// ֱ��  ȱʧ�� ���� �߿�֧��
struct UI2DLine
{
	UI2DLine( int beginX, int beginY, int endX, int endY, float z=0.5f );
	UI2DLine( POINT beginPoint, POINT endPoint, float z=0.5f );
	~UI2DLine();

	void operator()( D3DCOLOR rgbS, D3DCOLOR rgbE, UCHAR alphy );			// ��ɫ������ ֻ֧��ʵ�߷�ʽ
	void operator()( D3DCOLOR rgb, UCHAR alphy=255 );						// ��ɫֱ��

	float _beginX, _beginY, _endX, _endY, _z;
};


// ����
struct UI2DRect
{
	UI2DRect( int beginX, int beginY, ULONG width, ULONG height, float z=0.5f );
	UI2DRect( RECT& rect, float z=0.5f );
	~UI2DRect();
	
	void operator()( D3DCOLOR rgb );									// ���Ʊ߿����
	void operator()( D3DCOLOR rgbUp, D3DCOLOR rgbDown, UCHAR alphy );	// ������ ���½���ɫʵ�����
	void operator()( D3DCOLOR rgb, UCHAR alphy );						// ������ ��ɫʵ�����

	float _beginX, _beginY, _width, _height, _z;
};


// ͼƬ֧��
/*
	ͼƬ����dll�з���
		1. ���ͼ��BMP��ʽ, ����ֱ�ӵ��룬���������Դ��������"Bitmap"��������;
		2. �����������ʽ, �ȵ���, ����дRCDATA��
		   Ȼ���.rc�ļ�, �������޸ĳ�"RCDATA DISCARDABLE"��
		   ���磺 IDR_JPG1    RCDATA DISCARDABLE    "texture.JPG"
*/
struct UI2DImage
{
	UI2DImage();
	UI2DImage( std::string imagePath, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );				// ����ͼƬ�ļ�
	UI2DImage( std::string resDLLPath, UINT id, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );	// ��dll�л�ȡͼƬ���� ������
	~UI2DImage();

	void Init( std::string imagePath, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void Init( std::string resDLLPath, UINT id, float z=0.5f, D3DCOLOR colorKey=D3DFMT_UNKNOWN );

	// 
	void operator()( RECT& srcRect, RECT& dstRect, UCHAR alphy=255 );	
	void operator()( RECT& srcRect, ULONG dstBeginX, ULONG dstBeginY, UCHAR alphy=255 );
	void operator()( RECT& dstRect, float scale=1.f, UCHAR alphy=255 );
	void operator()( ULONG dstCenterX, ULONG dstCenterY, float scale=1.f, UCHAR alphy=255 );

	// ������Ϣ
	IDirect3DTexture9* _pITex;
	D3DXIMAGE_INFO _srcInfo;
	D3DCOLOR _colorKey;

	// 2D����
	ID3DXSprite* _pISprite;

	// 
	float _z;
};


/*
	��ܱ���ͼ���������ࣺ
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
	����ʱ��Ϊ9������
	���	��										��
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


// 2D����ü�֧��
struct UI2DClipRect
{
	void BeginClipRect( RECT& clipRC );
	void EndClipRect();

	static int _count;	
	RECT _preClipRC;
};


// ����֧��
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