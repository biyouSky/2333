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
	// ��Ϣ���н�����Ϣ����
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

	// WM_PAINT��Ϣ����
	if( message==WM_PAINT )
	{
		if( wParam==1 )
		{	// ��Ҫ��ʱˢ�µĻ���
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
	
	// ������Ϣ����
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

		// ˢ�±��
		bool f1 = false;

		// ��33ms�ھ����ܶ��ȡ��Ϣ�������д���
		while( PopMessage( msg )==true )	
		{	
			// ��ϢΪWM_PAINT����ˢ�±��Ϊtrue
			// ������Ϣ�����Ĭ�ϴ��������д���
			if( msg._msg==WM_PAINT )		
				f1 = true;
			else
				HandleMessage( msg._pWin, msg._msg, msg._wParam, msg._lParam );

			// ����33ms���˳�ȡ��Ϣѭ��
			bool breakFlag;
			BREAK_WAIT( FRAME_INTERVAL-3, breakFlag )
			if( breakFlag ) break;
		}

		// ��ˢ�±��Ϊtrue�����ˢ��
		if( f1 )
			UIRender::GetSingleton()->Render();

		// ��û����33ms��ȴ�
		gDXCS.UnLock();
		END_WAIT( FRAME_INTERVAL )
	}
}

void UIMessageLoopManage::RunMessageLoop() 
{ 
	StartThread( "UIMsgLoop", &UIMessageLoopManage::RunMessageLoopThread, NULL ); 
}