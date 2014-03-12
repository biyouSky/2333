#include "stdafx.h"

#include "UIRender.h"
#include "UI2D.h"
#include "UI3D.h"
#include "UIWinShell.h"
using namespace std;


UIDXInit* UIDXInit::_pSingleton = NULL;
UIRender* UIRender::_pSingleton = NULL;
UIAnimateManage* UIAnimateManage::_pSingleton = NULL;
UICaret* UICaret::_pSingleton = NULL;

UIDXInit::UIDXInit() 
{ 
	_pIDevice = NULL; 
	memset( &_presentParam, 0, sizeof( _presentParam ) );
}

UIDXInit::~UIDXInit()
{
	if( _pIDevice!=NULL ) 
		_pIDevice->Release();
}

UIDXInit* UIDXInit::GetSingleton()
{
	if( _pSingleton==NULL )  
		_pSingleton = new UIDXInit;

	return _pSingleton; 
}

void UIDXInit::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

void UIDXInit::Assign( UIDXInit* pPara )
{
	_pSingleton = pPara;
}


bool UIDXInit::InitD3D(	HWND bindHWnd,									// [in] 绑定的win32窗口句柄
						ULONG width, ULONG height,						// [in] Backbuffer dimensions.
						bool windowed,									// [in] Windowed ( true )or full screen ( false ).
						D3DDEVTYPE deviceType )							// [in] HAL or REF
{
	// Step 1: 创建IDirect3D9接口对象.
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	//
	if( !d3d9 )
	{
		::MessageBoxA( 0, "Direct3DCreate9() - FAILED", "Warning", 0 );
		return false;
	}

	// Step 2: 检查硬件顶点能力
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps( D3DADAPTER_DEFAULT, deviceType, &caps );
	//
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: 填充D3DPRESENT_PARAMETERS显示结构体.
	_presentParam.BackBufferWidth            = width;
	_presentParam.BackBufferHeight           = height;
	_presentParam.BackBufferFormat           = D3DFMT_A8R8G8B8;
	_presentParam.BackBufferCount            = 1;								// 缓存屏幕数量
	_presentParam.MultiSampleType            = D3DMULTISAMPLE_NONE;				// 全屏抗锯齿的类型
	_presentParam.MultiSampleQuality         = 0;								// 全屏抗锯齿的质量等级
	_presentParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD;			// 指定表面在交换链中是如何被交换
	_presentParam.hDeviceWindow              = bindHWnd;
	_presentParam.Windowed                   = windowed;
	_presentParam.EnableAutoDepthStencil     = true;							// 设为true，D3D将自动创建深度/模版缓冲
	_presentParam.AutoDepthStencilFormat     = D3DFMT_D24S8;					// 32位深度/模版缓存,其中24位为深度缓存,8位为模版缓存
	_presentParam.Flags                      = 0;
	_presentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// 刷新率
	_presentParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	

	// Step 4: 创建显示设备接口.
	HRESULT	hr = d3d9->CreateDevice( D3DADAPTER_DEFAULT,						// primary adapter
									 deviceType,								// device type
									 bindHWnd,									// window associated with device
									 vp,										// vertex processing
									 &_presentParam,							// present parameters
									 &_pIDevice );								// return created device
	if( FAILED( hr ) )
	{
		// try again using a 16-bit depth buffer
		_presentParam.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice( 
			D3DADAPTER_DEFAULT,
			deviceType,
			bindHWnd,
			vp,
			&_presentParam,
			&_pIDevice );

		if( FAILED( hr ) )
		{
			d3d9->Release();													// done with d3d9 object
			::MessageBoxA( 0, "CreateDevice() - FAILED", "Warning", 0 );
			return false;
		}
	}

	// step 5: 释放d3d接口，其它初始化
	d3d9->Release();															// done with d3d9 object
	return true;
}

bool UIDXInit::ResetD3D( ULONG width, ULONG height )
{
	if( _pIDevice==NULL ) return false;

	if( width==0 ) width=1;
	if( height==0 ) height=1;

	_presentParam.BackBufferWidth = width;
	_presentParam.BackBufferHeight = height;
	return _pIDevice->Reset( &_presentParam )==S_OK;
}

