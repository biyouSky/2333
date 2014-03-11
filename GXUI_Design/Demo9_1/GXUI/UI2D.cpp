#include "stdafx.h"

#include "UI2D.h"
#include "UIRender.h"
using namespace std;


const DWORD UI2DVertex::_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
int UI2DClipRect::_count = 0;

UI2DVertex::UI2DVertex()
{
}

UI2DVertex::UI2DVertex( float x, float y, float z, D3DCOLOR rgb )
{
	_x = x;  _y = y;  _z = z;
	_rhw = 1.0f;
	_color = rgb;
}

UI2DPoint::UI2DPoint( int x, int y, float z )
{
	_x = (float)x;
	_y = (float)y;
	_z = z;
}

UI2DPoint::~UI2DPoint() 
{ 
}

void UI2DPoint::operator()( D3DCOLOR rgb )
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->SetFVF( UI2DVertex::_FVF );	

	UI2DVertex quad = UI2DVertex( _x, _y, _z, rgb );
	pIDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 1, &quad, sizeof( UI2DVertex ) );
}

UI2DLine::UI2DLine( int beginX, int beginY, int endX, int endY, float z )
{
	_beginX = (float)beginX;
	_beginY = (float)beginY;
	_endX = (float)endX;
	_endY = (float)endY;
	_z = z;
}

UI2DLine::UI2DLine( POINT beginPoint, POINT endPoint, float z )
{
	_beginX = (float)beginPoint.x;
	_beginY = (float)beginPoint.y;
	_endX = (float)endPoint.x;
	_endY = (float)endPoint.y;
	_z = z;
}

UI2DLine::~UI2DLine() 
{
}

void UI2DLine::operator()( D3DCOLOR rgbS, D3DCOLOR rgbE, UCHAR alphy )
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->SetFVF( UI2DVertex::_FVF );	

	D3DCOLOR colorS = D3DCOLOR_MIX( alphy, rgbS );
	D3DCOLOR colorE = D3DCOLOR_MIX( alphy, rgbE );

	UI2DVertex quad[2];
	quad[0] = UI2DVertex( _beginX, _beginY, _z, colorS );			// v1
	quad[1] = UI2DVertex( _endX, _endY, _z, colorE );				// v2

	pIDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	pIDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 1, quad, sizeof( UI2DVertex ) );

#if 0
	D3DXVECTOR2 points[2];
	points[0] = D3DXVECTOR2( _beginX, _beginY );
	points[1] = D3DXVECTOR2( _endX, _endY );

	ID3DXLine* pILine;
	HRESULT	hr = D3DXCreateLine( pIDevice, &pILine );
	pILine->SetPattern( 0x0000ffff );
	pILine->SetWidth( 1.f );  
    pILine->SetAntialias( 1 );										// 消除锯齿
    pILine->Draw( points, 2, rgbS );
#endif
}

void UI2DLine::operator()( D3DCOLOR rgb, UCHAR alphy ) 
{ 
	this->operator()( rgb, rgb, alphy ); 
}


UI2DRect::UI2DRect( int beginX, int beginY, ULONG width, ULONG height, float z )
{
	_beginX = (float)beginX;
	_beginY = (float)beginY;
	_width = (float)width;
	_height = (float)height;
	_z = z;
}

UI2DRect::UI2DRect( RECT& rect, float z )
{
	_beginX = (float)rect.left;
	_beginY = (float)rect.top;
	_width = (float)( rect.right-rect.left );
	_height = (float)( rect.bottom-rect.top );
	_z = z;
}
	
UI2DRect::~UI2DRect() 
{
}

void UI2DRect::operator()( D3DCOLOR rgb )
{
	if( _width==0 || _height==0 ) return;

	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->SetFVF( UI2DVertex::_FVF );

	UI2DVertex quad[5];
	quad[0] = UI2DVertex( _beginX, _beginY, _z, rgb );							// v1
	quad[1] = UI2DVertex( _beginX+_width-1, _beginY, _z, rgb );					// v2
	quad[2] = UI2DVertex( _beginX+_width-1, _beginY+_height-1, _z, rgb );		// v4
	quad[3] = UI2DVertex( _beginX, _beginY+_height-1, _z, rgb );				// v3
	quad[4] = UI2DVertex( _beginX, _beginY, _z, rgb );							// v1

	pIDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	pIDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, quad, sizeof( UI2DVertex ) );
}

