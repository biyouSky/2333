#include "stdafx.h"

#include "UIBase.h"
#include "UIRender.h"
#include "WinShell.h"
#include "UI2D.h"

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
	case WM_CREATE:
	case WM_DESTROY:
	case WM_PAINT:
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
			_uiMsgList.push_front( UIMessage( winPoint, message, wParam, lParam ) );
		else
			_uiMsgList.push_back( UIMessage( winPoint, message, wParam, lParam ) );

		_uiMsgList.erase( unique( _uiMsgList.begin(), 
								  _uiMsgList.end(), 
								  []( UIMessage& msg1, UIMessage& msg2 ) { return ( msg1._msg==WM_PAINT ) && ( msg2._msg==WM_PAINT ); } 
								), 
						  _uiMsgList.end() );
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
	
	if( winPoint!=NULL )
		rt = winPoint->HandleMessage( message, wParam, lParam );
	else
		rt = UIWinShell::_uiTopContainer.HandleMessagePre( message, wParam, lParam );

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


UIWinBase::UIWinBase()
{
	_isShow = false;
	_isRecvTabKey = false;
	_isFocus = false;
	_isTransmissionMsg = false;

	_layoutLever = 1;
	_z = 0;

	_pParentUIContainer = NULL;
	_pRealParent = NULL;
	_pUIContainer = NULL;
}

UIWinBase::~UIWinBase() 
{
}

bool UIWinBase::CreateDirect( UIContainer* pUIContainer, RECT& relativeRect, int layoutFlag, bool isShow, bool isOnHeap )
{
	// ��鸸�����Ƿ����
	if( pUIContainer==NULL ) 
		return false;
	
	// ��ȡ������
	_pParentUIContainer = pUIContainer;

	// ������Ϣ
	_layoutMode = layoutFlag;

	// �Ƿ���ʾ������
	_isShow = isShow;

	// ��С λ����Ϣ
	// ���Դ�С
	_clientRC = relativeRect;
	OffsetRect( &_clientRC, -_clientRC.left, -_clientRC.top );
	// ��Ը��������
	_relativePoint = GenPoint()( relativeRect.left, relativeRect.top );
	// ��Ծ���������
	POINT p = pUIContainer->GetBindWinAbusolutePoint();
	_abusolutePoint = GenPoint()( p.x+relativeRect.left, p.y+relativeRect.top );

	// �����Ϣ��zֵ
	_layoutLever = pUIContainer->GetBindWinLayoutLever()+1;
	_z = 0.2f;
	for( int i=1; i<_layoutLever; ++i )
		_z -= 0.01f;

	//
	return HandleMessage( WM_CREATE, isOnHeap?1:0, (LPARAM)( _pParentUIContainer ) );
}

bool UIWinBase::CreateDirect( UIWinBase* pParent, RECT& relativeRect, int layoutFlag, bool isShow, bool isOnHeap )
{
	return CreateDirect( pParent->_pUIContainer, relativeRect, layoutFlag, isShow, isOnHeap );
}

bool UIWinBase::DestroyDirect()
{
	return HandleMessage( WM_DESTROY, 0, 0 );
}

bool UIWinBase::HandleMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	return DefHandleMessage( message, wParam, lParam );
}

bool UIWinBase::DefHandleMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	// ��ϢԤ����
	bool rt = false;

	if( message==WM_CREATE )
	{	// ��Ҫ�������ڴ�����ӵ���������
		_pParentUIContainer->AddChild( this, wParam==1?true:false );
		_pParentUIContainer->SetNewFocusOnChild( this );
	}
	else if( message==WM_DESTROY )
	{	// ɾ���Ӵ��� 
		if( _pUIContainer!=NULL )
			_pUIContainer->HandleMessagePre( WM_DESTROY, wParam, lParam );
		
		// ��������Ӹ�������ɾ��
		_pParentUIContainer->DelChild( this );
	}
	else if( _pUIContainer!=NULL )
	{
		rt = _pUIContainer->HandleMessagePre( message, wParam, lParam );
	}
	
	return rt;
}

