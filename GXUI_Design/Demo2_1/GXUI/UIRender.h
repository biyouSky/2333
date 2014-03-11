
#pragma once

#include "UIUtil.h"

/*-------------------------------------------------------DirectX初始化-------------------------------------------------------*/
class UIDXInit
{
public:
	static UIDXInit* GetSingleton();
	static void Destroy();

	bool InitD3D(	HWND bindHWnd,							// [in]
					ULONG width, ULONG height,				// [in] Backbuffer dimensions. 缓存面大小 最好与窗口大小保持一致
					bool windowed=true,						// [in] Windowed ( true )or full screen ( false ).
					D3DDEVTYPE deviceType=D3DDEVTYPE_HAL );	// [in] HAL or REF

	bool ResetD3D( ULONG width, ULONG height );

	HWND GetBindHWnd();		

	ULONG GetPresentWidth();
	ULONG GetPresentHeight();		

	IDirect3DDevice9* _pIDevice;							// d9 设备接口
	D3DPRESENT_PARAMETERS _presentParam;					// 缓存表面描述

private:
	// 构造 析构
	UIDXInit();
	~UIDXInit();

	static UIDXInit* _pSingleton;
};






/*-------------------------------------------------------UI绘制层-------------------------------------------------------*/
class UIRender
{
public:
	static UIRender* GetSingleton();
	static void Destroy();

	void Render();																// 绘制托管
	bool AutoAdjustSizeChange( ULONG width, ULONG height );						// 自适应窗口大小变化	

private:
	UIRender();
	~UIRender();

	void InitRenderEffect();

	static UIRender* _pSingleton;
};