void UI2DRect::operator()( D3DCOLOR rgbUp, D3DCOLOR rgbDown, UCHAR alphy )
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	pIDevice->SetFVF( UI2DVertex::_FVF );
	D3DCOLOR colorUp = D3DCOLOR_MIX( alphy, rgbUp );
	D3DCOLOR colorDown = D3DCOLOR_MIX( alphy, rgbDown );

	/*
		v1   v2
		v3   v4
	*/
	UI2DVertex quad[4];
	quad[0] = UI2DVertex( _beginX+_width, _beginY, _z, colorUp );			// v2
	quad[1] = UI2DVertex( _beginX+_width, _beginY+_height, _z, colorDown );	// v4
	quad[2] = UI2DVertex( _beginX, _beginY, _z, colorUp );					// v1
	quad[3] = UI2DVertex( _beginX, _beginY+_height, _z, colorDown );		// v3

	pIDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	pIDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, quad, sizeof( UI2DVertex ) );	
} 

void UI2DRect::operator()( D3DCOLOR rgb, UCHAR alphy ) 
{ 
	this->operator()( rgb, rgb, alphy ); 
}

UI2DImage::UI2DImage() 
{
}

UI2DImage::UI2DImage( std::string imagePath, float z, D3DCOLOR colorKey )
{
	Init( imagePath, z, colorKey );
}
	
UI2DImage::UI2DImage( std::string resDLLPath, UINT id, float z, D3DCOLOR colorKey )
{
	Init( resDLLPath, id, z, colorKey );
}

void UI2DImage::Init( std::string imagePath, float z, D3DCOLOR colorKey )
{
	_z = z;
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	// 取透明色
	_colorKey = colorKey;

	// 创建材质
	_pITex = NULL;
	HRESULT hr = D3DXCreateTextureFromFileExA(	pIDevice,
												imagePath.c_str(),
												D3DX_DEFAULT_NONPOW2, 
												D3DX_DEFAULT_NONPOW2, 
												D3DX_FROM_FILE,				// 图片的图层
												D3DPOOL_DEFAULT,			// 纹理的使用方法
												D3DFMT_UNKNOWN,				// 纹理的格式 D3DFMT_A1R5G5B5
												D3DPOOL_MANAGED, 
												D3DX_DEFAULT,				// 图像像素的过滤方式
												D3DX_DEFAULT,				// MIP的像素过滤方式
												_colorKey,					// 透明色 D3DRS_ALPHABLENDENABLE开启时有效
												&_srcInfo,					// 记录载入图片信息
												NULL,						// 记录调色板信息
												&_pITex );

	// 精灵配置
	_pISprite = NULL;
	D3DXCreateSprite( pIDevice, &_pISprite );
}

void UI2DImage::Init( std::string resDLLPath, UINT id, float z, D3DCOLOR colorKey )
{
	_z = z;
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	// 取透明色
	_colorKey = colorKey;

	// 载入DLL模块
	HMODULE hModule = ::LoadLibraryA( resDLLPath.c_str() );
	assert( hModule!=NULL ); 

	// 创建材质
	_pITex = NULL;
	HRESULT hr = D3DXCreateTextureFromResourceExA(	pIDevice,
													hModule,
													MAKEINTRESOURCEA(id),
													D3DX_DEFAULT_NONPOW2, 
													D3DX_DEFAULT_NONPOW2, 
													D3DX_FROM_FILE,				// 图片的图层
													D3DPOOL_DEFAULT,			// 纹理的使用方法
													D3DFMT_UNKNOWN,				// 纹理的格式 D3DFMT_A1R5G5B5
													D3DPOOL_MANAGED, 
													D3DX_DEFAULT,				// 图像像素的过滤方式
													D3DX_DEFAULT,				// MIP的像素过滤方式
													_colorKey,					// 透明色 D3DRS_ALPHABLENDENABLE开启时有效
													&_srcInfo,					// 记录载入图片信息
													NULL,						// 记录调色板信息
													&_pITex );

	// 精灵配置
	_pISprite = NULL;
	D3DXCreateSprite( pIDevice, &_pISprite );
}


UI2DImage::~UI2DImage()
{
	if( _pITex!=NULL ) _pITex->Release();
	if( _pISprite!=NULL ) _pISprite->Release();
}

