
#pragma once

#include "UIUtil.h"


extern cnCriticalSection gDXCS;

/*-------------------------------------------------------DirectUI 消息循环机制-------------------------------------------------------*/
// 消息包格式
class UIWinBase;
struct UIMessage
{
	UIMessage();
	UIMessage( UIWinBase* pWin, UINT message, WPARAM wParam, LPARAM lParam );

	UIWinBase* _pWin;
	UINT _msg;
	WPARAM _wParam;
	LPARAM _lParam;
};



// DirectUI 消息循环处理机制  所有事件全部放入消息循环中 按队列机制处理  变相实现了操作互斥
class UIMessageLoopManage : public _MultiThreadManage<UIMessageLoopManage>
{
public:
	static UIMessageLoopManage* GetSingleton();
	static void Destroy();
	
	void RunMessageLoop();
	friend void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );

private:
	UIMessageLoopManage();
	~UIMessageLoopManage();

	void RunMessageLoopThread( void* pData );

	void PushMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );
	bool PopMessage( UIMessage& msg ); 

	bool HandleMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );

	static UIMessageLoopManage* _pSingleton;

	std::deque<UIMessage> _uiMsgList;		// UI消息队列
	cnCriticalSection _uiMsgListCS;
};
void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );
void UIRefresh( bool immediately=true );
bool UIMessageFilter1( UINT message );		// 进入UI消息循环过滤







/*-------------------------------------------------------UIWinBase-------------------------------------------------------*/
/*
	基础窗口类 
	基础信息：
	1 位置信息
	2 层次信息
	3 布局信息
	4 其它

	基础功能：
	1 基础绘制
	2 窗口生成、销毁
	3 消息的默认处理
	注：操作分有保护和无保护，有保护即将操作请求发送到消息循环中，无保护即直接操作，
		对于无保护的直接设置函数名后都将加上Direct，如SetXXDirect()；
*/
class UIContainer;
class UIWinBase
{
	friend UIContainer;

public:
	// 构造 析构
	UIWinBase();
	virtual ~UIWinBase();

	// 创建窗口  
	bool CreateDirect( UIContainer* pUIContainer, RECT& relativeRect, int layoutFlag=UILayoutCalc::NO_ZOOM, bool isShow=true, bool isOnHeap=false );
	bool CreateDirect( UIWinBase* pParent, RECT& relativeRect=_NULLRECT_, int layoutFlag=UILayoutCalc::NO_ZOOM, bool isShow=true, bool isOnHeap=false );
	// 销毁窗口
	bool DestroyDirect();

	// 消息处理
	virtual bool HandleMessage( UINT message, WPARAM wParam, LPARAM lParam );

	// 设置 直接无保护
	

	// 读取 无保护
	RECT GetClientRect();
	RECT GetRelativeRect();

	//
	virtual void Draw();						// 绘制

	UIContainer* _pUIContainer;					// 自身容器指针

protected:
	// 默认消息处理
	bool DefHandleMessage( UINT message, WPARAM wParam, LPARAM lParam );

	// 位置信息
	RECT _clientRC;								// 起始为0的大小矩形
	POINT _relativePoint;						// 相对父窗口的起始位置点
	POINT _abusolutePoint;						// 句柄顶层窗口的起始位置点	

	// 层次信息
	int _layoutLever;							// 层次 决定深度值 1-0.2 2-0.19 3-0.18
	float _z;									// 深度值 

	// 其它
	bool _isShow;								// 显示窗口
	bool _isRecvTabKey;							// 接收tab键
	bool _isFocus;								// 窗口获得焦点激活信息
	int _layoutMode;							// 布局模式
	bool _isTransmissionMsg;					// 透射子窗口传递的消息

	UIContainer* _pParentUIContainer;			// 父容器指针
	UIWinBase *_pRealParent;					// popup类型窗口所用
};






/*-------------------------------------------------------UIContainer-------------------------------------------------------*/
/*
	名字：窗口容器
	描述：容器类，提供对子窗口的管理，分发传递并处理消息。
	用法：建议通过 包含 的方式，在父窗体的消息处理中按需求加入容器的消息处理。
	功能：
		1 子窗口管理，参与窗口的创建、销毁、布局计算、绘制管理等；
		2 消息向下传递处理，父窗口收到消息会先通过绑定的窗口容器传递到子窗口进行预处理
		 （通过子窗口绑定的窗口容器继续向下传递，形成一个递归过程），若子窗口未能处理则再由父窗口自身进行处理；

	注意：容器类的所有设置操作都是无保护的，只能在消息循环处理中调用才能确保安全，但因为容器类只在GXUI框架内部使用，
		  属于框架内部概念，因此函数名后不另加Direct区别；
*/
class UIContainer
{
	// 容器元素
	struct UIElement
	{
		UIWinBase* _pWin;						// 子窗口指针
		UILayoutCalc _layoutInfor;				// 布局模式
		bool _isOnHeap;							// 窗口内存是否位于堆上

		UIElement( UIWinBase* p, bool isOnHeap ) { _pWin=p; _isOnHeap=isOnHeap; }
		bool operator==( UIWinBase* p ) { return _pWin==p; }
	};
	typedef std::vector<UIElement> UIElementListType;

public:
	UIContainer();
	~UIContainer();

	// 与父窗口绑定
	void BindWin( UIWinBase *pBindWindow );
	void BindWin( HWND hWnd );

	// 添加子窗口  删除子窗口  操作中未进行数据保护
	void AddChild( UIWinBase* pWin, bool isOnHeap );	
	void DelChild( UIWinBase* pWin );
 
	// 消息预前处理包含除WM_PAINT外所有消息
	bool HandleMessagePre( UINT message, WPARAM wParam, LPARAM lParam );

	// 读取
	POINT GetBindWinAbusolutePoint();
	int GetBindWinLayoutLever();
	
	// 设置
	void SetNewFocusOnChild( UIWinBase* pNew );

	// 绘制
	void Draw();

private:	
	UIWinBase* GetFocusOnChild();				// 查找focus为on的子窗口

	bool _isBindDUI;							// 标记绑定的父窗体时DUI或hwnd
	UIWinBase* _pBindDUIWin;					// 相绑定的DUI父窗体
	HWND _bindWindowHwnd;						// 相绑定的hwnd父窗体

	POINT _preMousePt;							// 记录的鼠标前一点位置

	UIElementListType _winList;					// 子窗体队列
};

// 提供自身Container支持
template<class T>
class UIContainerHelp
{
public:
	UIContainerHelp()
	{
		T* pT = static_cast<T*>( this );
		pT->_pUIContainer = &_uiContainer;
		_uiContainer.BindWin( pT ); 
	}

protected:
	UIContainer _uiContainer;
};
