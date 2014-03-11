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


bool UIDXInit::InitD3D(	HWND bindHWnd,										// [in] �󶨵�win32���ھ��
						ULONG width, ULONG height,						// [in] Backbuffer dimensions.
						bool windowed,									// [in] Windowed ( true )or full screen ( false ).
						D3DDEVTYPE deviceType )							// [in] HAL or REF
{
	// Step 1: ����IDirect3D9�ӿڶ���.
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	//
	if( !d3d9 )
	{
		::MessageBoxA( 0, "Direct3DCreate9() - FAILED", "Warning", 0 );
		return false;
	}

	// Step 2: ���Ӳ����������
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps( D3DADAPTER_DEFAULT, deviceType, &caps );
	//
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: ���D3DPRESENT_PARAMETERS��ʾ�ṹ��.
	_presentParam.BackBufferWidth            = width;
	_presentParam.BackBufferHeight           = height;
	_presentParam.BackBufferFormat           = D3DFMT_A8R8G8B8;
	_presentParam.BackBufferCount            = 1;								// ������Ļ����
	_presentParam.MultiSampleType            = D3DMULTISAMPLE_NONE;				// ȫ������ݵ�����
	_presentParam.MultiSampleQuality         = 0;								// ȫ������ݵ������ȼ�
	_presentParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD;			// ָ�������ڽ�����������α�����
	_presentParam.hDeviceWindow              = bindHWnd;
	_presentParam.Windowed                   = windowed;
	_presentParam.EnableAutoDepthStencil     = true;							// ��Ϊtrue��D3D���Զ��������/ģ�滺��
	_presentParam.AutoDepthStencilFormat     = D3DFMT_D24S8;					// 32λ���/ģ�滺��,����24λΪ��Ȼ���,8λΪģ�滺��
	_presentParam.Flags                      = 0;
	_presentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ˢ����
	_presentParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	

	// Step 4: ������ʾ�豸�ӿ�.
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

	// step 5: �ͷ�d3d�ӿڣ�������ʼ��
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

	// �رյƹ�
	pIDevice->SetRenderState( D3DRS_LIGHTING, false );

	// ������ɫalpha͸��
	pIDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	pIDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pIDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// ��˹��դ �������ɫ����Ч��
	pIDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );

	// �����÷����ѡ
	pIDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
}

bool UIRender::AutoAdjustSizeChange( ULONG width, ULONG height )
{
	UIDXInit::GetSingleton()->ResetD3D( width, height );			// �������û�����Ĵ�С
	InitRenderEffect();												// ���³�ʼ����ʾЧ��
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
	// ������С��������render
	HWND hWnd = UIDXInit::GetSingleton()->GetBindHWnd();
	if( hWnd!=0 )
	{
		if( IsIconic( hWnd )!=0 ) 
			return;
	}
		
	// ����������
	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	pIDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER| D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0 );
	
	pIDevice->BeginScene();

	// ���Ƶ�
	UI2DPoint( 9, 9 )( _RED_ );
	UI2DPoint( 9, 10 )( _RED_ );
	UI2DPoint( 10, 9 )( _RED_ );
	UI2DPoint( 10, 10 )( _RED_ );

	// ������
	UI2DLine( 10, 20, 190, 150 )( _RED_, _BLUE_, 255 );
	UI2DLine( 10, 30, 190, 170 )( _RED_ );

	// ���ƾ���
	UI2DRect( GenRect()( 200, 20, 400, 200 ) )( _BLUE_ );
	UI2DRect( GenRect()( 450, 20, 650, 200 ) )( _ORANGE_, 255 );
	UI2DRect( GenRect()( 700, 20, 900, 200 ) )( _BLUE_, _RED_, 255 );

	// ����ͼƬ
	UI2DImage( "voltage.png" )( GenRect()( 20, 220, 84, 284 ) );
	UI2DImage( "voltage.png" )( 150, 260, 1.5 );

	UI2DFont font;
	//for( int i=0; i<1000; ++i )
	font( "Font����123", GenRect()( 300, 220, 400, 284 ) );

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