void UI2DImage::operator()( RECT& srcRect, RECT& dstRect, UCHAR alphy )
{
	if( _pITex==NULL || _pISprite==NULL ) return;
	_pISprite->Begin( D3DXSPRITE_ALPHABLEND );

	// 计算缩放向量
	float kWidth = (float)GetWidth()( dstRect ) / GetWidth()( srcRect );
	float kHeight = (float)GetHeight()( dstRect ) / GetHeight()( srcRect );

	D3DXMATRIX mtrx;  
	D3DXMatrixTransformation2D( &mtrx,
								NULL,
								0.f,
								&D3DXVECTOR2( kWidth, kHeight ),	
								&D3DXVECTOR2( 0.f, 0.f ),
								0,												
								&D3DXVECTOR2( (float)dstRect.left, (float)dstRect.top ) );
	_pISprite->SetTransform( &mtrx ); 

	_pISprite->Draw( _pITex,
					 &srcRect,
					 NULL, 
					 &D3DXVECTOR3( 0, 0, _z ), 
					 ( alphy<<24 )|0x00FFFFFF );

	_pISprite->End();
}

void UI2DImage::operator()( RECT& srcRect, ULONG dstBeginX, ULONG dstBeginY, UCHAR alphy )
{
	RECT dstRect = GenRect()( GenPoint()( dstBeginX, dstBeginY ), GenSize()( GetWidth()( srcRect ), GetHeight()( srcRect ) ) );
	this->operator()( srcRect, dstRect, alphy );
}

void UI2DImage::operator()( RECT& dstRect, float scale, UCHAR alphy )
{
	RECT srcRect = GenRect()( GenPoint()( 0, 0 ), GenSize()( _srcInfo.Width, _srcInfo.Height ) );
	if( scale==1 )
	{
		this->operator()( srcRect, dstRect, alphy );
		return;
	}

	int dx = static_cast<int>( ( scale-1 )*GetWidth()( dstRect )/2 );
	int dy = static_cast<int>( ( scale-1 )*GetHeight()( dstRect )/2 );
	RECT dstRect2 = GenRect()( dstRect.left-dx/2, dstRect.top-dy/2, dstRect.right+dx/2, dstRect.bottom+dy/2 );
	this->operator()( srcRect, dstRect2, alphy );
}

void UI2DImage::operator()( ULONG dstCenterX, ULONG dstCenterY, float scale, UCHAR alphy )
{
	RECT srcRect = GenRect()( GenPoint()( 0, 0 ), GenSize()( _srcInfo.Width, _srcInfo.Height ) );
	RECT dstRect = GenRect()( GenPoint()( (LONG)( dstCenterX-_srcInfo.Width*scale/2 ), (LONG)( dstCenterY-_srcInfo.Height*scale/2 ) ), 
										    GenSize()( (LONG)( _srcInfo.Width*scale ), (LONG)( _srcInfo.Height*scale ) ) );
	this->operator()( srcRect, dstRect, alphy );
}


DrawFrameHelp::DrawFrameHelp( std::string imagePath, int topBarHeight, int bottomBarHeight, int leftBarWidth, int rightBarWidth, 
							  float z, D3DCOLOR colorKey ):_image( imagePath, z, colorKey )
{
	_topBarHeight = topBarHeight;
	_bottomBarHeight = bottomBarHeight;
	_leftBarWidth = leftBarWidth;
	_rightBarWidth = rightBarWidth;
}

DrawFrameHelp::DrawFrameHelp( std::string resDLLPath, UINT id, int topBarHeight, int bottomBarHeight, int leftBarWidth, int rightBarWidth, 
							  float z, D3DCOLOR colorKey ):_image( resDLLPath, id, z, colorKey )
{
	_topBarHeight = topBarHeight;
	_bottomBarHeight = bottomBarHeight;
	_leftBarWidth = leftBarWidth;
	_rightBarWidth = rightBarWidth;
}

