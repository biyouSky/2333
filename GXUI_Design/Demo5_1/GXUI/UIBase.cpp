#include "stdafx.h"

#include "UIBase.h"
#include "UIRender.h"

using namespace std;


cnCriticalSection gDXCS;
UIMessageLoopManage* UIMessageLoopManage::_pSingleton = NULL;


UIMessage::UIMessage() 
{ 
	memset( this, 0, sizeof( UIMessage ) ); 
}

UIMessage::UIMessage( UIWinBase* pWin, UINT message, WPARAM wParam, LPARAM lParam )
{
	_pWin = pWin;
	_msg = message;
	_wParam = wParam;
	_lParam = lParam;
}

bool UIMessageFilter1( UINT message )
{
	bool rt = true;
	switch( message )
	{
	case WM_PAINT:
	case WM_CREATE:
	case WM_DESTROY:
	case WM_SIZE:
	case WM_MOUSEHOVER:
	case WM_MOUSELEAVE:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_KEYDOWN:
	case WM_CHAR:
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	case WM_HSCROLL:
	case WM_VSCROLL:
	case WM_MOUSEWHEEL:
	case WM_COPY:
	case WM_PASTE:
	case WM_CUT:
	case WM_MOVE:
	case WM_NOTIFY:
	case WM_COMMAND:
	//case WM_ADJUSTSIZE:
	case WM_UPDATE:
		rt = false;
	}
	return rt;
}



void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam )
{
	// 消息队列接收消息过滤
	if( UIMessageFilter1( message ) ) 
		return;						

	UIMessageLoopManage::GetSingleton()->PushMessage( winPoint, message, wParam, lParam );
}

void UIRefresh( bool immediately )
{
	UIPostMessage( 0, WM_PAINT, immediately?1:0, 0 );
}

UIMessageLoopManage* UIMessageLoopManage::GetSingleton()
{
	if( _pSingleton==NULL ) 
		_pSingleton = new UIMessageLoopManage;

	return _pSingleton; 
}

void UIMessageLoopManage::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

UIMessageLoopManage::UIMessageLoopManage() 
{ 
}

UIMessageLoopManage::~UIMessageLoopManage()
{
}

void UIMessageLoopManage::PushMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam )
{
	cnCriticalSectionAuto csAuto( _uiMsgListCS );

	// WM_PAINT消息处理
	if( message==WM_PAINT )
	{
		if( wParam==1 )
		{	// 需要即时刷新的绘制
			if( _uiMsgList.size()>0 )
				_uiMsgList.erase( remove_if( _uiMsgList.begin(), _uiMsgList.end(), [](UIMessage& msg) { return msg._msg==WM_PAINT; } ), _uiMsgList.end() );

			_uiMsgList.push_front( UIMessage( winPoint, message, wParam, lParam ) );
		}
		else
		{
			if( _uiMsgList.size()>0 )
			{
				if( _uiMsgList.back()._msg==WM_PAINT )
					return;
			}
			
			_uiMsgList.push_back( UIMessage( winPoint, message, wParam, lParam ) );
		}
		return;
	}
	
	// 其它消息处理
	_uiMsgList.push_back( UIMessage( winPoint, message, wParam, lParam ) );
}

bool UIMessageLoopManage::PopMessage( UIMessage& msg )
{
	cnCriticalSectionAuto csAuto( _uiMsgListCS );

	if( _uiMsgList.size()>0 )
	{
		msg = _uiMsgList.front();
		_uiMsgList.pop_front();
		return true;
	}
	return false;
}

bool UIMessageLoopManage::HandleMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam )
{
	bool rt = true;
	
	/*if( winPoint!=NULL )
		rt = winPoint->HandleMessage( message, wParam, lParam );
	else
		rt = gUIContainerTop.HandleMessagePre( message, wParam, lParam );*/

	return rt;
}

void UIMessageLoopManage::RunMessageLoopThread( void* pData )
{
	enum
	{
		FRAME_INTERVAL = 33
	};

	UIMessage msg;
	while( 1 )
	{	
		BEGIN_WAIT()
		gDXCS.Lock();

		// 刷新标记
		bool f1 = false;

		// 在33ms内尽可能多的取消息出来进行处理
		while( PopMessage( msg )==true )	
		{	
			// 消息为WM_PAINT则置刷新标记为true
			// 其它消息则调用默认处理函数进行处理
			if( msg._msg==WM_PAINT )		
				f1 = true;
			else
				HandleMessage( msg._pWin, msg._msg, msg._wParam, msg._lParam );

			// 若满33ms则退出取消息循环
			bool breakFlag;
			BREAK_WAIT( FRAME_INTERVAL-3, breakFlag )
			if( breakFlag ) break;
		}

		// 若刷新标记为true则进行刷新
		if( f1 )
			UIRender::GetSingleton()->Render();

		// 若没有满33ms则等待
		gDXCS.UnLock();
		END_WAIT( FRAME_INTERVAL )
	}
}

void UIMessageLoopManage::RunMessageLoop() 
{ 
	StartThread( "UIMsgLoop", &UIMessageLoopManage::RunMessageLoopThread, NULL ); 
}