ULONG UIDXInit::GetPresentWidth() 
{ 
	return _presentParam.BackBufferWidth;  
}

ULONG UIDXInit::GetPresentHeight() 
{ 
	return _presentParam.BackBufferHeight;  
}

HWND UIDXInit::GetBindHWnd()
{
	return UIDXInit::GetSingleton()->_presentParam.hDeviceWindow;
}



UIRender::UIRender()
{
	InitRenderEffect();
}

UIRender::~UIRender()
{
}

UIRender* UIRender::GetSingleton()
{
	if( _pSingleton==NULL ) 
		_pSingleton = new UIRender;

	return _pSingleton; 
}

void UIRender::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

void UIRender::Assign( UIRender* pPara )
{
	_pSingleton = pPara;
}

void UIRender::InitRenderEffect()
{
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;

	// 关闭灯光
	pIDevice->SetRenderState( D3DRS_LIGHTING, false );

	// 开启颜色alpha透明
	pIDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	pIDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pIDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// 高斯光栅 会产生颜色渐变效果
	pIDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );

	// 不采用反面拣选
	pIDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// 确定可视柱体
	// 设置摄像头位置
	UICamera::GetSingleton()->SetCameraView();
	// 设置剪裁面宽高比、摄像头上下视角、前后剪裁面与摄像头距离
	UICamera::GetSingleton()->SetPerspective( (float)( UIDXInit::GetSingleton()->GetPresentWidth() )/UIDXInit::GetSingleton()->GetPresentHeight() );
}

bool UIRender::AutoAdjustSizeChange( ULONG width, ULONG height )
{
	UIDXInit::GetSingleton()->ResetD3D( width, height );			// 重新设置缓存面的大小
	InitRenderEffect();												// 重新初始化显示效果
	return true;
}

void UIRender::SaveUIImage( std::string imagePath )
{
	LPDIRECT3DSURFACE9 pFrontSurface = NULL; 
	UIDXInit::GetSingleton()->_pIDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pFrontSurface ); 

	D3DXSaveSurfaceToFileA( imagePath.c_str(), D3DXIFF_PNG, pFrontSurface, NULL, NULL ); 

	pFrontSurface->Release(); 
}

void UIRender::Render()
{
	// 窗口最小化不进行render
	HWND hWnd = UIDXInit::GetSingleton()->GetBindHWnd();
	if( hWnd!=0 )
	{
		if( IsIconic( hWnd )!=0 ) 
			return;
	}
		
	// 清除缓存表面
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER| D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0 );
	
	pIDevice->BeginScene();

	// 顶层容器绘制子窗口
	UIWinShell::_pWinShell->_uiTopContainer.Draw();

	// 动画主动绘制
	UIAnimateManage::GetSingleton()->DrawAutoList();

	//static UI3DCubeTest cube;
	//cube.Draw();

	pIDevice->EndScene();

	// Swap the back and front buffers.
	HRESULT hr = pIDevice->Present( 0, 0, 0, 0 );
}



UIAnimateManage* UIAnimateManage::GetSingleton()
{
	if( _pSingleton==NULL )  
		_pSingleton = new UIAnimateManage;

	return _pSingleton; 
}

void UIAnimateManage::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

void UIAnimateManage::Assign( UIAnimateManage* pPara )
{
	_pSingleton = pPara;

	UICaret::GetSingleton()->Assign( (UICaret*)pPara->_animateList.front() );
}

UIAnimateManage::UIAnimateManage()
{
	AddAnimate( UICaret::GetSingleton() );
}

UIAnimateManage::~UIAnimateManage()
{
	UICaret::GetSingleton()->Destroy();
}

void UIAnimateManage::AddAnimate( UIAnimateBase* pAnimateObj )
{
	if( pAnimateObj!=NULL )
	{
		if( find( _animateList.begin(), _animateList.end(), pAnimateObj )==_animateList.end() )
			_animateList.push_back( pAnimateObj );
	}
}

