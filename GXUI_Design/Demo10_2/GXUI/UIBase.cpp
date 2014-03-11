#include "stdafx.h"

#include "UIBase.h"
#include "UIRender.h"
#include "WinShell.h"
#include "UI2D.h"

using namespace std;


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
	case WM_UPDATE:
	case WM_SIZERESET:
	case WM_REGANIMATE:
		rt = false;
	}
	return rt;
}

bool UIMessageFilter2( UINT message )
{
	bool rt = false;
	switch( message )
	{
	case WM_NOTIFY:
	case WM_HSCROLL:
	case WM_VSCROLL:
		rt = true;
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

void UIMessageLoopManage::Assign( UIMessageLoopManage* pPara )
{
	_pSingleton = pPara;
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
	
	// GUI消息处理
	if( winPoint!=NULL )
		rt = winPoint->HandleMessage( message, wParam, lParam );
	else
		rt = UIWinShell::_pWinShell->_uiTopContainer.HandleMessagePre( message, wParam, lParam );

	// Game消息处理 一般用于处理纯的动画对象
	if( winPoint==NULL )
		UIWinShell::_pWinShell->HandleMessageGame( message, wParam, lParam );

	return rt;
}

void UIMessageLoopManage::RunMessageLoopThread( void* pData )
{
	UIDXInit::GetSingleton()->InitD3D( UIWinShell::_pWinShell->_hWnd, UIWinShell::_pWinShell->GetWidth(), UIWinShell::_pWinShell->GetHeight() );

	enum
	{
		FRAME_INTERVAL = 33
	};

	bool breakFlag1 = false;
	UIMessage msg;
	while( 1 )
	{	
		BEGIN_WAIT()

		// WM_PAINT刷新标记
		bool f1 = false;

		// 在33ms内尽可能多的取消息出来进行处理
		while( PopMessage( msg )==true )	
		{	
			if( msg._pWin==NULL && msg._msg==WM_DESTROY )
				breakFlag1 = true;

			// 消息为WM_PAINT则置刷新标记为true
			// 其它消息则调用默认处理函数进行处理
			if( msg._msg==WM_PAINT )		
				f1 = true;
			else
				HandleMessage( msg._pWin, msg._msg, msg._wParam, msg._lParam );

			// 若满33ms则退出取消息循环
			bool breakFlag2;
			BREAK_WAIT( FRAME_INTERVAL-3, breakFlag2 )
			if( breakFlag2 ) break;
		}

		// 动画绘制判定
		bool f2 = UIAnimateManage::GetSingleton()->HandleTimer();

		// 若刷新标记为true则进行刷新
		if( f1|f2 )
			UIRender::GetSingleton()->Render();

		// 
		if( breakFlag1 ) break;

		// 若没有满33ms则等待
		END_WAIT( FRAME_INTERVAL )
	}

	UIDXInit::GetSingleton()->Destroy();
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
	_isUpTransmissionMsg = false;

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
	// 获取父容器
	_pParentUIContainer = pUIContainer;

	// 是否显示即绘制
	_isShow = isShow;

	// 大小 位置信息
	// 绝对大小
	_clientRC = relativeRect;
	OffsetRect( &_clientRC, -_clientRC.left, -_clientRC.top );
	// 相对父窗口起点
	_relativePoint = GenPoint()( relativeRect.left, relativeRect.top );
	// 相对句柄窗口起点
	POINT p = pUIContainer->GetBindWinAbusolutePoint();
	_abusolutePoint = GenPoint()( p.x+relativeRect.left, p.y+relativeRect.top );

	// 布局信息 层次信息及z值
	_layoutMode = layoutFlag;				
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

bool UIWinBase::CreatePopup( UIWinBase* pParent, RECT& relativeRect, int layoutFlag, bool isShow, bool isOnHeap )
{
	_pRealParent = pParent;
	_isShow = isShow;

	// 大小 位置信息 
	_clientRC = relativeRect;
	OffsetRect( &_clientRC, -_clientRC.left, -_clientRC.top );
	//
	_relativePoint = GenPoint()( relativeRect.left, relativeRect.top );
	_abusolutePoint = _relativePoint;

	// 层次信息
	_pParentUIContainer = &UIWinShell::_pWinShell->_uiTopContainer;
	_layoutMode = layoutFlag;
	_layoutLever = 0;
	_z = 0.f;
	
	// 通告
	UIPostMessage( this, WM_CREATE, isOnHeap?1:0, (LPARAM)( _pParentUIContainer ) );
	return true;
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
	// 透射消息
	if( _isUpTransmissionMsg && UIMessageFilter2( message ) )
	{
		_pParentUIContainer->SendMessageToBindWin( message, wParam, lParam );
		return true;
	}

	// 消息预处理
	bool rt = false;
	if( message==WM_CREATE )
	{	// 将要创建窗口窗体添加到父容器中
		_pParentUIContainer->AddChild( this, wParam==1?true:false );
		_pParentUIContainer->SetFocusOnChild( this );
	}
	else if( message==WM_DESTROY )
	{	// 删除子窗口 
		if( _pUIContainer!=NULL )
			_pUIContainer->HandleMessagePre( WM_DESTROY, wParam, lParam );
		
		// 将本窗体从父容器中删除
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
	if( !_isShow )
		return;

	// 自身绘制
	//UI2DRect( _abusolutePoint.x, _abusolutePoint.y, GetWidth()( _clientRC ), GetHeight()( _clientRC ), _z )( _RED_, 100 );

	// 该窗口绑定的容器绘制子窗口
	if( _pUIContainer!=NULL )
	{
		UI2DClipRect uiClip;
		RECT rc = _clientRC;
		OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );
		uiClip.BeginClipRect( rc );

		_pUIContainer->Draw();

		uiClip.EndClipRect();
	}
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

RECT UIWinBase::GetAbusoluteRect() 
{
	RECT abusoluteRect = _clientRC;
	OffsetRect( &abusoluteRect, _abusolutePoint.x, _abusolutePoint.y );
	return abusoluteRect; 
}

bool UIWinBase::IsWindowFocus()
{
	return _isFocus;
}

bool UIWinBase::IsWindowShow()
{
	return _isShow;
}

void UIWinBase::MoveWindowDirect( RECT& relativeRect )
{
	if( _pParentUIContainer==false ) return;

	POINT p = _pParentUIContainer->GetBindWinAbusolutePoint();
	POINT tempPoint = GenPoint()( p.x+relativeRect.left, p.y+relativeRect.top );

	// 位置不变化则不进行更新计算
	if( CompareRect()( relativeRect, GetRelativeRect() ) && ComparePoint()( _abusolutePoint, tempPoint ) ) 
		return;

	// 位置计算
	_relativePoint = GenPoint()( relativeRect.left, relativeRect.top );
	//
	_abusolutePoint = tempPoint;
	//
	_clientRC = relativeRect;
	OffsetRect( &_clientRC, -relativeRect.left, -relativeRect.top );

	//
	this->HandleMessage( WM_SIZE, 0, ( GetHeight()( relativeRect )<<16 )+GetWidth()( relativeRect ) );
}

void UIWinBase::ShowWindowDirect( bool flag )
{
	_isShow = flag;
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
	// 放入子窗口列表中
	_winList.push_back( UIElement( pWin, isOnHeap ) );
	_winList.back()._layoutInfor.SetLayoutMode( pWin->_layoutMode );

	// 子窗口的布局信息初始化
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
	// 从子窗口列表中删除
	UIElementListType::iterator it = find( _winList.begin(), _winList.end(), pWin );
	if( it==_winList.end() )  return;

	if( it->_isOnHeap )
		delete pWin;
	_winList.erase( it );	
}

void UIContainer::SetFocusOnChild( UIWinBase* pNew )
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

void UIContainer::SetUnFocusAll()
{
	for( UINT i=0; i<_winList.size(); ++i )
	{	
		if( _winList[i]._pWin->_isShow==false ) continue;

		// 
		if( _winList[i]._pWin->_isFocus==true )
		{
			_winList[i]._pWin->_isFocus = false;
			_winList[i]._pWin->HandleMessage( WM_KILLFOCUS, NULL, NULL );
		}
	}
}

void UIContainer::SendMessageToBindWin( UINT message, WPARAM wParam, LPARAM lParam )
{
	if( _isBindDUI==true ) 
		UIPostMessage( _pBindDUIWin, message, wParam, lParam );
	else 
		::PostMessage( _bindWindowHwnd, message, wParam, lParam );
}

UIWinBase* UIContainer::GetMinZChild( const POINT& pt )
{
	VECTOR_INT hoverIndexList;

	// 找到所有hover的子窗口
	for( UINT i=0; i<_winList.size(); ++i )
	{	
		if( _winList[i]._pWin->_isShow==false ) continue;

		if( IsPointInRect()( pt, _winList[i]._pWin->GetAbusoluteRect() )==true )
			hoverIndexList.push_back( i );
	}
	// 对悬停的所有子窗口进行处理
	if( hoverIndexList.size()==0 ) return NULL;
	
	// 寻找布局层次最高的子窗口  若相同的则返回最先找到的一个
	UIWinBase* pMinZWin = _winList[hoverIndexList[0]]._pWin;
	for( UINT i=1; i<hoverIndexList.size(); ++i )
	{
		if( _winList[hoverIndexList[i]]._pWin->_z < pMinZWin->_z )
			pMinZWin = _winList[hoverIndexList[i]]._pWin;
	}

	return pMinZWin;
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
		}
		break;
	case WM_DESTROY:
		{	// 形成嵌套调用删除子窗口
			for( UINT i=0; i<_winList.size(); ++i )
				_winList[i]._pWin->HandleMessage( WM_DESTROY, 0, 0 );
		}
		break;
	case WM_REGANIMATE:
		{
			UIAnimateBase* pAnimate = (UIAnimateBase*)wParam;
			UIAnimateManage::GetSingleton()->AddAnimate( (UIAnimateBase*)wParam );
			pAnimate->InitAnimate();
		}
		break;
	case WM_SIZERESET:
		{
			RECT rc;
			if( ::GetClientRect( UIWinShell::_pWinShell->_hWnd, &rc )==TRUE )
			{
				ULONG width = rc.right-rc.left; 
				ULONG height = rc.bottom-rc.top; 

				if( UIDXInit::GetSingleton()->_pIDevice!=NULL )
					UIRender::GetSingleton()->AutoAdjustSizeChange( width, height );
			}
			isMsgHandled = true;
		}
		break;
	case WM_SIZE:
		{
			//SetUnFocusAll();

			SIZE size;
			size.cx = LOWORD( lParam );
			size.cy = HIWORD( lParam );

			// 计算子窗口的缩放
			for( UINT i=0; i<_winList.size(); ++i )
			{
				RECT newRect;
				if( _winList[i]._layoutInfor._zoomModeflag==UILayoutCalc::NO_ZOOM )
					newRect = _winList[i]._pWin->GetRelativeRect();
				else
					newRect = _winList[i]._layoutInfor.CalcLayout( size.cx, size.cy );

				_winList[i]._pWin->MoveWindowDirect( newRect );
			}
		}
		break;
	//case WM_MOUSEHOVER:
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD( lParam );
			pt.y = HIWORD( lParam );

			// 记录当前点为前一点 下一次使用
			POINT prePoint = _preMousePt;
			_preMousePt = pt;						

			// 找到鼠标刚离开的窗口
			VECTOR_INT leaveIndexList;
			for( UINT i=0; i<_winList.size(); ++i )
			{	
				if( _winList[i]._pWin->_isShow==false ) continue;

				if( IsPointInRect()( pt, _winList[i]._pWin->GetAbusoluteRect() )==false )
				{
					if( IsPointInRect()( prePoint, _winList[i]._pWin->GetAbusoluteRect() )==true )
						leaveIndexList.push_back( i );
				}
			}
			// 对鼠标离开的所有子窗口进行处理
			if( leaveIndexList.size()!=0 )
			{	// 寻找布局层次最高的子窗口
				pMinZWin = _winList[leaveIndexList[0]]._pWin;
				for( UINT i=1; i<leaveIndexList.size(); ++i )
				{
					if( _winList[leaveIndexList[i]]._pWin->_z < pMinZWin->_z )
						pMinZWin = _winList[leaveIndexList[i]]._pWin;
				}
				pMinZWin->HandleMessage( WM_MOUSELEAVE, wParam, lParam );
			}

			// 找到鼠标所在布局层次最高的子窗口 传递WM_MOUSEMOVE消息
			pMinZWin = GetMinZChild( pt );
			if( pMinZWin!=NULL )
			{
				if( !ComparePoint()( pt, prePoint ) ) 
					isMsgHandled = pMinZWin->HandleMessage( WM_MOUSEMOVE, wParam, lParam );
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			UIWinBase* pChild = GetFocusOnChild();
			if( pChild==NULL ) break;

			isMsgHandled = pChild->HandleMessage( WM_MOUSEWHEEL, wParam, lParam );
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD( lParam );
			pt.y = HIWORD( lParam );
	
			pMinZWin = GetMinZChild( pt );
			if( pMinZWin!=NULL )
			{
				SetFocusOnChild( pMinZWin );
				isMsgHandled = pMinZWin->HandleMessage( WM_LBUTTONDOWN, wParam, lParam );
			}
			else
			{
				SetUnFocusAll();
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD( lParam );
			pt.y = HIWORD( lParam );

			pMinZWin = GetMinZChild( pt );
			if( pMinZWin!=NULL )
				isMsgHandled = pMinZWin->HandleMessage( WM_LBUTTONUP, wParam, lParam );
		}
		break;
	case WM_RBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD( lParam );
			pt.y = HIWORD( lParam );

			pMinZWin = GetMinZChild( pt );
			if( pMinZWin!=NULL )
			{
				SetFocusOnChild( pMinZWin );
				isMsgHandled = pMinZWin->HandleMessage( WM_RBUTTONDOWN, wParam, lParam );
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD( lParam );
			pt.y = HIWORD( lParam );

			pMinZWin = GetMinZChild( pt );
			if( pMinZWin!=NULL )
				isMsgHandled = pMinZWin->HandleMessage( WM_RBUTTONUP, wParam, lParam );
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD( lParam );
			pt.y = HIWORD( lParam );

			pMinZWin = GetMinZChild( pt );
			if( pMinZWin!=NULL )
				isMsgHandled = pMinZWin->HandleMessage( WM_LBUTTONDBLCLK, wParam, lParam );
		}
		break;
	case WM_KEYDOWN:
		{
			UIWinBase* pChild = GetFocusOnChild();
			if( pChild==NULL ) break;

			if( !( wParam==0x03 || wParam==0x16 || wParam==0x18 ) )
				pChild->HandleMessage( WM_KEYDOWN, wParam, 0 );

			//isMsgHandled = true;
		}
		break;
	case WM_CHAR:
		{
			UIWinBase* pChild = GetFocusOnChild();
			if( pChild==NULL ) break;

			if( wParam==0x03 )
				pChild->HandleMessage( WM_COPY, 0, 0 );
			else if( wParam==0x16 )
				pChild->HandleMessage( WM_PASTE, 0, 0 );
			else if( wParam==0x18 )
				pChild->HandleMessage( WM_CUT, 0, 0 );
			else
				pChild->HandleMessage( WM_CHAR, wParam, 0 );

			isMsgHandled = true;
		}
		break;
	case WM_COPY:
		{
			UIWinBase* pChild = GetFocusOnChild();
			if( pChild==NULL ) break;

			isMsgHandled = pChild->HandleMessage( WM_COPY, 0, 0 );
		}
		break;
	case WM_PASTE:
		{
			UIWinBase* pChild = GetFocusOnChild();
			if( pChild==NULL ) break;

			isMsgHandled = pChild->HandleMessage( WM_PASTE, 0, 0 );
		}
		break;
	case WM_CUT:
		{
			UIWinBase* pChild = GetFocusOnChild();
			if( pChild==NULL ) break;

			isMsgHandled = pChild->HandleMessage( WM_CUT, 0, 0 );
		}
		break;
	case WM_KILLFOCUS:
		{
			for( UINT i=0; i<_winList.size(); ++i )
			{
				if( _winList[i]._pWin->_isFocus==true )
				{
					_winList[i]._pWin->_isFocus = false;
					_winList[i]._pWin->HandleMessage( message, wParam, lParam );
				}
			}
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
			_winList[i]._pWin->Draw();				// 每个窗口进行绘制
	}
}
