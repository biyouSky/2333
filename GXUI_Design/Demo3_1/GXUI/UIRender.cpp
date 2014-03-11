#include "stdafx.h"

#include "UIRender.h"
#include "UI2D.h"
using namespace std;


UIDXInit* UIDXInit::_pSingleton = NULL;
UIRender* UIRender::_pSingleton = NULL;

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


bool UIDXInit::InitD3D(	HWND bindHWnd,										// [in] 绑定的win32窗口句柄
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

	// 绘制点
	UI2DPoint( 9, 9 )( _RED_ );
	UI2DPoint( 9, 10 )( _RED_ );
	UI2DPoint( 10, 9 )( _RED_ );
	UI2DPoint( 10, 10 )( _RED_ );

	// 绘制线
	UI2DLine( 10, 20, 190, 150 )( _RED_, _BLUE_, 255 );
	UI2DLine( 10, 30, 190, 170 )( _RED_ );

	// 绘制矩形
	UI2DRect( GenRect()( 200, 20, 400, 200 ) )( _BLUE_ );
	UI2DRect( GenRect()( 450, 20, 650, 200 ) )( _ORANGE_, 255 );
	UI2DRect( GenRect()( 700, 20, 900, 200 ) )( _BLUE_, _RED_, 255 );

	// 绘制图片
	UI2DImage( "voltage.png" )( GenRect()( 20, 220, 84, 284 ) );
	UI2DImage( "voltage.png" )( 150, 260, 1.5 );

	UI2DFont font;
	//for( int i=0; i<1000; ++i )
	font( "Font测试123", GenRect()( 300, 220, 400, 284 ) );

	/*for( int i=0; i<1000; ++i )
	{
		UI2DFont font2;
		font2( "", GenRect()( 300, 220, 400, 284 ) );
	}*/

	//SaveUIImage( "d:\\tt.png" );

	pIDevice->EndScene();

	// Swap the back and front buffers.
	HRESULT hr = pIDevice->Present( 0, 0, 0, 0 );
}