bool UIAnimateManage::HandleTimer()
{	
	for( auto i=_animateList.begin(); i!=_animateList.end(); ++i )
	{
		if( (*i)->IsRunAnimate()==false )
			(*i)->UninitAnimate();
	}

	// 删除播放完毕的动画
	_animateList.remove_if( std::not1( std::mem_fun<bool, UIAnimateBase>( &UIAnimateBase::IsRunAnimate ) ) );

	// 计算每个要播放的动画
	_animateListTemp.clear();
	for( auto i=_animateList.begin(); i!=_animateList.end(); ++i )
	{
		if( (*i)->IsDrawFrame() ) 
			_animateListTemp.push_back( *i );
	}
	//
	return _animateListTemp.size()>0 ? true:false;
}

void UIAnimateManage::DrawAutoList()
{
	for( auto i=_animateList.begin(); i!=_animateList.end(); ++i )
		(*i)->DrawAuto();
}

void UIRegisterAnimate( UIAnimateBase* pAnimateObj )
{
	UIPostMessage( NULL, WM_REGANIMATE, (WPARAM)pAnimateObj, 0 );
}



UICaret* UICaret::GetSingleton()
{
	if( _pSingleton==NULL )  
		_pSingleton = new UICaret;

	return _pSingleton; 
}

void UICaret::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

void UICaret::Assign( UICaret* pPara )
{
	_pSingleton = pPara;
}

UICaret::UICaret()
{
	_x = 0;
	_y = 0;
	_width = 1;
	_height = 16;
	_z = -0.5;

	_rgb = _BLACK_;
	_oldTick = 0;

	_isAnimateOn = false;
	_isCaretOn = false;
	_isDraw = false;
}

bool UICaret::IsRunAnimate() 
{ 
	return true; 
}

bool UICaret::IsDrawFrame()
{
	if( !_isAnimateOn ) 
		return false;

	DWORD nowTick = ::GetTickCount();
	DWORD dxTick = abs( (int)( nowTick-_oldTick ) );
	if( dxTick>600 )
	{
		_isDraw = true;
		_isCaretOn = !_isCaretOn;
		_oldTick = nowTick; 
	}
	
	return _isDraw;
}

void UICaret::HideCaret() 
{ 
	_isAnimateOn=false; 
}

void UICaret::SetCaret( float z, float width, float height, D3DCOLOR rgb )
{
	_z = z;
	_width = width;
	_height = height;
	_rgb = rgb;

	_isAnimateOn = true;
	_isCaretOn = false;
	_oldTick = ::GetTickCount();
}

void UICaret::SetPos( int x, int y, bool IsShowImmd )
{
	if( _x!=(float)x || _y!=(float)y )
	{
		_x = (float)x;
		_y = (float)y;
	}

	if( IsShowImmd )
	{	
		_isDraw = true;	
		_isCaretOn = true;
		_oldTick = ::GetTickCount();
	}
}

void UICaret::DrawAuto()
{
	if( !_isAnimateOn ) return;

	_isDraw = false;

	if( !_isCaretOn ) return;
	
	UI2DRect( (int)_x, (int)( _y-_height/2 ), (int)_width, (int)_height, _z )( _rgb, 255 );
}

void UIShowCaret( float z, int width, int height, D3DCOLOR rgb )
{
	UICaret::GetSingleton()->SetCaret( z, (float)width, (float)height, rgb );
}

void UIHideCaret()
{
	UICaret::GetSingleton()->HideCaret();
}

void UISetCaretPos( int x, int y, bool IsShowImmd )
{
	UICaret::GetSingleton()->SetPos( x, y, IsShowImmd );
}




UIAnimateEffect::UIAnimateEffect()
{
	_frameCount = 0;
	_maxFrame = -1;

	_loadImageWay = 1;
	_imageID = -1;
	_colorKey = 0;
	_hitPower = 0.8f;
}

void UIAnimateEffect::PlayAnimate( int mode, int count )
{
	_animateMode = mode;

	_frameCount = 0;

	if( _animateMode>=100 )
	{
		if( _animateMode==HIT_DRUM )
			_maxFrame = HIT_DRUM_FRAME;
		else if( _animateMode==SET_HOT )
			_maxFrame = SET_HOT_FRAME;
		else if( _animateMode==SET_COOL )
			_maxFrame = SET_COOL_FRAME;
	}
	else
	{
		_maxFrame = count;
	}
	
	UIRegisterAnimate( this );
}

bool UIAnimateEffect::IsRunAnimate() 
{ 
	return _frameCount<=_maxFrame; 
}

