
#pragma once

#include "UIUtil.h"

/*-------------------------------------------------------DirectX初始化-------------------------------------------------------*/
class UIDXInit
{
public:
	static UIDXInit* GetSingleton();
	static void Destroy();
	static void Assign( UIDXInit* pPara );

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
	static void Assign( UIRender* pPara );

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

	virtual void InitAnimate() {}					// 初始化
	virtual void UninitAnimate() {}					// 

	virtual bool IsRunAnimate() = 0;				// 动画是否运行
	virtual bool IsDrawFrame() = 0;					// 计算本帧动画 是否绘制
	virtual void DrawAuto() {}						// 本帧动画绘制 可能为空 
};

// 动画管理
class UIAnimateManage
{
public:
	static UIAnimateManage* GetSingleton();
	static void Destroy();
	static void Assign( UIAnimateManage* pPara );

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


// 插入符支持
class UICaret : public UIAnimateBase
{
public:
	static UICaret* GetSingleton();
	static void Destroy();
	static void Assign( UICaret* pPara );

	void SetCaret( float z, float width, float height, D3DCOLOR rgb );
	void HideCaret();
	void SetPos( int x, int y, bool IsShowImmd );

private:
	UICaret();

	bool IsRunAnimate();
	bool IsDrawFrame();
	void DrawAuto();

	static UICaret* _pSingleton;

	// 绘制相关
	float _x, _y, _width, _height, _z;			// 其中_x, _y为caret中心第一点的坐标
	D3DCOLOR _rgb;

	// 动画相关
	bool _isAnimateOn;							// 动画标志
	bool _isCaretOn;							// 显示标志
	bool _isDraw;								// 绘制标志

	DWORD _oldTick;
};
//
void UIShowCaret( float z, int width, int height, D3DCOLOR rgb );
void UIHideCaret();
void UISetCaretPos( int x, int y, bool IsShowImmd=true );


// 动画公共特效
class UIAnimateEffect : public UIAnimateBase
{
	/*--------------------------------- 共有数据 ---------------------------------*/
public:
	UIAnimateEffect();

	void PlayAnimate( int mode, int count=5 );

	enum 
	{	
		HIT_DRUM = 100,			 // 100以内留给自定义
		SET_HOT,
		SET_COOL
	};

protected:
	bool IsRunAnimate();
	bool IsDrawFrame();

	enum 
	{	// 动画播放帧数
		HIT_DRUM_FRAME = 10,
		SET_HOT_FRAME = 9,
		SET_COOL_FRAME = 9
	};

	int _animateMode;
	int _maxFrame;
	int _frameCount;

	/*--------------------------------- 击鼓特效 ---------------------------------*/
public:
	void SetImageIDAndDLL( int id, std::string path="GUIResource.dll", D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void SetImagePath( std::string path, D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void SetHitPower( float v );

	void DrawHitDrumAnimate( int centerX, int centerY, float _z );
	void DrawHitDrumAnimate( RECT rc, float _z );
	void DrawFrameHitDrumAnimate( RECT rc, int corner, float _z );

protected:
	int _loadImageWay;							// 1: _imageID    2: _imagePath
	int _imageID;
	std::string _dllPath;						
	std::string _imagePath;
	D3DCOLOR _colorKey;
	float _hitPower;							// 图片放大系数

	
	/*--------------------------------- hot特效 ---------------------------------*/
public:
	void DrawHotAnimate( RECT rc, float _z );


	/*--------------------------------- cool特效 ---------------------------------*/
public:
	void DrawCoolAnimate( RECT rc, float _z );
};