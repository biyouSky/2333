
#pragma once

#include "UIUtil.h"


extern cnCriticalSection gDXCS;

/*-------------------------------------------------------DirectUI 消息循环机制-------------------------------------------------------*/
// 消息包格式
class UIWinBase;
struct UIMessage
{
	UIWinBase* _pWin;
	UINT _msg;
	WPARAM _wParam;
	LPARAM _lParam;

	UIMessage();
	UIMessage( UIWinBase* pWin, UINT message, WPARAM wParam, LPARAM lParam );
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
