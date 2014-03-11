
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

	void SaveUIImage( std::string imagePath );

private:
	UIRender();
	~UIRender();

	void InitRenderEffect();

	static UIRender* _pSingleton;
};



/*-------------------------------------------------------动画引擎-------------------------------------------------------*/

// 动画基类
/*
	从基类派生的动画对象绘制情况分两种：
	1. 补全DrawAuto()，自身进行绘制
	2. 不补全DrawAuto()，只继承了动画绘制判定，具体绘制另行处理
*/
struct UIAnimateBase
{
	friend class UIAnimateManage;

	virtual void Init() {}							// 初始化
	virtual void Uninit() {}						// 

	virtual bool IsRun() = 0;						// 动画是否运行
	virtual bool IsDrawFrame() = 0;					// 计算本帧动画 是否绘制
	virtual void DrawAuto() {}						// 本帧动画绘制 可能为空 
};



// 动画管理
class UIAnimateManage
{
public:
	static UIAnimateManage* GetSingleton();
	static void Destroy();

	void AddAnimate( UIAnimateBase* pAnimateObj );
	bool HandleTimer();
	void DrawAutoList();

protected:
	UIAnimateManage();
	~UIAnimateManage();

	std::list<UIAnimateBase* > _animateList;		// 动画列表 动画播放完毕自动删除
	std::vector<UIAnimateBase* > _animateListTemp;	

	static UIAnimateManage* _pSingleton;
};
void UIRegisterAnimate( UIAnimateBase* pAnimateObj );