bool UIAnimateEffect::IsDrawFrame() 
{
	++_frameCount;
	return _frameCount<=_maxFrame; 
}

void UIAnimateEffect::SetImageIDAndDLL( int id, std::string path, D3DCOLOR colorKey ) 
{
	_imageID = id;
	_dllPath = path;
	_colorKey = colorKey;
	_loadImageWay = 1;
}

void UIAnimateEffect::SetImagePath( std::string path, D3DCOLOR colorKey )
{
	_imagePath = path;
	_colorKey = colorKey;
	_loadImageWay = 2;
}

void UIAnimateEffect::SetHitPower( float v ) 
{ 
	_hitPower=v; 
}

void UIAnimateEffect::DrawHitDrumAnimate( int centerX, int centerY, float _z )
{
	if( _animateMode!=HIT_DRUM  ) return;
	if( _loadImageWay==1 && _imageID==-1 ) return;
	if( _loadImageWay==2 && _imagePath=="" ) return;

	UI2DImage image;
	if( _loadImageWay==1 )
		image.Init( _dllPath.c_str(), _imageID, _z, _colorKey );
	else if( _loadImageWay==2 )
		image.Init( _imagePath, _z, _colorKey );
	image.operator()( centerX, centerY );
	image.operator()( centerX, centerY, 1+_hitPower*_frameCount/HIT_DRUM_FRAME, 255-250*_frameCount/HIT_DRUM_FRAME );
}

void UIAnimateEffect::DrawHitDrumAnimate( RECT rc, float _z )
{
	if( _animateMode!=HIT_DRUM ) return;
	if( _loadImageWay==1 && _imageID==-1 ) return;
	if( _loadImageWay==2 && _imagePath=="" ) return;

	UI2DImage image;
	if( _loadImageWay==1 )
		image.Init( _dllPath.c_str(), _imageID, _z, _colorKey );
	else if( _loadImageWay==2 )
		image.Init( _imagePath, _z, _colorKey );
	image.operator()( rc );
	image.operator()( rc, 1+_hitPower*_frameCount/HIT_DRUM_FRAME, 255-250*_frameCount/HIT_DRUM_FRAME );
}

void UIAnimateEffect::DrawFrameHitDrumAnimate( RECT rc, int corner, float _z )
{
	if( _animateMode!=HIT_DRUM ) return;
	if( _loadImageWay==1 && _imageID==-1 ) return;
	if( _loadImageWay==2 && _imagePath=="" ) return;

	DrawFrameHelp frameImage;
	if( _loadImageWay==1 )
		frameImage.Init( _dllPath.c_str(), _imageID, corner, corner, corner, corner, _z, _colorKey );
	else if( _loadImageWay==2 )
		frameImage.Init( _imagePath, corner, corner, corner, corner, _z, _colorKey );
	frameImage.operator()( rc );
	frameImage.operator()( CalcZoomRect()( rc, 1+_hitPower*_frameCount/HIT_DRUM_FRAME ), 255-250*_frameCount/HIT_DRUM_FRAME );
}

void UIAnimateEffect::DrawHotAnimate( RECT rc, float _z )
{
	if( _animateMode!=SET_HOT ) 
		return;

	int dx[SET_HOT_FRAME];
	int dxTemp = 230/( SET_HOT_FRAME-1 );
	for( int i=0; i<SET_HOT_FRAME; ++i )
		dx[i] = 10 + dxTemp*i;

	DrawFrameHelp( "GUIResource.dll", IDB_HOT_EFFECT2, 2, 2, 2, 2, _z, D3DCOLOR_XRGB(255, 0, 255) )( rc, dx[_frameCount-1] ); 
}

void UIAnimateEffect::DrawCoolAnimate( RECT rc, float _z )
{
	if( _animateMode!=SET_COOL )
		return;

	int dx[SET_COOL_FRAME];
	int dxTemp = 230/( SET_COOL_FRAME-1 );
	for( int i=0; i<SET_COOL_FRAME; ++i )
		dx[i] = 10 + dxTemp*i;

	DrawFrameHelp( "GUIResource.dll", IDB_HOT_EFFECT2, 2, 2, 2, 2, _z, D3DCOLOR_XRGB(255, 0, 255) )( rc, dx[SET_COOL_FRAME-_frameCount] ); 
}