void UIWinBase::Draw()
{
	if( _isShow )
		UI2DRect( _abusolutePoint.x, _abusolutePoint.y, GetWidth()( _clientRC ), GetHeight()( _clientRC ), _z )( _RED_, 100 );

	if( _pUIContainer!=NULL )
		_pUIContainer->Draw();
}

RECT UIWinBase::GetClientRect()  
{ 
	return _clientRC; 
}

RECT UIWinBase::GetRelativeRect() 
{
	RECT relativeRect = _clientRC;
	OffsetRect( &relativeRect, _relativePoint.x, _relativePoint.y );
	return relativeRect; 
}


UIContainer::UIContainer()
{
	_isBindDUI = true;
	_pBindDUIWin = NULL;

	_preMousePt = GenPoint()( 0, 0 );
}

UIContainer::~UIContainer()
{
}

void UIContainer::BindWin( UIWinBase *pBindWindow )
{
	_pBindDUIWin = pBindWindow;
}

void UIContainer::BindWin( HWND hWnd )
{
	_isBindDUI = false;
	_bindWindowHwnd = hWnd;
}

void UIContainer::AddChild( UIWinBase* pWin, bool isOnHeap )
{
	// �����Ӵ����б���
	_winList.push_back( UIElement( pWin, isOnHeap ) );
	_winList.back()._layoutInfor.SetLayoutMode( pWin->_layoutMode );

	// �Ӵ��ڵĲ�����Ϣ��ʼ��
	RECT parentRect;
	if( _isBindDUI==true )
		parentRect = _pBindDUIWin->GetClientRect();
	else
		GetClientRect( _bindWindowHwnd, &parentRect );
	//
	_winList.back()._layoutInfor.InitLayout( parentRect, pWin->GetRelativeRect() );
}

void UIContainer::DelChild( UIWinBase* pWin )
{
	// ���Ӵ����б���ɾ��
	UIElementListType::iterator it = find( _winList.begin(), _winList.end(), pWin );
	if( it==_winList.end() )  return;

	if( it->_isOnHeap )
		delete pWin;
	_winList.erase( it );	
}

void UIContainer::SetNewFocusOnChild( UIWinBase* pNew )
{
	UIWinBase* pOld = GetFocusOnChild();
	if( pOld==pNew ) return;

	if( pOld!=NULL )
	{
		pOld->_isFocus = false;
		pOld->HandleMessage( WM_KILLFOCUS, NULL, NULL );
	}

	pNew->_isFocus = true;
	pNew->HandleMessage( WM_SETFOCUS, NULL, NULL );
}

UIWinBase* UIContainer::GetFocusOnChild()
{
	for( UINT i=0; i<_winList.size(); ++i )
	{	
		if( _winList[i]._pWin->_isShow==false ) continue;
		if( _winList[i]._pWin->_isFocus==true ) return _winList[i]._pWin;
	}
	return NULL;
}

POINT UIContainer::GetBindWinAbusolutePoint()
{
	if( _isBindDUI==true ) 
		return _pBindDUIWin->_abusolutePoint;
	else 
		return GenPoint()( 0, 0 );
}

int UIContainer::GetBindWinLayoutLever()
{
	if( _isBindDUI==true ) 
		return _pBindDUIWin->_layoutLever;
	else 
		return 0;
}

bool UIContainer::HandleMessagePre( UINT message, WPARAM wParam, LPARAM lParam )
{
	UIWinBase* pMinZWin = NULL;
	bool isMsgHandled = false;
	switch( message )
	{
	case WM_CREATE:
		{
			if( UIWinShell::_pWinShell!=NULL )
				UIWinShell::_pWinShell->AddUIElements();

			isMsgHandled = true;
		}
		break;
	case WM_DESTROY:
		{	// �γ�Ƕ�׵���ɾ���Ӵ���
			for( UINT i=0; i<_winList.size(); ++i )
				_winList[i]._pWin->HandleMessage( WM_DESTROY, 0, 0 );
		}
		break;
	}

	return isMsgHandled;
}

void UIContainer::Draw()
{
	for( UINT i=0; i<_winList.size(); ++i )
	{
		if( _winList[i]._pWin->_isShow )
			_winList[i]._pWin->Draw();
	}
}
