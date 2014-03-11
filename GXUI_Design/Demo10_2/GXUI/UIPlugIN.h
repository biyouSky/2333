
#pragma once

#include "UI3D.h"
#include "UIWidget.h"
#include "WinShell.h"

#define ID_MAINBAR						10000
#define ID_METROSTART					10100

// 信息说明
typedef char* (*PFunc_PlugIN_Infor)();

// 初始化 创建
typedef void (*PFunc_PlugIN_Init)( UIDXInit* pPara1, UIAnimateManage* pPara2, UIMessageLoopManage* pPara3, UICamera* pPara4, UIRender* pPara5, UIWinShell* pPara6 );
typedef void (*PFunc_PlugIN_Create)( UIContainer* pPara1, RECT* pPara2 );

// 绘制相关
typedef void (*PFunc_PlugIN_SetShow)( bool );
typedef void (*PFunc_PlugIN_Draw)();

// menu信息
typedef int (*PFunc_PlugIN_MenuCount)();
typedef char* (*PFunc_PlugIN_MenuCell)( int index );
typedef void (*PFunc_PlugIN_MenuNotify)( int param );

// tile信息
typedef char* (*PFunc_PlugIN_TileTitle)();
typedef int (*PFunc_PlugIN_TileImageID)();


struct PlugINOBJ
{
	#define CheckNULL( obj )  if( obj==NULL ) return false;

	bool Load( std::string path );

	HMODULE _handle;
		
	std::string _dllPath;

	std::string _infor;
	std::string _tileTitle;
	int _tileImageID;
	VECTOR_STRING _menu;
	int _menuSelIndex;

	PFunc_PlugIN_Create PlugINCreate;
	PFunc_PlugIN_SetShow PlugINSetShow;
	PFunc_PlugIN_Draw PlugINDraw;
	PFunc_PlugIN_MenuNotify PlugINMenuNotify;
};



/*------------------------------------------------------- UIMetro UIMainBar UIStartScreen-------------------------------------------------------*/
// UIMainBar由home button、menu、show tip 3部分组成
class UIMainBar : public UIControl<UIMainBar>
{
	friend UIControl;

	struct MenuCell
	{
		std::string _str;
		RECT _rc;
		bool _isHover;
		bool _isSelect;

		MenuCell( std::string str );
	};
	typedef std::vector<MenuCell> MenuType;

public:
	UIMainBar();
	void Draw();
	void CalcArea();

	void AttachMenu( VECTOR_STRING& strList, int selectIndex=-1 );
	void ClearMenu();

	void SetShowTip( std::string str );

private:
	bool OnMouseMove( POINT pt );
	void OnMouseLeave( POINT pt );
	bool OnLButtonUp( POINT pt );
	void OnUpdate( UINT flag, void* data );

	void CalcMenu();

	// 位置信息
	RECT _homeButtonRC;
	RECT _menuRC;
	RECT _msgTipRC;
	int _dx;
	
	MenuType _menu;					// menu
	std::string _msgTip;			// msg
	bool _isHover;					// hover信息
	RECT _hoverRC;

	// update缓存
	struct Cache1
	{
		VECTOR_STRING _strListCache;
		int _selectIndex;
	};

	struct Cache2
	{
		std::string _msg;
	};
};

class UIMetro : public UIControl<UIMetro>, public UIContainerHelp<UIMetro>, public UIAnimateEffect
{
	friend UIControl;

	// 卡片相关
	class UITile : public UIControl<UITile>, public UIAnimateEffect
	{
		friend UIControl;
		friend UIMetro;

	public:
		UITile();
		void Draw();
		void DrawNoHot();
		void CalcArea();

		void BindPlugIN( PlugINOBJ* pPlugIN );
		void SetHoverDirect( bool flag );

	private:
		bool OnMouseMove( POINT pt );
		void OnMouseLeave( POINT pt );
		bool OnLButtonDown( POINT pt );
		void OnUpdate( UINT flag, void* data );

		// 基础绘制
		void DrawBackGround();
		void DrawHotEffect( UCHAR alphy );
		void DrawImage();
		void DrawTitle();

		void UninitAnimate();

		PlugINOBJ* _pPlugIN;							// 对应的插件

		RECT _rc1, _rc2;
		bool _isHover;
	};

public:
	UIMetro();

	void CalcArea();
	void Draw();

	void SetTitle( int index, PlugINOBJ* pPlugIN );

	enum 
	{	
		START_BUBBLE = 10
	};

private:
	void OnCreate();
	bool OnMouseMove( POINT pt );
	void OnNotify( int id, int param );

	void DrawCommon();
	void DrawStartBubble();

	RECT CalcZoomRC( int index, float zoomScale );
	
	UITile _tileList[6];				// 支持6个瓷片
	RECT _tileRCList[6];
};

class UIStartScreen : public UIControl<UIStartScreen>, public UIContainerHelp<UIStartScreen>
{
	friend UIControl;

public:
	UIStartScreen();
	void Draw();

	void LoadPlugINs( std::string folder );

private:
	void OnCreate();
	void OnNotify( int id, int param );

	inline int FindSelPlugINIndex();

	UIMainBar _mainBar;						// mainBar栏
	
	UITab _tab;								// metro主界面
	std::vector<UIMetro> _metroList;			

	std::vector<PlugINOBJ> _plugINList;		// 插件
	std::vector<bool> _createFlagList;		// 插件创建标志
	PlugINOBJ* _pSelPlugIN;					// 当前显示的插件
};