void DrawFrameHelp::operator()( ULONG dstBeginX, ULONG dstBeginY, ULONG width, ULONG height, UCHAR alphy )
{
	UINT& imageWidth = _image._srcInfo.Width;
	UINT& imageHeight = _image._srcInfo.Height;

	RECT rc1 = GenRect()( GenPoint()( 0, 0 ), GenSize()( _leftBarWidth, _topBarHeight ) );
	_image( rc1, dstBeginX, dstBeginY, alphy );

	RECT rc3 = GenRect()( GenPoint()( imageWidth-_rightBarWidth , 0 ), GenSize()( _rightBarWidth, _topBarHeight ) );
	_image( rc3, dstBeginX+width-_rightBarWidth, dstBeginY, alphy );

	RECT rc4 = GenRect()( GenPoint()( 0, imageHeight-_bottomBarHeight ), GenSize()( _leftBarWidth, _bottomBarHeight ) );
	_image( rc4, dstBeginX, dstBeginY+height-_bottomBarHeight, alphy );

	RECT rc6 = GenRect()( GenPoint()( imageWidth-_rightBarWidth, imageHeight-_bottomBarHeight ), GenSize()( _rightBarWidth, _bottomBarHeight ) );
	_image( rc6, dstBeginX+width-_rightBarWidth, dstBeginY+height-_bottomBarHeight, alphy );

	RECT rc2 = GenRect()( GenPoint()( _leftBarWidth, 0 ) , GenSize()( imageWidth-_leftBarWidth-_rightBarWidth, _topBarHeight ) );
	RECT rc2d = GenRect()( GenPoint()( dstBeginX+_leftBarWidth, dstBeginY ), GenSize()( width-_leftBarWidth-_rightBarWidth, _topBarHeight ) );
	_image( rc2, rc2d, alphy );

	RECT rc5 = GenRect()( GenPoint()( _leftBarWidth, imageHeight-_bottomBarHeight ), GenSize()( imageWidth-_leftBarWidth-_rightBarWidth, _bottomBarHeight ) );
	RECT rc5d = GenRect()( GenPoint()( dstBeginX+_leftBarWidth, dstBeginY+height-_bottomBarHeight ), GenSize()( width-_leftBarWidth-_rightBarWidth, _bottomBarHeight ) );
	_image( rc5, rc5d, alphy );

	RECT rc7 = GenRect()( GenPoint()( 0, _topBarHeight ), GenSize()( _leftBarWidth, imageHeight-_topBarHeight-_bottomBarHeight ) );
	RECT rc7d = GenRect()( GenPoint()( dstBeginX, dstBeginY+_topBarHeight ), GenSize()( _leftBarWidth, height-_topBarHeight-_bottomBarHeight ) );
	_image( rc7, rc7d, alphy );

	RECT rc8 = GenRect()( GenPoint()( imageWidth-_rightBarWidth, _topBarHeight ), GenSize()( _rightBarWidth, imageHeight-_topBarHeight-_bottomBarHeight ) );
	RECT rc8d = GenRect()( GenPoint()( dstBeginX+width-_rightBarWidth, dstBeginY+_topBarHeight ), GenSize()( _rightBarWidth, height-_topBarHeight-_bottomBarHeight ) );
	_image( rc8, rc8d, alphy );
	
	RECT rc9 = GenRect()( GenPoint()( _leftBarWidth, _topBarHeight ), GenSize()( imageWidth-_leftBarWidth-_rightBarWidth, imageHeight-_topBarHeight-_bottomBarHeight ) );
	RECT rc9d = GenRect()( GenPoint()( dstBeginX+_leftBarWidth, dstBeginY+_topBarHeight ), GenSize()( width-_leftBarWidth-_rightBarWidth, height-_topBarHeight-_bottomBarHeight ) );
	_image( rc9, rc9d, alphy );
}
	
void DrawFrameHelp::operator()( RECT& dstRC, UCHAR alphy )
{
	this->operator()( dstRC.left, dstRC.top, GetWidth()( dstRC ), GetHeight()( dstRC ), alphy );
}


void UI2DClipRect::BeginClipRect( RECT& clipRC )
{	
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, 1 );

	++UI2DClipRect::_count;

	RECT crossClipRC= clipRC;

	// 获得上次的clip矩形 若存在则计算相交矩形
	pIDevice->GetScissorRect( &_preClipRC );
	if( CompareRect()( _preClipRC, _NULLRECT_ )==false )
	{	// 计算相交矩形
		if( CrossRect()( _preClipRC, clipRC, crossClipRC )==false )
			crossClipRC = GenRect()( 0, 0, 1, 1 );															// ??? 应该不绘制
	}

	pIDevice->SetScissorRect( &crossClipRC );
}

void UI2DClipRect::EndClipRect()
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->SetScissorRect( &_preClipRC );

	--UI2DClipRect::_count;

	if( UI2DClipRect::_count==0 )
		pIDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, 0 );
}


UI2DFont::UI2DFont( float z, LONG height, LONG weight, tstring name ) 
{ 
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	_z = z;

	// 字体信息
	memset( &_font, 0, sizeof( D3DXFONT_DESC ) );
	_font.Height = height;									// in logical units
	_font.Width = 0;										// boldness, range 0(light) - 1000(bold)
	_font.Weight = weight;
	_font.MipLevels = D3DX_DEFAULT;
	_font.Italic = FALSE;
	_font.CharSet = DEFAULT_CHARSET;
	_font.OutputPrecision = OUT_OUTLINE_PRECIS;// OUT_DEFAULT_PRECIS;
	_font.Quality = CLEARTYPE_QUALITY;// DEFAULT_QUALITY;
	_font.PitchAndFamily =  DEFAULT_PITCH | FF_SWISS;
	memcpy( _font.FaceName, name.c_str(), name.size()*sizeof( TCHAR ) );

	// 创建字体接口
	_pIFont = NULL;
	HRESULT rt = D3DXCreateFontIndirect( pIDevice, &_font, &_pIFont );
	assert( rt == S_OK );

	// 创建精灵接口
	_pISprite = NULL;
	rt = D3DXCreateSprite( pIDevice, &_pISprite );
	assert( rt == S_OK );	
}

UI2DFont::~UI2DFont()
{		
	if( _pISprite!=NULL ) _pISprite->Release();
	if( _pIFont!=NULL ) _pIFont->Release();
}

SIZE UI2DFont::GetDrawAreaSize( std::wstring str )
{
	if( str.size()==0 )
	{
		return GenSize()( 0, 0 );
	}
	else if( str.back()!=L' ' )
	{
		RECT rc = _NULLRECT_;
		_pIFont->DrawTextW( NULL, str.c_str(), str.size(), &rc, DT_CALCRECT, _BLACK_ );
		return GenSize()( GetWidth()( rc ), GetHeight()( rc ) );
	}
	else
	{
		RECT rc1 = _NULLRECT_;
		_pIFont->DrawTextW( NULL, ( str+L"1" ).c_str(), str.size()+1, &rc1, DT_CALCRECT, _BLACK_ );

		RECT rc2 = _NULLRECT_;
		_pIFont->DrawTextW( NULL, ( str+L"11" ).c_str(), str.size()+2, &rc2, DT_CALCRECT, _BLACK_ );

		return GenSize()( 2*GetWidth()( rc1 )-GetWidth()( rc2 ), GetHeight()( rc1 ) );
	}
}

SIZE UI2DFont::GetDrawAreaSize( std::string str )
{
	if( str.size()==0 )
	{
		return GenSize()( 0, 0 );
	}
	else if( str.back()!=' ' )
	{
		RECT rc = _NULLRECT_;
		_pIFont->DrawTextA( NULL, str.c_str(), str.size(), &rc, DT_CALCRECT, _BLACK_ );
		return GenSize()( GetWidth()( rc ), GetHeight()( rc ) );
	}
	else
	{
		RECT rc1 = _NULLRECT_;
		_pIFont->DrawTextA( NULL, ( str+"1" ).c_str(), str.size()+1, &rc1, DT_CALCRECT, _BLACK_ );

		RECT rc2 = _NULLRECT_;
		_pIFont->DrawTextA( NULL, ( str+"11" ).c_str(), str.size()+2, &rc2, DT_CALCRECT, _BLACK_ );

		return GenSize()( 2*GetWidth()( rc1 )-GetWidth()( rc2 ), GetHeight()( rc1 ) );
	}
}

void UI2DFont::operator()( std::string str, RECT& rc, DWORD pos, D3DCOLOR color )
{
	_pISprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	D3DXMATRIX matrix;
	D3DXMatrixIdentity( &matrix );
	D3DXMatrixTranslation( &matrix, 0.0f, 0.0f, _z );
	_pISprite->SetTransform( &matrix );
	_pIFont->DrawTextA( _pISprite, str.c_str(), str.size(), &rc, pos, color );

	_pISprite->End();
}

void UI2DFont::operator()( std::wstring str, RECT& rc, DWORD pos, D3DCOLOR color )
{
	_pISprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	D3DXMATRIX matrix;
	D3DXMatrixIdentity( &matrix );
	D3DXMatrixTranslation( &matrix, 0.0f, 0.0f, _z );
	_pISprite->SetTransform( &matrix );
	_pIFont->DrawTextW( _pISprite, str.c_str(), str.size(), &rc, pos, color );
		
	_pISprite->End();
}
