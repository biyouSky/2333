#include "stdafx.h"
#include "UIWidget.h"
#include "UIWinShell.h"

using namespace std;


UILable::UILable()
{
	_color = _BLACK_;
	_fontHeight = 20;
}

void UILable::Draw()
{
	//JudgeIsCalcArea();
	UI2DFont fontHelp( _z, _fontHeight );
	fontHelp( _str, GetAbusoluteRect(), UI2DFont::FONT_LEFT, _color );
}

void UILable::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		_str = _strCache;
	}
	else if( flag==2 )
	{
		_color = (D3DCOLOR)data;
	}
	else if( flag==3 )
	{
		_fontHeight = (LONG)data;
	}
}

void UILable::SetString( std::string str )
{
	_strCache = str;
	UIPostMessage( this, WM_UPDATE, 1, 0 );
}

void UILable::GetString( std::string& str )
{
	str = _str;
}

void UILable::SetColor( D3DCOLOR color )
{
	UIPostMessage( this, WM_UPDATE, 2, color );
}

void UILable::SetFontHeight( LONG h )
{
	UIPostMessage( this, WM_UPDATE, 3, h );
}

void UIImage::Draw()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	int centerX = GetWidth()( _clientRC )/2+x_;
	int centerY = GetHeight()( _clientRC )/2+y_;

	if( !IsRunAnimate() )
	{
		UI2DImage image;
		if( _loadImageWay==1 )
			image.Init( _dllPath.c_str(), _imageID, _z, _colorKey );
		else if( _loadImageWay==2 )
			image.Init( _imagePath, _z, _colorKey );

		image.operator()( centerX, centerY );
	}
	else
	{
		DrawHitDrumAnimate( centerX, centerY, _z );
	}
}

// 设置函数
void UIImage::PlayAnimateHitDrum()
{
	PlayAnimate( UIAnimateEffect::HIT_DRUM );
}





UIButton::UIButton() 
{
	SetHitPower( 0.3f );

	_isHover = false;
	_isLButtonDown = false;
}

void UIButton::Draw()
{
	//JudgeIsCalcArea();

	IDirect3DDevice9* pIDevice = UIDXInit::GetSingleton()->_pIDevice;
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	if( !IsRunAnimate() )
	{
		// 状态图
		int frameBMPID = IDB_BUTTON1_NORMAL;
		if( _isHover==true )
			frameBMPID = IDB_BUTTON1_HOT;
		if( _isLButtonDown==true )
			frameBMPID = IDB_BUTTON1_DOWN;

		// 按钮贴图
		DrawFrameHelp( "GUIResource.dll", frameBMPID, 3, 3, 3, 3, _z, D3DCOLOR_XRGB(255, 0, 255) )
					 ( x_, y_, GetWidth()( _clientRC ), GetHeight()( _clientRC ) );
	}
	else
	{
		RECT rc = _clientRC;
		OffsetRect( &rc, x_, y_ );
		DrawFrameHitDrumAnimate( rc, 3, _z );
	}

	// 显示string
	UI2DFont fontHelp( _z );
	RECT rc;
	if( _isLButtonDown==false )
		rc = _strRect;
	else
		rc = _strRect2;
	OffsetRect( &rc, x_, y_ );
	fontHelp( _windowText, rc );
}

void UIButton::CalcArea()
{
	// 计算常态时字符串显示区域
	_strRect.left = _clientRC.left+1;
	_strRect.right = _clientRC.right - 1;
	_strRect.top = _clientRC.top + 1;
	_strRect.bottom = _clientRC.bottom - 1;

	// 计算按下时字符串显示区域
	_strRect2 = _strRect;
	_strRect2.left += 1;
	_strRect2.right += 1;
	_strRect2.top += 1;
	_strRect2.bottom += 1;
}

void UIButton::SetString( std::string winString )
{
	Cache1* pCache = new Cache1;
	pCache->_str = winString;
	UIPostMessage( this, WM_UPDATE, 0, (LPARAM)pCache );
}

void UIButton::GetString( std::string& winString )
{
	winString = _windowText;
}

bool UIButton::OnMouseMove( POINT pt ) 
{
	if( !_isHover ) 
	{
		_isHover = true;
		UIRefresh();
	}
	
	return true;
}

void UIButton::OnMouseLeave( POINT pt ) 
{  
	_isHover = false;
	_isLButtonDown = false;
	UIRefresh();
}

bool UIButton::OnLButtonDown( POINT pt ) 
{
	_isLButtonDown = true;

	UIRefresh();
	return true;
}

bool UIButton::OnLButtonUp( POINT pt ) 
{ 
	if( !_isLButtonDown )
		return true;

	_isLButtonDown = false;
	SendMessageToParent( WM_NOTIFY, (WPARAM)_id, 0 );

	SetImageIDAndDLL( IDB_BUTTON1_HOT, "GUIResource.dll", D3DCOLOR_XRGB(255, 0, 255) );
	PlayAnimate( UIAnimateEffect::HIT_DRUM );

	return true;
}

void UIButton::OnKeyDown( TCHAR nChar )
{
	if( nChar==VK_RETURN )
	{
		SendMessageToParent( WM_NOTIFY, (WPARAM)_id, 0 );

		SetImageIDAndDLL( IDB_BUTTON1_HOT, "GUIResource.dll", D3DCOLOR_XRGB(255, 0, 255) );
		PlayAnimate( UIAnimateEffect::HIT_DRUM );
	}
}

void UIButton::OnUpdate( UINT flag, void* data )
{
	if( flag==0 )
	{
		Cache1* pCache = (Cache1*)data;

		_windowText = pCache->_str;
		delete pCache;
	}
}





UICheckButton::UICheckButton()
{
	_isHover = false;
	_isCheck = false;
}

void UICheckButton::Draw()
{
	//JudgeIsCalcArea();

	int checkBoxBMPID;
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画check框
	if( _isCheck && !_isHover )
		checkBoxBMPID = IDB_CHECKBOX1_TICK_NOR;
	else if( !_isCheck && !_isHover ) 
		checkBoxBMPID = IDB_CHECKBOX1_NOR;
	else if( _isCheck && _isHover )
		checkBoxBMPID = IDB_CHECKBOX1_TICK_HOT;
	else
		checkBoxBMPID = IDB_CHECKBOX1_HOT;

	// 按钮贴图
	if( !_isHover )
		UI2DImage( "GUIResource.dll", checkBoxBMPID, _z, D3DCOLOR_XRGB(255, 0, 255) ) ( GetWidth()( _checkRect )/2+x_, GetHeight()( _checkRect )/2+y_ );
	else
		UI2DImage( "GUIResource.dll",checkBoxBMPID,  _z ) ( GetWidth()( _checkRect )/2+x_, GetHeight()( _checkRect )/2+y_ );

	// 显示string
	RECT rc = _strRect;
	OffsetRect( &rc, x_, y_ );
	UI2DFont fontHelp( _z );
	fontHelp( _windowText, rc, UI2DFont::FONT_LEFT );
}

void UICheckButton::CalcArea()
{
	//
	_checkRect = _clientRC;
	_checkRect.right = 30;

	// 计算常态时字符串显示区域
	_strRect.left = _checkRect.right;
	_strRect.right = _clientRC.right;
	_strRect.top = _clientRC.top ;
	_strRect.bottom = _clientRC.bottom ;
}

void UICheckButton::SetString( std::string winString )
{
	Cache1* pCache = new Cache1;
	pCache->_str = winString;
	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UICheckButton::GetString( std::string& winString )
{
	winString = _windowText;
}

void UICheckButton::SetCheck( bool f )
{
	if( f!=_isCheck )
		UIPostMessage( this, WM_UPDATE, 2, f?1:0 );
}

void UICheckButton::GetCheck( bool& f )
{
	f = _isCheck;
}

void UICheckButton::SetMutexList( vector<UICheckButton*> mutexList )
{
	Cache2* pCache = new Cache2;
	pCache->_mutexList = mutexList;
	UIPostMessage( this, WM_UPDATE, 3, (LPARAM)pCache );
}

void UISetCheckButtonMutex( vector<UICheckButton*>& mutexList )
{
	for( UINT i=0; i<mutexList.size(); ++i )
	{
		vector<UICheckButton*> listTemp = mutexList;
		listTemp.erase( listTemp.begin()+i );

		mutexList[i]->SetMutexList( listTemp );
	}
}

void UISetCheckButtonMutex( UICheckButton* but1, UICheckButton* but2 )
{
	vector<UICheckButton*> mutexList;
	mutexList.push_back( but1 );
	mutexList.push_back( but2 );

	UISetCheckButtonMutex( mutexList );
}

bool UICheckButton::OnMouseMove( POINT pt )
{
	if( !_isHover )
		_isHover = true;
	
	UIRefresh();
	return true;
}

void UICheckButton::OnMouseLeave( POINT pt )
{
	_isHover = false;
	UIRefresh();
}

bool UICheckButton::OnLButtonDown( POINT pt )
{
	// 互斥绑定的button
	for( UINT i=0; i<_mutexList.size(); ++i )
		_mutexList[i]->_isCheck = _isCheck;

	_isCheck = !_isCheck ;
	SendMessageToParent( WM_NOTIFY, (WPARAM)_id, _isCheck?1:0 );

	UIRefresh();
	return true;
}

bool UICheckButton::OnLButtonDbClk( POINT pt )
{
	return OnLButtonDown( pt );
}

void UICheckButton::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		Cache1* pCache = (Cache1*)data;
		_windowText = pCache->_str;
		delete pCache;
	}
	else if( flag==2 )
	{
		_isCheck = (int)data==1 ? true:false;
	}
	else if( flag==3 )
	{
		Cache2* pCache = (Cache2*)data;
		_mutexList = pCache->_mutexList;
		delete pCache;
	}
}



UIEdit::UIEdit()
{
	_beginDrawXPos = false;									// 开始绘制的x坐标

	_isDrawCaretImmd = false;
	_caretPassChar = 0;										// 插入符

#ifndef _UNICODE
	_isCNInput = false; 
#endif

	_isSelArea = false;										// 是否选中区域
	_beginAreaPassChar = 0;									// 选中区域开始pass的字符

	_fontHeight = 20;
	_fontColor = _BLACK_;
}

void UIEdit::Draw()
{
	//JudgeIsCalcArea();

	UI2DFont fontHelp( _z, _fontHeight );
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画边框
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEBLUE_ );

	// 绘制内容
	// 计算并选定剪裁区域
	UI2DClipRect uiClip;
	RECT clipRC = _drawRectAllow;
	OffsetRect( &clipRC, x_, y_ );

	// 获得字符串的大小信息
	SIZE contentSize = fontHelp.GetDrawAreaSize( _strContent.c_str() );
	SIZE caretPassCharSize = fontHelp.GetDrawAreaSize( _strContent.substr( 0, _caretPassChar ).c_str() );
	SIZE beginAreaPassSize = fontHelp.GetDrawAreaSize( _strContent.substr( 0, _beginAreaPassChar ).c_str() );

	// 计算绘制区域
	_drawRectReal.left = _drawRectAllow.left + _beginDrawXPos;
	_drawRectReal.right = _drawRectReal.left + contentSize.cx;
	
	if( _isFocus )
	{	
		// 计算caret位置
		int xPos = caretPassCharSize.cx+_beginDrawXPos+5;
		int yPos = ( _drawRectAllow.top+_drawRectAllow.bottom )/2;

		// 绘制caret
		UISetCaretPos( xPos+x_, yPos+y_, _isDrawCaretImmd );
		_isDrawCaretImmd = false;

		// 画选中区域背景色
		if( _isSelArea )
		{
			uiClip.BeginClipRect( clipRC );

			int xPos2 = beginAreaPassSize.cx+_beginDrawXPos+5; 
			rc = GenRect()( xPos2<xPos?xPos2:xPos, _drawRectAllow.top+2, xPos>xPos2?xPos:xPos2, _drawRectAllow.bottom-2 );
			OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );
			UI2DRect( rc, _z )( D3DCOLOR_XRGB(187, 233, 255), D3DCOLOR_XRGB(255, 233, 187), 180 );

			uiClip.EndClipRect();
		}
	}

	// 画string
	{
		uiClip.BeginClipRect( clipRC );
	
		RECT drawRect = _drawRectReal;
		OffsetRect( &drawRect, x_, y_ );
		fontHelp( _strContent, drawRect, UI2DFont::FONT_LEFT, _fontColor );

		uiClip.EndClipRect();
	}
}

void UIEdit::CalcArea()
{
	// 计算绘制区域
	_drawRectAllow = _clientRC;

	_drawRectAllow.left += 5;
	_drawRectAllow.right -= 5;
	_drawRectAllow.top += 2;
	_drawRectAllow.bottom -= 2;

	_drawRectReal = _drawRectAllow;
}

void UIEdit::SetString( std::string winString )
{
	Cache1* pCache = new Cache1;
	MByteToWChar()( winString, pCache->_wStr );
	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UIEdit::GetString( std::string& winString )
{
	WCharToMByte()( _strContent, winString );
}

void UIEdit::EraseSelectArea()
{
	if( _isSelArea==false ) return;

	if( _caretPassChar>_beginAreaPassChar )
	{
		int dx = _caretPassChar-_beginAreaPassChar;
		_strContent.erase( _beginAreaPassChar, dx );
		_caretPassChar -= dx;
	}
	else if( _caretPassChar<_beginAreaPassChar )
	{
		_strContent.erase( _caretPassChar, _beginAreaPassChar-_caretPassChar );
	}

	_isSelArea = false;
	CalcBeginDrawXPos();
}

// 计算绘制起始字符x坐标 _beginDrawXPos与_caretPassChar一起决定光标位置
void UIEdit::CalcBeginDrawXPos()
{
	UI2DFont fontHelp( _z, _fontHeight );

	if( _caretPassChar==0 )
	{	// 初始化值 将光标置于_drawRectAllow的起点
		_beginDrawXPos = 0;
	}
	else if( _caretPassChar==_strContent.size() )
	{	// 将光标置于字符串的最后
		SIZE contentSize = fontHelp.GetDrawAreaSize( _strContent.c_str() );

		if( contentSize.cx<GetWidth()( _drawRectAllow ) ) 
			_beginDrawXPos = 0;
		else 
			_beginDrawXPos = -( contentSize.cx - GetWidth()( _drawRectAllow ) );
	}
	else	
	{	// 修正当前值 一般对应键盘的left right键
		// 计算插入符截取的字符串长度
		SIZE caretPassSize = fontHelp.GetDrawAreaSize( _strContent.substr( 0, _caretPassChar ).c_str() );

		// 计算截取字符串最后个字符位置
		int lastCharPos = caretPassSize.cx+_beginDrawXPos;
		// 修正
		if( lastCharPos<0 )											// 小于_drawRectAllow  将光标置于_drawRectAllow的起点
		{
			_beginDrawXPos -= lastCharPos;
		}
		else if( lastCharPos>GetWidth()( _drawRectAllow ) )			// 大于_drawRectAllow  将光标置于_drawRectAllow的终点
		{
			_beginDrawXPos -= lastCharPos-GetWidth()( _drawRectAllow );
		}
		// else  位于_drawRectAllow内 _beginDrawXPos保持不变
	}
}

void UIEdit::SelectAllText()
{
	UIPostMessage( this, WM_UPDATE, 2, 0 );
}

void UIEdit::CalcCaretPassChar( POINT& point )
{
	UI2DFont fontHelp( _z, _fontHeight );

	// 计算绝对位置点
	POINT pt = point;
	pt.x -= _abusolutePoint.x;
	pt.y -= _abusolutePoint.y;

	// 获得字符串的大小信息
	int preSizeCX = 0;

	if( pt.x<=_drawRectAllow.left )					// 点在允许绘制区域的前面
	{
		_caretPassChar = 0;
		_beginDrawXPos = 0;
	}
	else if( pt.x>=_drawRectAllow.right )			// 点在允许绘制区域的后面
	{
		_caretPassChar = _strContent.size();
		CalcBeginDrawXPos();
	}
	else											// 点在允许绘制区域的里面
	{		
		preSizeCX = 0;
		UINT i;
		for( i=0; i<=_strContent.size(); ++i )
		{
			// 截取[0-i]字符串
			SIZE contentSize = fontHelp.GetDrawAreaSize( _strContent.substr( 0, i ).c_str() );

			// 超过截取字符串 继续计算
			if( pt.x >= _drawRectAllow.left+_beginDrawXPos+contentSize.cx )
			{ 
				preSizeCX = contentSize.cx;
				continue;
			}
			
			// 未超过则计算i-1与i中偏向哪一个
			if( pt.x <= _drawRectAllow.left+_beginDrawXPos+preSizeCX+( contentSize.cx-preSizeCX )/2 )
				_caretPassChar = i-1;	
			else 
				_caretPassChar = i;
			break;
		}
		// 
		if( i>_strContent.size() )
			_caretPassChar = _strContent.size();
	}
}

void UIEdit::OnDestroy()
{
	if( _isFocus )
		UIHideCaret();
}

void UIEdit::OnSetFocus()
{
	UIShowCaret( _z, 1, _fontHeight+4, _BLUE_ );
}

void UIEdit::OnKillFocus()
{
	UIHideCaret();
	UIRefresh();
}

bool UIEdit::OnLButtonDown( POINT pt )
{		
	// 计算Caret位置
	CalcCaretPassChar( pt );
	_isDrawCaretImmd = true;
	
	_isSelArea = false;

	UIRefresh();
	return true;
}

bool UIEdit::OnLButtonDbClk( POINT pt )
{
	SelectAllText();

	UIRefresh();
	return true;
}

bool UIEdit::OnMouseMove( POINT pt )
{
	//::SetCursor( ::LoadCursor( NULL, IDC_IBEAM ) );

	if( IsKeyDown()( VK_LBUTTON )==true )
	{
		if( _isSelArea==false )
		{
			_isSelArea = true;
			_beginAreaPassChar = _caretPassChar;
		}

		if( _isSelArea==true )
		{
			CalcCaretPassChar( pt );
		}

		UIRefresh();
	}
	
	return true;
}

void UIEdit::OnKeyDown( TCHAR nChar )
{
	if( nChar==VK_LEFT || nChar==VK_RIGHT || nChar==VK_HOME || nChar==VK_END )
	{
		if( IsKeyDown()( VK_SHIFT )==true )
		{
			if( _isSelArea==false )
			{
				_isSelArea = true;
				_beginAreaPassChar = _caretPassChar;
			}
		}
		else
		{
			_isSelArea = false;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		{
			if( _caretPassChar==0 ) 
				return;
			
			--_caretPassChar;
			CalcBeginDrawXPos();
			_isDrawCaretImmd = true;
		}
		break;
	case VK_RIGHT:
		{
			if( _caretPassChar==_strContent.size() )
				return;

			++_caretPassChar;
			CalcBeginDrawXPos();
			_isDrawCaretImmd = true;
		}
		break;
	case VK_HOME:
		{
			if( _caretPassChar==0 )
				return;

			_caretPassChar = 0;
			_beginDrawXPos = 0;
			_isDrawCaretImmd = true;
		}
		break;
	case VK_END:
		{
			if( _caretPassChar==_strContent.size() )
				return;
			
			_caretPassChar = _strContent.size();
			CalcBeginDrawXPos();
			_isDrawCaretImmd = true;
		}
		break;
	case VK_DELETE:
		{
			if( _isSelArea )
			{
				EraseSelectArea();
			}
			else if( _caretPassChar<_strContent.size() )
			{
				_strContent.erase( _caretPassChar, 1 );
			}
			else
			{
				return;
			}

			_isDrawCaretImmd = true;
		}
		break;
	case VK_BACK:
		{
			if( _isSelArea )
			{
				EraseSelectArea();
			}
			else if( _caretPassChar>=1 )
			{
				_strContent.erase( --_caretPassChar, 1 );
				CalcBeginDrawXPos();	
			}
			else
			{
				return;
			}

			_isDrawCaretImmd = true;
		}
		break;
	case (TCHAR)VK_PROCESSKEY:  // 输入法 输入处理 
		{
			_isCNInput = true;
		}
		break;
	default:
		_isCNInput = false;
		break;
	};

	UIRefresh();
}

void UIEdit::OnChar( TCHAR nChar )
{	
	switch( nChar )
	{
	case VK_BACK:
		break;
	default:
		{	
			if( _isSelArea )
				EraseSelectArea();

			// 插入字符
#ifdef _UNICODE
			_strContent.insert( _caretPassChar, 1, nChar );
			++_caretPassChar;
#else		
			if( _isCNInput==false )
			{
				_strContent.insert( _caretPassChar, 1, ( wchar_t )nChar );
				++_caretPassChar;
			}
			else // 输入法输入字符
			{
				// 加到缓冲字符串中去
				_CNStrCache += nChar;
				// 缓冲字符串长度为2则进行转换
				if( _CNStrCache.size()==2 )
				{
					wstring wStr;
					MByteToWChar()( _CNStrCache, wStr );
					_strContent.insert( _caretPassChar, wStr );
					_caretPassChar += wStr.size();
					_CNStrCache.clear();
				}
			}
#endif			
			// 计算新的绘制起始字符x坐标
			CalcBeginDrawXPos();
		}
		break;
	};

	UIRefresh();
}

bool UIEdit::OnPaste()
{	
	std::wstring strBuf;
	StringPasteFromClipboard()( strBuf, UIWinShell::_pWinShell->_hWnd );

	// 修改内容
	if( _isSelArea ) 
		EraseSelectArea();
	//
	_strContent.insert( _caretPassChar, strBuf );
	_caretPassChar += strBuf.size();

	CalcBeginDrawXPos(); 
	UIRefresh();
	return true;
}

bool UIEdit::OnCopy()
{
	if( _isSelArea==false || _beginAreaPassChar==_caretPassChar ) 
		return true;

	StringCopyToClipboard()( wstring( _caretPassChar>_beginAreaPassChar?&_strContent[_beginAreaPassChar]:&_strContent[_caretPassChar], 
							 abs( (int)_caretPassChar-(int)_beginAreaPassChar ) ), UIWinShell::_pWinShell->_hWnd );
	return true;
}

bool UIEdit::OnCut()
{
	OnCopy();
	if( _isSelArea ) 
		EraseSelectArea();

	UIRefresh();
	return true;
}

void UIEdit::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		Cache1* pCache = (Cache1*)data;

		_caretPassChar = 0;
		_isSelArea = false;
		CalcBeginDrawXPos();

		_strContent = pCache->_wStr;
		delete pCache;
	}
	else if( flag==2 )
	{
		_isSelArea = true;
		_beginAreaPassChar = 0;
		_caretPassChar = _strContent.size();
		_isDrawCaretImmd = true;

		CalcBeginDrawXPos();
	}
}



UISelectList::UISelectList()
{
	_selectIndex = -1;
	_hoverIndex = -1;
	_isHover = false;
}

void UISelectList::Draw()
{
	//JudgeIsCalcArea();

	// 计算绘制的区域
	int dh = 20*_list.size();
	RECT parentRC = _pRealParent->GetAbusoluteRect();
	RECT abusoluteRC =	parentRC.bottom+dh<UIWinShell::_pWinShell->GetHeight() ? 
						GenRect()( parentRC.left, parentRC.bottom, parentRC.right, parentRC.bottom+dh ) : 
						GenRect()( parentRC.left, parentRC.top-dh, parentRC.right, parentRC.top );
	MoveWindowDirect( abusoluteRC );

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画边框
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEWHITE_, 255 );
	UI2DRect( rc, _z )( _FRAMEBLUE_ );

	// 画hover
	if( _hoverIndex !=-1 )
	{
		RECT rc = _clientRC;
		rc.top = _hoverIndex*20;
		rc.bottom = ( _hoverIndex+1 )*20;
		++rc.left;
		++rc.top;
		--rc.right;
		--rc.bottom;
		OffsetRect( &rc, x_, y_ );

		DrawFrameHelp( "GUIResource.dll", IDB_HOT_EFFECT2, 2, 2, 2, 2, _z, D3DCOLOR_XRGB(255, 0, 255) )( rc, 255 ); 
	}

	// 画内容
	for( UINT i=0; i<_list.size(); ++i )
	{
		RECT rc = _clientRC;
		rc.left += 5;
		rc.top = i*20;
		rc.bottom = rc.top+20;
		OffsetRect( &rc, x_, y_ );

		// 运行动画效果时
		if( IsRunAnimate() )
		{
			int width = GetWidth()( rc )+i*10;

			int dx = width/_maxFrame;
			rc.left += dx*( _maxFrame-_frameCount );
		}

		string str = _list[i];
		if( str=="" )
			str = "NULL";
		UI2DFont( _z, 16 )( str.c_str(), rc, UI2DFont::FONT_LEFT );
	}
}

void UISelectList::AddString( std::string str )
{
	Cache1* pCache = new Cache1;
	pCache->_string = str;

	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UISelectList::GetString( int index, std::string& str )
{
	str = _list[index];
}

UINT UISelectList::GetListCount()
{
	return _list.size();
}

void UISelectList::ClearList()
{
	UIPostMessage( this, WM_UPDATE, 2, 0 );
}

bool UISelectList::OnMouseMove( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	int oldIndex = _hoverIndex;

	_hoverIndex = point.y/20;
	if( _hoverIndex>=(int)_list.size() )
		_hoverIndex = (int)_list.size()-1;

	if( oldIndex!=_hoverIndex )
		UIRefresh();
	return true;
}

void UISelectList::OnMouseLeave( POINT pt )
{
	_hoverIndex = -1;
}

bool UISelectList::OnLButtonDown( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	_selectIndex = point.y/20;
	UIPostMessage( _pRealParent, WM_NOTIFY, _id, _selectIndex );
	UIRefresh();
	return true;
}

void UISelectList::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		Cache1* pCache = (Cache1*)data;
		_list.push_back( pCache->_string );
		delete pCache;
	}
	else if( flag==2 )
	{
		_list.clear();

		_selectIndex = -1;
		_hoverIndex = -1;
		_isHover = false;
	}
}



UIComboBox::UIComboBox()
{
	_isDrawBoader = true;
	_isDropDown = false;
	_isHover = false;

	_selectIndex = -1;
}

void UIComboBox::Draw()
{
	//JudgeIsCalcArea();

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画边框
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	if( _isDrawBoader ) 
		UI2DRect( rc, _z )( !_isHover ? _FRAMEBLUE_:_GOLD_ );

	// 画显示的内容
	if( _selectIndex!=-1 )
	{
		string str;
		_dropDownList.GetString( _selectIndex, str );

		rc.left += 5;
		UI2DFont font( _z, 20 );
		font( str, rc, UI2DFont::FONT_LEFT );
	}

	// 
	rc.left = rc.right-34;
	rc.right = rc.left+32;
	rc.top += GetHeight()( rc )/2-16;
	rc.bottom = rc.top+32;
	UI2DImage( "GUIResource.dll", IDB_ARROW1_DOWN, _z )( rc );
}

void UIComboBox::AddString( std::string str )
{
	_dropDownList.AddString( str );
}

bool UIComboBox::GetString( std::string& str )
{
	if( _selectIndex==-1 ) return false;

	_dropDownList.GetString( _selectIndex, str );
	return true;
}

void UIComboBox::SetSelectIndex( int index )
{
	UIPostMessage( this, WM_UPDATE, 1, index );
}

void UIComboBox::ClearList()
{
	_dropDownList.ClearList();
}

int UIComboBox::GetSelectIndex()
{
	return _selectIndex;
}

void UIComboBox::IsDrawBoader( bool flag )
{
	_isDrawBoader = flag;
}

void UIComboBox::SetDropDown( bool flag )
{
	if( _isDropDown==flag ) return;

	UINT listCount = _dropDownList.GetListCount();
	if( listCount==0 ) return;

	_isDropDown = flag;
	_dropDownList.ShowWindowDirect( flag );

	if( flag )
		_dropDownList.PlayAnimate( 0, 6 );
}

void UIComboBox::OnCreate()
{
	_dropDownList._id = 0;
	_dropDownList.CreatePopup( this, _NULLRECT_, 0, false );
}

void UIComboBox::OnDestroy()
{
	_dropDownList.DestroyDirect();
}

void UIComboBox::OnKillFocus()
{
	_isHover = false;
	SetDropDown( false );

	UIRefresh();
}

bool UIComboBox::OnMouseMove( POINT pt )
{
	if( _isHover==false )
	{
		_isHover = true;
		UIRefresh();
	}

	return true;
}

void UIComboBox::OnMouseLeave( POINT pt )
{
	_isHover = false;
	UIRefresh();
}

bool UIComboBox::OnLButtonDbClk( POINT pt )
{
	return OnLButtonDown( pt );
}

bool UIComboBox::OnLButtonDown( POINT pt )
{
	SetDropDown( !_isDropDown );
	UIRefresh();

	return true;
}

void UIComboBox::OnNotify( int id, int param )
{
	if( id==0 )
	{
		_selectIndex = param;
		SetDropDown( false );
		_isHover = false;

		SendMessageToParent( WM_NOTIFY, _id, _selectIndex );
	}
}

void UIComboBox::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
		_selectIndex = (int)data;

	UIRefresh();
}




UIScrollBar::UIScrollBar( int coordFlag )
{
	_coordFlag = coordFlag;
	_pageScale = 0.2;
	_posScale = 0;
}

void UIScrollBar::Draw()
{
	JudgeIsCalcArea();

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画边框
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEBLUE_ );

	//
	rc = _firstRect;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEPREPLE_, 255 );

	//
	rc = _secondRect;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEPREPLE_, 255 );

	//
	rc = _barRect;
	if( GetWidth()( rc )<8 || GetHeight()( rc )<8 )
	{
		if( _coordFlag==0 )
		{
			if( _posScale<0.5 )
				rc.right = rc.left+8;
			else
				rc.left = rc.right-8;
		}
		else
		{
			if( _posScale<0.5 )
				rc.bottom = rc.top+8;
			else
				rc.top = rc.bottom-8;
		}
	}
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( D3DCOLOR_XRGB(203, 196, 231), 255 );
}

void UIScrollBar::CalcArea()
{
	if( _coordFlag==0 )
	{
		_firstRect.left = 1;
		_firstRect.right = 15;
		_firstRect.top = 1;
		_firstRect.bottom = _clientRC.bottom-1;

		_secondRect.left = _clientRC.right-15;
		_secondRect.right = _clientRC.right-1;
		_secondRect.top = 1;
		_secondRect.bottom = _clientRC.bottom-1;

		_scrollRect.left = _firstRect.right;
		_scrollRect.right = _secondRect.left;
		_scrollRect.top = 1;
		_scrollRect.bottom = _clientRC.bottom-1;
	}
	else
	{
		_firstRect.left = 1;
		_firstRect.right = _clientRC.right-1;
		_firstRect.top = 1;
		_firstRect.bottom = 15;

		_secondRect.left = 1;
		_secondRect.right = _clientRC.right-1;
		_secondRect.top = _clientRC.bottom-15;
		_secondRect.bottom = _clientRC.bottom-1;

		_scrollRect.left = 1;
		_scrollRect.right = _clientRC.right-1;
		_scrollRect.top = _firstRect.bottom;
		_scrollRect.bottom = _secondRect.top;
	}

	CalcBarArea();
}

void UIScrollBar::CalcBarArea()
{
	if( _coordFlag==0 )
	{
		_barRect.top = _scrollRect.top;
		_barRect.bottom = _scrollRect.bottom;

		_barRect.left = _scrollRect.left + (int)( _posScale*GetWidth()( _scrollRect ) );
		_barRect.right = _barRect.left + (int)( _pageScale*GetWidth()( _scrollRect ) );
	}
	else
	{
		_barRect.left = _scrollRect.left;
		_barRect.right = _scrollRect.right;

		_barRect.top = _scrollRect.top + (int)( _posScale*GetHeight()( _scrollRect ) );
		_barRect.bottom = _barRect.top + (int)( _pageScale*GetHeight()( _scrollRect ) );
	}
}

void UIScrollBar::SetPageScaleDirect( double s )
{
	Cache1* pCache = new Cache1;
	pCache->_v = s;
	OnUpdate( 1, (void*)pCache );
}

void UIScrollBar::SetPosScaleDirect( double s )
{
	Cache1* pCache = new Cache1;
	pCache->_v = s;
	OnUpdate( 2, (void*)pCache );
}

void UIScrollBar::SetPageScale( double s )
{
	Cache1* pCache = new Cache1;
	pCache->_v = s;
	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

double UIScrollBar::GetPageScale()
{
	return _pageScale;
}

void UIScrollBar::SetPosScale( double s )
{
	Cache1* pCache = new Cache1;
	pCache->_v = s;
	UIPostMessage( this, WM_UPDATE, 2, (LPARAM)pCache );
}

double UIScrollBar::GetPosScale()
{
	return _posScale<( 1-_pageScale ) ? _posScale:( 1-_pageScale ) ;
}

void UIScrollBar::SetCoordXY( int coordFlag )
{
	UIPostMessage( this, WM_UPDATE, 3, coordFlag );
}

bool UIScrollBar::OnMouseMove( POINT pt )
{
	// 判断鼠标在控件范围外面LButtonUp的情况
	if( IsKeyDown()( VK_LBUTTON ) )
	{
		POINT point = pt;
		point.x -= _abusolutePoint.x;
		point.y -= _abusolutePoint.y;

		if( !IsWindowFocus() )
			return true;

		if( _coordFlag==0 )
		{
			int dx = point.x-_prePoint.x;
			OffsetRect( &_barRect, dx, 0 );
			if( _barRect.left<_scrollRect.left )
				OffsetRect( &_barRect, _scrollRect.left-_barRect.left, 0 );
			else if( _barRect.right>_scrollRect.right )
				OffsetRect( &_barRect, _scrollRect.right-_barRect.right, 0 );

			_posScale = (double)( _barRect.left-_scrollRect.left )/GetWidth()( _scrollRect );
			SendMessageToParent( WM_HSCROLL, SB_THUMBTRACK, (LPARAM)this );
		}
		else
		{
			int dy = point.y-_prePoint.y;
			OffsetRect( &_barRect, 0, dy );
			if( _barRect.top<_scrollRect.top )
				OffsetRect( &_barRect, 0, _scrollRect.top-_barRect.top );
			else if( _barRect.bottom>_scrollRect.bottom )
				OffsetRect( &_barRect, 0, _scrollRect.bottom-_barRect.bottom );

			_posScale = (double)( _barRect.top-_scrollRect.top )/GetHeight()( _scrollRect );
			SendMessageToParent( WM_VSCROLL, SB_THUMBTRACK, (LPARAM)this );
		}

		_prePoint = point;
		UIRefresh();
	}

	return true;
}

bool UIScrollBar::OnLButtonDown( POINT point )
{
	POINT pt = point;
	pt.x -= _abusolutePoint.x;
	pt.y -= _abusolutePoint.y;
	_prePoint = pt;

	if( IsPointInRect()( pt, _firstRect )==true )
	{
		if( _coordFlag==0 )
			SendMessageToParent( WM_HSCROLL, SB_LINELEFT, (LPARAM)this );
		else
			SendMessageToParent( WM_VSCROLL, SB_LINEUP, (LPARAM)this );
	}
	else if( IsPointInRect()( pt, _secondRect )==true )
	{
		if( _coordFlag==0 )
			SendMessageToParent( WM_HSCROLL, SB_LINERIGHT, (LPARAM)this );
		else
			SendMessageToParent( WM_VSCROLL, SB_LINEDOWN, (LPARAM)this );
	}
	else if( IsPointInRect()( pt, _barRect )==false )
	{
		if( _coordFlag==0 )
		{
			if( pt.x<_barRect.left )
			{
				_posScale -= _pageScale;
				if( _posScale<0 )
					_posScale = 0;
				SendMessageToParent( WM_HSCROLL, SB_PAGELEFT, (LPARAM)this );
			}
			else if( pt.x>_barRect.right )
			{
				_posScale += _pageScale;
				if( _posScale>1-_pageScale )
					_posScale = 1-_pageScale;
				SendMessageToParent( WM_HSCROLL, SB_PAGERIGHT, (LPARAM)this );
			}
		}
		else
		{
			if( pt.y<_barRect.top )
			{
				_posScale -= _pageScale;
				if( _posScale<0 )
					_posScale = 0;
				SendMessageToParent( WM_VSCROLL, SB_PAGEUP, (LPARAM)this );
			}
			else if( pt.y>_barRect.bottom )
			{
				_posScale += _pageScale;
				if( _posScale>1-_pageScale )
					_posScale = 1-_pageScale;
				SendMessageToParent( WM_VSCROLL, SB_PAGEDOWN, (LPARAM)this );
			}
		}
		CalcBarArea();
	}

	UIRefresh();
	return true;
}

bool UIScrollBar::OnLButtonDbClk( POINT pt )
{
	return OnLButtonDown( pt );
}

void UIScrollBar::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		Cache1* pCache = (Cache1*)data;
		_pageScale = pCache->_v;
		delete pCache;

		CalcBarArea();
	}
	else if( flag==2 )
	{
		Cache1* pCache = (Cache1*)data;
		_posScale = pCache->_v;
		delete pCache;

		CalcBarArea();
	}
	else if( flag==3 )
	{
		_coordFlag = (int)data;
		_isCalcArea = true;
	}
}




UIGrid::GridCellInfo::GridCellInfo( std::string str )
{
	_strContent = str;
	_color = _BLACK_;
	_wordPos = UI2DFont::FONT_LEFT;

	// 默认激活时都为edit控件
	_pCtrl = NULL;
	_controlType = EDIT;
	_isHold = false;
}

void UIGrid::GridCellInfo::CreateControl( UIContainer *pUIContainer, int firstRowPos, int firstColumnPos, int id )
{
	if( _pCtrl!=NULL ) 
		DeleteControl();

	if( _controlType==EDIT )
	{
		UIEdit* pControl = new UIEdit;
		_pCtrl = pControl;

		// 创建edit
		pControl->Create( 8000, pUIContainer, GenRect()( _pos.left+firstColumnPos, _pos.top+firstRowPos, _pos.right+1+firstColumnPos, _pos.bottom+1+firstRowPos ), 0, true );
		pControl->SetString( _strContent );
		pControl->SelectAllText();
	}
	else if( _controlType==CHECKBUTTON )
	{
		UICheckButton* pControl = new UICheckButton;
		_pCtrl = pControl;

		pControl->Create( id, pUIContainer, GenRect()( _pos.left+firstColumnPos, _pos.top+firstRowPos, _pos.right+1+firstColumnPos, _pos.bottom+1+firstRowPos ), 0, true );
		pControl->SetString( _strContent );
	}
	else if( _controlType==BUTTON )
	{
		UIButton* pControl = new UIButton;
		_pCtrl = pControl;

		pControl->Create( id, pUIContainer, GenRect()( _pos.left+firstColumnPos, _pos.top+firstRowPos, _pos.right+1+firstColumnPos, _pos.bottom+1+firstRowPos ), 0, true );
		pControl->SetString( _strContent );
	}
	else if( _controlType==COMBOBOX )
	{
		UIComboBox* pControl = new UIComboBox;
		_pCtrl = pControl;

		pControl->Create( id, pUIContainer, GenRect()( _pos.left+firstColumnPos, _pos.top+firstRowPos, _pos.right+1+firstColumnPos, _pos.bottom+1+firstRowPos ), 0, true );
		pControl->IsDrawBoader( false );

		int selIndex=0;
		for( UINT i=0; i<_cacheStrList.size(); ++i )
		{
			pControl->AddString( _cacheStrList[i] );
			if( _strContent==_cacheStrList[i] )
				selIndex = i;
		}
		pControl->SetSelectIndex( selIndex );

		if( !_isHold )
			UIPostMessage( pControl, WM_LBUTTONDOWN, 0, 0 );
	}
	else if( _controlType==FIX )
	{
		return;
	}

	UIRefresh();
}

bool UIGrid::GridCellInfo::DeleteControl()
{
	// 检查控件是否存在
	if( _pCtrl==NULL ) return true;	
	if( _isHold==true ) return true;

	if( _controlType == EDIT )
	{
		// 获得控件指针
		UIEdit* pControl = (UIEdit*)_pCtrl;

		// 获取控件string值并设置到cell上
		string str;
		pControl->GetString( str );
		_strContent = str;
		
		// 销毁edit控件
		pControl->DestroyDirect();
		
	}
	else if( _controlType==CHECKBUTTON )
	{
		// 获得控件指针
		UICheckButton* pControl = (UICheckButton*)_pCtrl;
		pControl->DestroyDirect();
	}
	else if( _controlType==BUTTON )
	{
		// 获得控件指针
		UIButton* pControl = (UIButton*)_pCtrl;
		pControl->DestroyDirect();
	}
	else if( _controlType==COMBOBOX )
	{
		// 获得控件指针
		UIComboBox* pControl = (UIComboBox*)_pCtrl;

		// 获取控件string值并设置到cell上
		string str;
		pControl->GetString( str );
		_strContent = str;

		pControl->DestroyDirect();
	}

	_pCtrl = NULL;
	return true;
}

void UIGrid::GridCellInfo::MoveControl( RECT& rc )
{
	if( _pCtrl==NULL ) return;

	_pCtrl->MoveWindowDirect( rc );
}

void UIGrid::GridCellInfo::DrawControl()
{
	if( _pCtrl==NULL ) return;

	_pCtrl->ShowWindowDirect( true );
	_pCtrl->Draw();
}

UIGrid::UIGrid(): _xScroll( 0 ), _yScroll( 1 )
{
	_firstRowPos = 0;
	_firstColumnPos = 0;

	_rowNum = 0;
	_columnNum = 0;

	_heightSum = 0;
	_widthSum = 0;

	_isFirstRowFix = false;
	_isFirstColumnFix = false;

	_isXScrollShow = false;
	_isYScrollShow = false;
	_xScrollBarArea = _NULLRECT_;
	_yScrollBarArea = _NULLRECT_;

	_selectInfo = NO_SEL;

	_isDraw = true;
	_isCalcArea = false;
}

void UIGrid::Draw()
{
	JudgeIsCalcArea();

	// 画边框
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEBLUE_ );

	// 绘制检查
	if( !_isDraw ) return;
	if( _rowNum==0 || _columnNum==0 ) return;

	// 画滚动条
	DrawScrollBar();

	// 计算绘制的起始行列
	CalcDrawBeginRowColumn( _beginDrawRow, _beginDrawColumn );

	// 绘制选中内容
	DrawSelect();

	// 绘制表格虚线
	DrawGrid();

	// 绘制cell内容
	DrawCells();
}

void UIGrid::DrawScrollBar()
{
	if( _isXScrollShow )
	{
		_xScroll.MoveWindowDirect( _xScrollBarArea );
		_xScroll.Draw();
	}
	//
	if( _isYScrollShow )
	{
		_yScroll.MoveWindowDirect( _yScrollBarArea );
		_yScroll.Draw();
	}
}

void UIGrid::DrawSelect()
{
	COLORREF fixColor = D3DCOLOR_XRGB(102, 204, 255);
	COLORREF selectColor = D3DCOLOR_XRGB(202, 228, 255);

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;	
	RECT rc;

	// 画fix区域常态背景
	if( _isFirstRowFix&&_isFirstColumnFix )
	{
		rc = GenRect()( _gridArea.left, _gridArea.top, _gridArea.right, _gridArea.top+_rowHeightList[0] );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( fixColor, 240 );

		int rcTop = _gridArea.top+_rowHeightList[0]<_gridArea.bottom ? ( _gridArea.top+_rowHeightList[0] ):_gridArea.bottom;

		rc = GenRect()( _gridArea.left, rcTop, _gridArea.left+_columnWidthList[0], _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( fixColor, 240 );
	}
	else if( _isFirstRowFix )
	{
		rc = GenRect()( _gridArea.left, _gridArea.top, _gridArea.right, _gridArea.top+_rowHeightList[0] );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( fixColor, 240 );
	}
	else if( _isFirstColumnFix )
	{
		rc = GenRect()( _gridArea.left, _gridArea.top, _gridArea.left+_columnWidthList[0], _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( fixColor, 240 );
	}

	// 绘制选中标识
	if( _selectInfo==ALL_SEL )					
	{	// 处理全部激活
		DrawSelectALL( selectColor );
	}
	else if( _selectInfo==CELL_SEL )			
	{	// 画选中的cell框
		DrawSelectCELL();
	}
	else if( _selectInfo==CELLS_SEL )			
	{	// 画选中的cells框
		DrawSelectCELLS( selectColor );
	}
	else if( _selectInfo==ROW_SEL )
	{
		DrawSelectROW( selectColor );
	}
	else if( _selectInfo==COLUMN_SEL )
	{	
		DrawSelectCOLUMN( selectColor );
	}
}

void UIGrid::DrawSelectALL( COLORREF& selectColor )
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;	

	// unfix区域的绘制
	RECT rc = GenRect()( _unfixGridArea.left, _unfixGridArea.top, _unfixGridArea.right, _unfixGridArea.bottom );
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( selectColor, 180 );
}

void UIGrid::DrawSelectCELL()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	RECT rc;

	GridCellInfo* pSelCell = &_cellArray[_selRowBegin][_selColumnBegin];
	RECT& pos = pSelCell->_pos;
	
	{	// unfix区域的绘制
		UI2DClipRect uiClip;
		rc = _unfixGridArea;
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		// 画选中的cell
		if( pSelCell->_pCtrl==NULL )
		{
			rc = GenRect()( pos.left+_firstColumnPos+1, pos.top+_firstRowPos+1, pos.right+_firstColumnPos, pos.bottom+_firstRowPos );
			OffsetRect( &rc, x_, y_ );
			UI2DRect( rc, _z )( _FRAMEPREPLE_ );
		}

		uiClip.EndClipRect();
	}

	// fix区域的绘制 cell的行列信息
	// 选中的行信息
	if( _isFirstColumnFix )
	{
		UI2DClipRect uiClip;
		rc = GenRect()( _gridArea.left, _unfixGridArea.top, _gridArea.right, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );
			
		rc = GenRect()( _cellArray[0][0]._pos.left, pos.top+_firstRowPos, _cellArray[0][0]._pos.right, pos.bottom+_firstRowPos );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( _FRAMEPREPLE_, 250 );

		uiClip.EndClipRect();
	}
	// 选中的列信息
	if( _isFirstRowFix )
	{
		UI2DClipRect uiClip;
		rc = GenRect()( _unfixGridArea.left, _gridArea.top, _gridArea.right, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		rc = GenRect()( pos.left+_firstColumnPos, _cellArray[0][0]._pos.top, pos.right+_firstColumnPos, _cellArray[0][0]._pos.bottom );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( _FRAMEPREPLE_, 250 );
			
		uiClip.EndClipRect();
	}
}

void UIGrid::DrawSelectCELLS( COLORREF& selectColor )
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	RECT rc;

	UINT beginRow, endRow, beginColumn, endColumn;
	CalcCellsRange( beginRow, endRow, beginColumn, endColumn );

	GridCellInfo* pCellBegin = &_cellArray[beginRow][beginColumn];
	GridCellInfo* pCellEnd = &_cellArray[endRow][endColumn];
		
	RECT& posBegin = pCellBegin->_pos;
	RECT& posEnd = pCellEnd->_pos;

	{	// unfix区域的绘制
		UI2DClipRect uiClip;
		rc = _unfixGridArea;
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		// 画选中的cell
		rc = GenRect()( posBegin.left+_firstColumnPos+1, posBegin.top+_firstRowPos+1, posEnd.right+_firstColumnPos, posEnd.bottom+_firstRowPos );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( selectColor, 180 );

		uiClip.EndClipRect();
	}

	// fix区域的绘制 cell的行列信息
	// 选中的行信息
	if( _isFirstColumnFix )
	{
		UI2DClipRect uiClip;
		rc = GenRect()( _gridArea.left, _unfixGridArea.top, _gridArea.right, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );
			
		rc = GenRect()( _cellArray[0][0]._pos.left, posBegin.top+_firstRowPos, _cellArray[0][0]._pos.right, posEnd.bottom+_firstRowPos );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( _FRAMEPREPLE_, 250 );

		uiClip.EndClipRect();
	}
	// 选中的列信息
	if( _isFirstRowFix )
	{
		UI2DClipRect uiClip;
		rc = GenRect()( _unfixGridArea.left, _gridArea.top, _gridArea.right, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		rc = GenRect()( posBegin.left+_firstColumnPos, _cellArray[0][0]._pos.top, posEnd.right+_firstColumnPos, _cellArray[0][0]._pos.bottom );
		OffsetRect( &rc, x_, y_ );
		UI2DRect( rc, _z )( _FRAMEPREPLE_, 250 );
			
		uiClip.EndClipRect();
	}
}

void UIGrid::DrawSelectROW( COLORREF& selectColor )
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	RECT rc;

	{	// unfix区域的绘制
		UI2DClipRect uiClip;
		rc = _unfixGridArea;
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );
	
		for( vector<UINT>::iterator i=_selRowList.begin(); i!=_selRowList.end(); ++i )
		{
			RECT& pos = _cellArray[*i][0]._pos;
			rc = GenRect()( pos.right, pos.top+_firstRowPos, _gridArea.right, pos.bottom+_firstRowPos );
			OffsetRect( &rc, x_, y_ );
			UI2DRect( rc, _z )( selectColor, 180 );
		}

		uiClip.EndClipRect();
	}

	{	// fix区域的绘制
		// 设定区域
		UI2DClipRect uiClip;
		rc = GenRect()( _gridArea.left, _unfixGridArea.top+1, _gridArea.right, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		for( vector<UINT>::iterator i=_selRowList.begin(); i!=_selRowList.end(); ++i )
		{
			RECT& pos = _cellArray[*i][0]._pos;
			rc = GenRect()( pos.left, pos.top+_firstRowPos, pos.right, pos.bottom+_firstRowPos );
			OffsetRect( &rc, x_, y_ );
			UI2DRect( rc, _z )( _FRAMEPREPLE_, 250 );
		}

		uiClip.EndClipRect();
	}
}

void UIGrid::DrawSelectCOLUMN( COLORREF& selectColor )
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	RECT rc;

	{	// unfix区域的绘制
		UI2DClipRect uiClip;
		rc = _unfixGridArea;
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		for( vector<UINT>::iterator i=_selColumnList.begin(); i!=_selColumnList.end(); ++i )
		{
			RECT& pos = _cellArray[0][*i]._pos;
			rc = GenRect()( pos.left+_firstColumnPos, pos.bottom, pos.right+_firstColumnPos, _gridArea.bottom );
			OffsetRect( &rc, x_, y_ );
			UI2DRect( rc, _z )( selectColor, 180 );
		}

		uiClip.EndClipRect();
	}

	{	// fix区域的绘制
		UI2DClipRect uiClip;
		rc = GenRect()( _unfixGridArea.left+1, _gridArea.top, _gridArea.right, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		for( vector<UINT>::iterator i=_selColumnList.begin(); i!=_selColumnList.end(); ++i )
		{
			RECT& pos = _cellArray[0][*i]._pos;
			rc = GenRect()( pos.left+_firstColumnPos, pos.top, pos.right+_firstColumnPos, pos.bottom );
			OffsetRect( &rc, x_, y_ );
			UI2DRect( rc, _z )( _FRAMEPREPLE_, 250 );
		}

		uiClip.EndClipRect();
	}
}

void UIGrid::DrawGrid()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;	

	// 画横线
	int top = _isFirstRowFix?_rowHeightList[0]:0;
	for( UINT i=_beginDrawRow; i<_rowNum; ++i )
	{
		int yPos = _cellArray[i][0]._pos.bottom + _firstRowPos;

		if( yPos<top ) 
			continue;
		else if( yPos>_gridArea.bottom+1 ) 
			break;

		UI2DLine( _gridArea.left+x_, yPos+y_, _gridArea.right+x_, yPos+y_, _z )( _GRAY_ );
	}
	if( _beginDrawRow!=0 && _isFirstRowFix )
	{
		int yPos = _cellArray[0][0]._pos.bottom;
		UI2DLine( _gridArea.left+x_, yPos+y_, _gridArea.right+x_, yPos+y_, _z )( _GRAY_ );
	}

	// 画竖线
	int left = _isFirstColumnFix?_columnWidthList[0]:0;
	for( UINT i=_beginDrawColumn; i<_columnNum; ++i )
	{
		int xPos = _cellArray[0][i]._pos.right + _firstColumnPos;

		if( xPos<left ) 
			continue;
		else if( xPos>_gridArea.right+1 ) 
			break;

		UI2DLine( xPos+x_, _gridArea.top+y_, xPos+x_, _gridArea.bottom+y_, _z )( _GRAY_ );
	}
	if( _beginDrawColumn!=0 && _isFirstColumnFix )
	{
		int xPos = _cellArray[0][0]._pos.right;
		UI2DLine( xPos+x_, _gridArea.top+y_, xPos+x_, _gridArea.bottom+y_, _z )( _GRAY_ );
	}
}

void UIGrid::DrawCells()
{
	// 计算相对gridUnfix区域
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;
	RECT rc;

	// 计算绘画的起始行列
	UINT endRow = _rowNum;
	UINT endColumn = _columnNum;

	// 画_unfixGridArea区域 内容
	if( _isFirstRowFix==true )
		_beginDrawRow = _beginDrawRow>1?_beginDrawRow:1;
	if( _isFirstColumnFix==true )
		_beginDrawColumn = _beginDrawColumn>1?_beginDrawColumn:1;

	// 
	UI2DFont font( _z, 16 );	

	int topLimit = ( _unfixGridArea.bottom-_firstRowPos )<_heightSum ? ( _unfixGridArea.bottom-_firstRowPos ):_heightSum;
	int rightLimit = ( _unfixGridArea.right-_firstColumnPos )<_widthSum ? ( _unfixGridArea.right-_firstColumnPos ):_widthSum;
	//
	for( UINT row=_beginDrawRow; row<_rowNum; ++row )
	{
		if( _cellArray[row][0]._pos.top>topLimit )
		{
			endRow = row;
			break;
		}
		for( UINT column=_beginDrawColumn; column<_columnNum; ++column )
		{
			if( _cellArray[0][column]._pos.left>rightLimit )
			{
				endColumn = column;
				break;
			}

			GridCellInfo& cell = _cellArray[row][column];
			//
			UI2DClipRect uiClip;
			rc = _unfixGridArea;
			OffsetRect( &rc, x_, y_ );
			uiClip.BeginClipRect( rc );
			//
			if( cell._pCtrl==NULL )
			{
				rc = GenRect()( cell._pos.left+_firstColumnPos+5, cell._pos.top+_firstRowPos, cell._pos.right+_firstColumnPos-5, cell._pos.bottom+_firstRowPos );
				OffsetRect( &rc, x_, y_ );
				rc.left += 2;
				font( cell._strContent, rc, cell._wordPos );
			}
			else if( cell._pCtrl!=NULL )
			{
				rc = GenRect()( cell._pos.left+_firstColumnPos, cell._pos.top+_firstRowPos, cell._pos.right+_firstColumnPos, cell._pos.bottom+_firstRowPos );
				cell.MoveControl( rc );
				cell.DrawControl();
			}
			//
			uiClip.EndClipRect();
		}
	}

	// 画fix行
	UI2DClipRect uiClip;
	if( _isFirstRowFix )
	{
		rc = GenRect()( _unfixGridArea.left, _gridArea.top, _gridArea.right, _unfixGridArea.top );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		for( UINT column=_beginDrawColumn; column<endColumn; ++column )
		{
			GridCellInfo& cell = _cellArray[0][column];
			if( cell._pCtrl==NULL )
			{
				rc = GenRect()( cell._pos.left+_firstColumnPos, _gridArea.top, cell._pos.right+_firstColumnPos, _gridArea.top+_rowHeightList[0] );
				OffsetRect( &rc, x_, y_ );
				font( cell._strContent, rc, UI2DFont::FONT_CENTER, cell._color );
			}
			else if( cell._pCtrl!=NULL )
			{
				rc = GenRect()( cell._pos.left+_firstColumnPos, cell._pos.top+_firstRowPos, cell._pos.right+_firstColumnPos, cell._pos.bottom+_firstRowPos );
				cell.MoveControl( rc );
				cell.DrawControl();
			}
		}
		
		uiClip.EndClipRect();
	}
	
	// 画fix列
	if( _isFirstColumnFix )
	{
		// 选定fix列裁剪区域
		rc = GenRect()( _gridArea.left, _unfixGridArea.top, _unfixGridArea.left, _gridArea.bottom );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );

		for( UINT row=_beginDrawRow; row<endRow; ++row )
		{
			GridCellInfo& cell = _cellArray[row][0];
			if( cell._pCtrl==NULL )
			{
				rc = GenRect()( _gridArea.left, cell._pos.top+_firstRowPos, _gridArea.left+_columnWidthList[0], cell._pos.bottom+_firstRowPos );
				OffsetRect( &rc, x_, y_ );
				font( cell._strContent, rc, UI2DFont::FONT_CENTER );
			}
			else if( cell._pCtrl!=NULL )
			{
				rc = GenRect()( cell._pos.left+_firstColumnPos, cell._pos.top+_firstRowPos, cell._pos.right+_firstColumnPos, cell._pos.bottom+_firstRowPos );
				cell.MoveControl( rc );
				cell.DrawControl();
			}
		}

		uiClip.EndClipRect();
	}

	// 绘制fix区域第一个cell
	{
		rc = GenRect()( _gridArea.left-1, _gridArea.top-1, _gridArea.right+1, _gridArea.bottom+1 );
		OffsetRect( &rc, x_, y_ );
		uiClip.BeginClipRect( rc );
		// 
		if( _isFirstRowFix && _isFirstColumnFix )
		{
			GridCellInfo& cell = _cellArray[0][0];
			if( cell._pCtrl==NULL )
			{
				rc = GenRect()( _gridArea.left, _gridArea.top, _gridArea.left+_columnWidthList[0], _gridArea.top+_rowHeightList[0] );
				OffsetRect( &rc, x_, y_ );
				font( cell._strContent, rc, UI2DFont::FONT_CENTER );
			}
			else if( cell._pCtrl!=NULL )
			{
				rc = GenRect()( cell._pos.left, cell._pos.top, cell._pos.right, cell._pos.bottom );
				cell.MoveControl( rc );
				cell.DrawControl();
			}
		}
		//
		uiClip.EndClipRect();
	}
}

void UIGrid::CalcArea()
{
	if( GetWidth()( _clientRC )==0 || GetHeight()( _clientRC )==0 )
		_isDraw = false;
	else
		_isDraw = true;

	CalcClientRect();
	CalcGridRect();
	CalcScrollBar();
	CalcCellsPos();
}

void UIGrid::CalcClientRect()
{
	// 计算 noScrollArea
	_noScrollArea = _clientRC;
	// 第一次计算
	bool xFlag = true;
	bool yFlag = true;
	if( _widthSum>GetWidth()( _clientRC )-2 )
	{
		xFlag = false;
		_noScrollArea.bottom -= 15;
	}
	if( _heightSum>GetHeight()( _clientRC )-2 )
	{
		yFlag = false;
		_noScrollArea.right -= 15;
	}	
	// 第二次计算 防止生成的滚动条影响表格区域
	if( _widthSum>GetWidth()( _noScrollArea ) && xFlag )	
		_noScrollArea.bottom -= 15;
	if( _heightSum>GetHeight()( _noScrollArea )  && yFlag )	
		_noScrollArea.right -= 15;
}

void UIGrid::CalcGridRect()
{
	// 计算grid区域
	_gridArea.left = _noScrollArea.left+1;
	_gridArea.top = _noScrollArea.top+1;
	_gridArea.right = _gridArea.left + _widthSum + _firstColumnPos;
	_gridArea.bottom = _gridArea.top + _heightSum + _firstRowPos;
	// 
	_gridArea.right = _gridArea.right<( _noScrollArea.right-1 ) ? _gridArea.right:( _noScrollArea.right-1 );
	_gridArea.bottom = _gridArea.bottom<( _noScrollArea.bottom-1 ) ? _gridArea.bottom:( _noScrollArea.bottom-1 );

	// 计算_unfixGridArea区域
	_unfixGridArea = _gridArea;
	if( _isFirstRowFix )
		_unfixGridArea.top += _rowHeightList[0];
 	if( _isFirstColumnFix )
		_unfixGridArea.left += _columnWidthList[0];
}

void UIGrid::CalcScrollBar()
{
	if( _widthSum==0 || _heightSum==0 )
		return;

	// 计算scrollbar区域
	if( _widthSum<=GetWidth()( _noScrollArea ) )
	{
		_isXScrollShow = false;
		_firstColumnPos = 0;
		_xScroll.SetPosScaleDirect( 0 );
	}
	else
	{
		// 计算滚动条信息
		_isXScrollShow = true;
		_xScroll.SetPageScaleDirect( (double)( GetWidth()( _gridArea )-1 )/_widthSum );

		// 计算滚动条位置
		_xScrollBarArea.left = _noScrollArea.left+1;
		_xScrollBarArea.right = _noScrollArea.right-1;
		_xScrollBarArea.top = _noScrollArea.bottom;
		_xScrollBarArea.bottom = _clientRC.bottom-1;
	}

	if( _heightSum<=GetHeight()( _noScrollArea ) )
	{
		_isYScrollShow = false;
		_firstRowPos = 0;
		_yScroll.SetPosScaleDirect( 0 );
	}
	else
	{
		// 计算滚动条信息 
		_isYScrollShow = true;
		_yScroll.SetPageScaleDirect( (double)( GetHeight()( _gridArea )-1 )/_heightSum );

		// 计算滚动条位置
		_yScrollBarArea.left = _noScrollArea.right;
		_yScrollBarArea.right = _clientRC.right-1;
		_yScrollBarArea.top = _clientRC.top+1;
		_yScrollBarArea.bottom = _noScrollArea.bottom-1;
	}
}

// 根据滚动条位置信息计算绘制的起始行列
void UIGrid::CalcDrawBeginRowColumn( UINT& beginRow, UINT& beginColumn )
{
	beginRow = 0; 
	beginColumn = 0; 

	if( _isYScrollShow )
	{
		long _top =  _noScrollArea.top - _firstRowPos;
		// 
		beginRow = (UINT)( _rowNum*_yScroll.GetPosScale() );
		if( beginRow==_rowNum )
		{
			beginRow -= 1;
		}
		else
		{
			while( _cellArray[beginRow][0]._pos.bottom<_top && beginRow<_rowNum ) 
				beginRow++;
			while( _cellArray[beginRow][0]._pos.top>_top && beginRow>0 ) 
				--beginRow;
		}
	}

	if( _isXScrollShow )
	{
		long _left = _noScrollArea.left - _firstColumnPos;
		beginColumn = (UINT)( _columnNum*_xScroll.GetPosScale() );
		if( beginColumn==_columnNum )
		{
			beginColumn -= 1;
		}
		else
		{
			while( _cellArray[0][beginColumn]._pos.right<_left && beginColumn<_columnNum ) 
				beginColumn++;
			while( _cellArray[0][beginColumn]._pos.left>_left && beginColumn>0 ) 
				--beginColumn;
		}
	}

	// 设置不再显示主要目的为不再接受消息
	for( UINT r=0; r<_rowNum; ++r )
	{
		for( UINT c=0; c<_columnNum; ++c )
		{
			GridCellInfo& cell = _cellArray[r][c];
			if( cell._pCtrl!=NULL )
				cell._pCtrl->ShowWindowDirect( false );
		}
	}
}

void UIGrid::CalcCellsPos()
{
	// 检查
	if( _rowNum==0 || _columnNum==0 ) 
		return;

	// 初始化第一个cell
	GridCellInfo& cell = _cellArray[0][0];
	cell._pos.left = _gridArea.left;
	cell._pos.top = _gridArea.top;
	cell._pos.right = cell._pos.left + _columnWidthList[0];
	cell._pos.bottom = cell._pos.top + _rowHeightList[0];
	// 初始化第一行的cell
	for( UINT colum=1; colum<_columnNum; ++colum )
	{
		GridCellInfo& cell = _cellArray[0][colum];
		cell._pos.left = _cellArray[0][colum-1]._pos.right;
		cell._pos.top = _gridArea.top;
		cell._pos.right = cell._pos.left + _columnWidthList[colum];
		cell._pos.bottom = cell._pos.top + _rowHeightList[0];
	}
	// 初始化第一列cell
	for( UINT row=1; row<_rowNum; ++row )
	{
		GridCellInfo& cell = _cellArray[row][0];
		cell._pos.left = _gridArea.left;
		cell._pos.top = _cellArray[row-1][0]._pos.bottom;
		cell._pos.right = cell._pos.left + _columnWidthList[0];
		cell._pos.bottom = cell._pos.top + _rowHeightList[row];
	}
	// 初始化剩余的cell
	for( UINT row=1; row<_rowNum; ++row )
	{
		for( UINT colum=1; colum<_columnNum; ++colum )
		{
			GridCellInfo& cell = _cellArray[row][colum];
			cell._pos.left = _cellArray[row][colum-1]._pos.right;
			cell._pos.top = _cellArray[row-1][colum]._pos.bottom;
			cell._pos.right = cell._pos.left + _columnWidthList[colum];
			cell._pos.bottom = cell._pos.top + _rowHeightList[row];
		}
	}
}

bool UIGrid::CalcCellIndexUnfix( UINT& row, UINT& column, POINT point )
{
	if( _unfixGridArea.top==_unfixGridArea.bottom || _unfixGridArea.left==_unfixGridArea.right ) 
		return false;

	int xCoord = point.x - _firstColumnPos;
	int yCoord = point.y - _firstRowPos;
	
	// 判断行
	row = _beginDrawRow;
	while( _cellArray[row][0]._pos.bottom<yCoord ) ++row;

	// 判断列
	column = _beginDrawColumn;
	while( _cellArray[0][column]._pos.right<xCoord ) ++column;

	return true;
}

void UIGrid::CalcCellsRange( UINT& beginRow, UINT& endRow, UINT& beginColumn, UINT& endColumn )
{
	beginRow = _selRowBegin<_selRowEnd ? _selRowBegin:_selRowEnd;
	endRow = _selRowBegin>_selRowEnd ? _selRowBegin:_selRowEnd;
	beginColumn = _selColumnBegin<_selColumnEnd ? _selColumnBegin:_selColumnEnd;
	endColumn = _selColumnBegin>_selColumnEnd ? _selColumnBegin:_selColumnEnd;	
}

void UIGrid::SetNoSel()
{
	// 释放上次激活的cell
	if( _selectInfo==CELL_SEL )
	{
		GridCellInfo& activeCell = _cellArray[_selRowBegin][_selColumnBegin];
		activeCell.DeleteControl();
	}

	_selectInfo = NO_SEL;
}	

void UIGrid::SetCellStringDirect( UINT row, UINT column, std::string& str )
{
	if( _cellArray[row][column]._pCtrl==NULL )
	{
		_cellArray[row][column]._strContent = str;
	}
	else 
	{	// 若cell中控件存在 则设置控件的string
		if( _cellArray[row][column]._controlType==GridCellInfo::EDIT )
		{
			UIEdit& edit = *(UIEdit*)( _cellArray[row][column]._pCtrl );
			edit.SetString( str );
		}
		else if( _cellArray[row][column]._controlType==GridCellInfo::CHECKBUTTON )
		{
			UICheckButton& but = *(UICheckButton*)( _cellArray[row][column]._pCtrl );
			but.SetString( str );
		}
	}
}

void UIGrid::SplitWordsForGrid( std::string line, VECTOR_STRING& words, const VECTOR_CHAR& splitList )
{
	VECTOR_INT splitIndex;
	splitIndex.push_back( -1 );
	for( UINT i=0; i<line.size(); ++i )
	{
		if( CheckSplit( line[i], splitList ) )
			splitIndex.push_back( i );
	}
	splitIndex.push_back( line.size() );

	for( UINT i=0; i<splitIndex.size()-1; ++i )
	{	// 1+splitIndex[i]中1一定要在splitIndex[i]前面 防止前越界错误
		words.push_back( string( line.begin()+1+splitIndex[i], line.begin()+splitIndex[i+1] ) );
	}
}

void UIGrid::SplitLineWordsForGrid( std::string str, vector<VECTOR_STRING >& allLineWordsList, const VECTOR_CHAR& splitList )
{
	// 过滤掉0X0D
	str.erase( remove( str.begin(), str.end(), 0X0D ), str.end() );

	VECTOR_INT splitIndex;
	splitIndex.push_back( -1 );
	for( UINT i=0; i<str.size(); ++i )
	{
		if( str[i]==0X0A )
			splitIndex.push_back( i );
	}

	for( UINT i=0; i<splitIndex.size()-1; ++i )
	{
		VECTOR_STRING words;
		SplitWordsForGrid( string( str.begin()+1+splitIndex[i], str.begin()+splitIndex[i+1] ), words, splitList );
		allLineWordsList.push_back( words );
	}
}

void UIGrid::SetRowColumn( int rowNum, int columnNum )
{
	// 行列数与先前相同 不做处理
	if( _rowNum==rowNum && _columnNum==columnNum ) 
		return;

	Cache3* pCache = new Cache3;
	pCache->_row = rowNum;
	pCache->_column = columnNum;

	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UIGrid::GetRowColumn( int& rowNum, int& columnNum )
{
	rowNum = _rowNum;
	columnNum = _columnNum;
}

void UIGrid::SetRowHeight( UINT row, int height )
{
	Cache2* pCache = new Cache2;
	pCache->_index = row;
	pCache->_v = height;

	UIPostMessage( this, WM_UPDATE, 9, (LPARAM)pCache );
}

void UIGrid::SetColumnWidth( UINT column, int width )
{
	Cache2* pCache = new Cache2;
	pCache->_index = column;
	pCache->_v = width;

	UIPostMessage( this, WM_UPDATE, 10, (LPARAM)pCache );
}

void UIGrid::SetRowFix()
{
	UIPostMessage( this, WM_UPDATE, 2, 0 );
}

void UIGrid::SetColumnFix()
{
	UIPostMessage( this, WM_UPDATE, 3, 0 );
}

void UIGrid::SetCellStringAutoWidth( UINT row, UINT column, std::string str )
{
	Cache1 *pCache = new Cache1;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_str = str;
	pCache->_flag = false;
	UIPostMessage( this, WM_UPDATE, 20, (LPARAM)pCache );
}

void UIGrid::SetCellString( UINT row, UINT column, string str )
{
	Cache1 *pCache = new Cache1;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_str = str;
	UIPostMessage( this, WM_UPDATE, 4, (LPARAM)pCache );
}

void UIGrid::GetCellString( UINT row, UINT column, std::string& str )
{
	if( row>_rowNum || column>_columnNum ) return;

	if( _cellArray[row][column]._pCtrl==NULL )
	{
		str = _cellArray[row][column]._strContent.c_str();
	}
	else
	{
		if( _cellArray[row][column]._controlType==GridCellInfo::EDIT )
		{
			UIEdit& edit = *(UIEdit*)( _cellArray[row][column]._pCtrl );
			edit.GetString( str );
		}
		else if( _cellArray[row][column]._controlType==GridCellInfo::CHECKBUTTON )
		{
			UICheckButton& but = *(UICheckButton*)( _cellArray[row][column]._pCtrl );
			but.GetString( str );
		}
		else if( _cellArray[row][column]._controlType==GridCellInfo::COMBOBOX )
		{
			UIComboBox& comboBox = *(UIComboBox*)( _cellArray[row][column]._pCtrl );
			comboBox.GetString( str );
		}
	}
}

void UIGrid::SetCellColor( UINT row, UINT column, D3DCOLOR color)
{
	Cache5 *pCache = new Cache5;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_color = color;
	UIPostMessage( this, WM_UPDATE, 17, (LPARAM)pCache );
}

void UIGrid::SetRow( UINT row, VECTOR_STRING& strList )
{
	Cache6* pCache = new Cache6;
	pCache->_row = row;
	pCache->_strList = strList;
	UIPostMessage( this, WM_UPDATE, 21, (LPARAM)pCache );
}

void UIGrid::AddRow( VECTOR_STRING& row, DWORD wordPos )
{
	Cache4* pCache = new Cache4;
	pCache->_rowList.push_back( row );
	pCache->_wordPos = wordPos;
	UIPostMessage( this, WM_UPDATE, 5, (LPARAM)pCache );
}

void UIGrid::AddRows( vector<VECTOR_STRING >& rowList, DWORD wordPos )
{
	Cache4* pCache = new Cache4;
	pCache->_rowList = rowList;
	pCache->_wordPos = wordPos;
	UIPostMessage( this, WM_UPDATE, 5, (LPARAM)pCache );
}

void UIGrid::ClearUnfixRows()
{
	UIPostMessage( this, WM_UPDATE, 6, 0 );
}

void UIGrid::ClearAllCells()
{
	UIPostMessage( this, WM_UPDATE, 7, 0 );
}

void UIGrid::SetAreaCells( UINT row, UINT column, std::vector<VECTOR_STRING >& rowList )
{
	Cache4* pCache = new Cache4;
	pCache->_rowList = rowList;
	pCache->_row = row;
	pCache->_column = column;
	UIPostMessage( this, WM_UPDATE, 16, (LPARAM)pCache );
}

void UIGrid::SetCellCheckButton( int id, UINT row, UINT column, std::string str )
{
	Cache1 *pCache = new Cache1;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_str = str;
	pCache->_flag = id;
	UIPostMessage( this, WM_UPDATE, 8, (LPARAM)pCache );
}

bool UIGrid::GetCellCheckState( UINT row, UINT column, bool& checkState )
{
	if( row>_rowNum || column>_columnNum ) 
		return false;

	GridCellInfo& cell = _cellArray[row][column];

	if( cell._pCtrl==NULL || cell._controlType!=GridCellInfo::CHECKBUTTON ) 
		return false;

	static_cast<UICheckButton*>( cell._pCtrl )->GetCheck( checkState );
	return true;
}

//void UIGrid::SetCheckButtonMutex( std::vector<std::pair<UINT, UINT> >& mutexList )
//{
//	Cache7 *pCache = new Cache7;
//	pCache->_pairList = mutexList;
//	UIPostMessage( this, WM_UPDATE, 22, (LPARAM)pCache );
//}

void UIGrid::SetCellCheckState( UINT row, UINT column, bool checkState )
{
	Cache3* pCache = new Cache3;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_flag = checkState;
	UIPostMessage( this, WM_UPDATE, 11, (LPARAM)pCache );
}

void UIGrid::SetCellComboBox( int id, UINT row, UINT column, VECTOR_STRING& strList, bool isHold )
{
	Cache6 *pCache = new Cache6;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_id = id;
	pCache->_isHold = isHold;
	pCache->_strList = strList;
	UIPostMessage( this, WM_UPDATE, 12, (LPARAM)pCache );
}

UIComboBox* UIGrid::GetCellComboBox( UINT row, UINT column )
{
	if( row>_rowNum || column>_columnNum ) 
		return 0;

	GridCellInfo& cell = _cellArray[row][column];

	if( cell._pCtrl==NULL || cell._controlType!=GridCellInfo::COMBOBOX ) 
		return 0;

	return static_cast<UIComboBox*>( cell._pCtrl );
}

void UIGrid::SetCellButton( int id, UINT row, UINT column, string str )
{
	Cache1 *pCache = new Cache1;
	pCache->_row = row;
	pCache->_column = column;
	pCache->_str = str;
	pCache->_flag = id;
	UIPostMessage( this, WM_UPDATE, 15, (LPARAM)pCache );
}

void UIGrid::SetCellFix( UINT row, UINT column )
{
	Cache3 *pCache = new Cache3;
	pCache->_row = row;
	pCache->_column = column;
	UIPostMessage( this, WM_UPDATE, 14, (LPARAM)pCache );
}

void UIGrid::SetAllCellsFix()
{
	UIPostMessage( this, WM_UPDATE, 13, 0 );
}

bool UIGrid::GetSelectCell( UINT& row, UINT& column )
{
	if( _selectInfo!=CELL_SEL ) 
		return false;

	row = _selRowBegin;
	column = _selColumnBegin;
	return true;
}

bool UIGrid::GetSelectCells( UINT& beginRow, UINT& beginColumn, UINT& endRow, UINT& endColumn )
{
	if( _selectInfo!=CELLS_SEL ) 
		return false;

	beginRow = _selRowBegin;
	beginColumn = _selColumnBegin;
	endRow = _selRowEnd;
	endColumn = _selColumnEnd;
	return true;
}

void UIGrid::GetSelectRows( vector<UINT>& selectRowList )
{
	selectRowList.clear();

	if( _selectInfo==ALL_SEL )
	{
		for( UINT i=1; i<_rowNum; ++i )	
			selectRowList.push_back( i );
	}
	else if( _selectInfo==ROW_SEL )
	{
		selectRowList = _selRowList;
		sort( selectRowList.begin(), selectRowList.end() );
	}	
	else if( _selectInfo==CELL_SEL )
	{
		selectRowList.push_back( _selRowBegin );
	}
}

void UIGrid::OnCreate()
{
	CalcArea();

	_xScroll.Create( 0, &_uiContainer, _xScrollBarArea );
	_yScroll.Create( 0, &_uiContainer, _yScrollBarArea );
}

bool UIGrid::OnLButtonDown( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	// 
	if( IsPointInRect()( point, _gridArea )==false ) 
		return true;

	// unfix区域处理
	if( IsPointInRect()( point, _unfixGridArea ) )
	{	
		if( !OnLButtonDownUnfix( point ) )
			return true;
	}
	else// fix区域处理
	{	
		OnLButtonDownFix( point );
	}

	// 单行被选中 向父窗体发送选中消息
	if( _selectInfo==ROW_SEL && _selRowList.size()==1 )
	{
		// 通告父窗体选中新的单行
		_nmGrid._code = 2;
		_nmGrid._row = _selRowList[0];
		_nmGrid._column = 0;
		SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );
	}
	else if( _selectInfo==COLUMN_SEL && _selColumnList.size()==1 )
	{
		// 通告父窗体选中新的单列
		_nmGrid._code = 3;
		_nmGrid._row = 0;
		_nmGrid._column = _selColumnList[0];
		SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );
	}
	else if( _selectInfo==CELL_SEL )  // cell被选中
	{
		_nmGrid._code = 4;
		_nmGrid._row = _selRowBegin;
		_nmGrid._column = _selColumnBegin;
		SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );
	}

	UIRefresh();
	return true;
}

bool UIGrid::OnLButtonDownUnfix( POINT point )
{
	// 判断行 列
	UINT row, column;
	if( !CalcCellIndexUnfix( row, column, point ) )
		return false;

	if( _selectInfo==CELL_SEL )
	{	
		if( _selRowBegin==row && _selColumnBegin==column )
		{	// 相同 激活cell中控件
			_cellArray[_selRowBegin][_selColumnBegin].CreateControl( &_uiContainer, _firstRowPos, _firstColumnPos );
		}
		else
		{
			_cellArray[_selRowBegin][_selColumnBegin].DeleteControl();
			_selRowBegin = row;
			_selColumnBegin = column;
		}
	}
	else
	{
		_selectInfo=CELL_SEL;
		_selRowBegin = row;
		_selColumnBegin = column;
	}

	return true;
}

void UIGrid::OnLButtonDownFix( POINT point )
{
	// _pos转换为坐标值
	int xCoord = point.x - _firstColumnPos;
	int yCoord = point.y - _firstRowPos;

	// 首先释放上次可能激活的cell
	if( _selectInfo==CELL_SEL )
		_cellArray[_selRowBegin][_selColumnBegin].DeleteControl();

	GridCellInfo& cell = _cellArray[0][0];
	if( _isFirstRowFix && _isFirstColumnFix && cell._pos.right>=point.x && cell._pos.bottom>=point.y )
	{
		_selectInfo = ALL_SEL;
	}
	else if( ( _isFirstRowFix && _isFirstColumnFix && cell._pos.right<point.x ) || 
				( _isFirstRowFix && !_isFirstColumnFix && cell._pos.left<point.x ) )
	{
		if( _selectInfo != COLUMN_SEL )
		{
			_selColumnList.clear();
			_selectInfo = COLUMN_SEL;
		}

		// 判断列
		UINT column = _beginDrawColumn;
		while( _cellArray[0][column]._pos.right<xCoord ) ++column;

		if( IsKeyDown()( VK_CONTROL ) )
		{
			_selColumnList.push_back( column );
		}
		else if( IsKeyDown()( VK_SHIFT ) )
		{
			if( _selColumnList.size()==0 )
			{
				_selColumnList.push_back( column );
			}
			else
			{
				UINT preColumn = _selColumnList.back();
				while( preColumn<column ) _selColumnList.push_back( ++preColumn );
				while( preColumn>column ) _selColumnList.push_back( --preColumn );
				// 去掉重复元素
				sort( _selColumnList.begin(), _selColumnList.end() );
				_selColumnList.erase( unique( _selColumnList.begin(), _selColumnList.end() ), _selColumnList.end() );
			}
		}
		else
		{
			_selColumnList.clear();
			_selColumnList.push_back( column );
		}
	}
	else if( ( _isFirstRowFix && _isFirstColumnFix && cell._pos.bottom<point.y ) || 
				( !_isFirstRowFix && _isFirstColumnFix && cell._pos.top<point.y ) )
	{
		if( _selectInfo != ROW_SEL )
		{
			_selRowList.clear();
			_selectInfo = ROW_SEL;
		}

		// 判断行
		UINT row = _beginDrawRow;
		while( _cellArray[row][0]._pos.bottom<yCoord ) ++row;

		if( IsKeyDown()( VK_CONTROL ) )
		{
			vector<UINT>::iterator it = find( _selRowList.begin(), _selRowList.end(), row );
			if( it==_selRowList.end() )
				_selRowList.push_back( row );
			else
				_selRowList.erase( it );
		}
		else if( IsKeyDown()( VK_SHIFT ) )
		{
			if( _selRowList.size()==0 )
			{
				_selRowList.push_back( row );
			}
			else
			{
				UINT preRow = _selRowList.back();
				while( preRow<row ) 
					_selRowList.push_back( ++preRow );
				while( preRow>row ) 
					_selRowList.push_back( --preRow );
				// 去掉重复元素
				sort( _selRowList.begin(), _selRowList.end() );
				_selRowList.erase( unique( _selRowList.begin(), _selRowList.end() ), _selRowList.end() );
			}
		}
		else
		{
			_selRowList.clear();
			_selRowList.push_back( row );
		}
	}
}

bool UIGrid::OnLButtonDbClk( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	// 检查active cell是否存在
	if( _selectInfo!=CELL_SEL ) 
		return true;

	if( IsPointInRect()( point, _unfixGridArea ) ) 
	{
		_cellArray[_selRowBegin][_selColumnBegin].CreateControl( &_uiContainer, _firstRowPos, _firstColumnPos );
		return true;
	}
	
	return OnLButtonDown( pt );
}

void UIGrid::OnHscroll( int code, UIScrollBar* pScrollBar )
{
	double posScale = pScrollBar->GetPosScale();
	double pageScale = pScrollBar->GetPageScale();

	switch( code )
	{
		case SB_LINELEFT:   // 向左的箭头
			{
				_firstColumnPos += 60;
				if( _firstColumnPos>0 )
					_firstColumnPos = 0;
				double scale = -(double)_firstColumnPos/_widthSum;
				pScrollBar->SetPosScaleDirect( scale );	
			}
			break;
		case SB_LINERIGHT:	// 向右的箭头
			{
				_firstColumnPos -= 60;
				if( ( -_firstColumnPos )>_widthSum*( 1-pageScale ) )
					_firstColumnPos = (int)( -_widthSum*( 1-pageScale ) );
				double scale = -(double)_firstColumnPos/_widthSum;
				pScrollBar->SetPosScaleDirect( scale );
			}
			break;
		case SB_THUMBTRACK: // 鼠标拖动
			_firstColumnPos = -(int)( posScale*_widthSum );
			break;
		case SB_PAGERIGHT:
			_firstColumnPos = -(int)( posScale*_widthSum );
			break;
		case SB_PAGELEFT:
			_firstColumnPos = -(int)( posScale*_widthSum );
			break;
	}

	// 
	UIRefresh();
}

void UIGrid::OnVscroll( int code, UIScrollBar* pScrollBar )
{
	double posScale = pScrollBar->GetPosScale();
	double pageScale = pScrollBar->GetPageScale();

	switch( code )
	{
		case SB_LINEUP:
			{
				_firstRowPos += 20;
				if( _firstRowPos>0 )
					_firstRowPos = 0;
				double scale = -(double)_firstRowPos/_heightSum;
				pScrollBar->SetPosScaleDirect( scale );		
			}
			break;
		case SB_LINEDOWN:
			{
				_firstRowPos -= 20;
				if( ( -_firstRowPos )>_heightSum*( 1-pageScale ) )
					_firstRowPos = (int)( -_heightSum*( 1-pageScale ) );
				double scale = -(double)_firstRowPos/_heightSum;
				pScrollBar->SetPosScaleDirect( scale );
			}
			break;
		case SB_THUMBTRACK:
			_firstRowPos = -(int)( posScale*_heightSum );
			break;
		case SB_PAGEUP:
			_firstRowPos = -(int)( posScale*_heightSum );
			break;
		case SB_PAGEDOWN:
			_firstRowPos = -(int)( posScale*_heightSum );
			break;
	}

	// 
	UIRefresh();
}

bool UIGrid::OnMouseWheel( short zDelta )
{
	if( _isYScrollShow==false ) return true;

	if( zDelta==120 ) 
		OnVscroll( SB_LINEUP, &_yScroll );
	if( zDelta==-120 ) 
		OnVscroll( SB_LINEDOWN, &_yScroll );

	UIRefresh();
	return true;
}

bool UIGrid::OnMouseMove( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	if( _xScroll.IsWindowFocus() )
	{	
		/*if( !IsPointInRect()( point, _xScrollBarArea ) )
			_xScroll.HandleMessage( WM_MOUSEMOVE, 0, MAKELONG( pt.x, pt.y ) );*/
	}
	else if( _yScroll.IsWindowFocus() )
	{
	/*	if( !IsPointInRect()( point, _yScrollBarArea ) )
			_yScroll.HandleMessage( WM_MOUSEMOVE, 0, MAKELONG( pt.x, pt.y ) );*/
	}
	else
	{
		if( IsKeyDown()( VK_LBUTTON ) )
		{
			if( _selectInfo==CELL_SEL || _selectInfo==CELLS_SEL )
			{
				if( IsPointInRect()( point, _unfixGridArea ) )
				{
					if( CalcCellIndexUnfix( _selRowEnd, _selColumnEnd, point ) )
					{
						if( _selRowBegin==_selRowEnd && _selColumnBegin==_selColumnEnd  )
						{
							_selectInfo = CELL_SEL;
						}
						else
						{
							SetNoSel();
							_selectInfo = CELLS_SEL;
							UIRefresh();
						}
					}
				}
			}
		}
	}

	return true;
}

void UIGrid::OnKeyDown( TCHAR nChar )
{
	GridCellInfo* pSelCell=NULL;

	if( nChar==VK_DOWN || nChar==VK_UP || nChar==VK_LEFT || nChar==VK_RIGHT )
	{
		OnKeyDownArrows( nChar );
	}
	else if( nChar==(TCHAR)VK_PROCESSKEY )
	{	// 输入法处理
		OnKeyDownProcessKey();
	}
	else if( nChar==VK_BACK )
	{
		OnKeyDownBack();
	}
	else if( nChar==VK_DELETE )
	{
		OnKeyDownDelete();
	}
	else if( nChar==VK_RETURN )
	{	// 通告父窗口某个cell获得了Enter键
		OnKeyDownReturn();
	}

	UIRefresh();
}

void UIGrid::OnKeyDownArrows( TCHAR nChar )
{
	GridCellInfo* pSelCell = NULL;
	bool calcXScroll = false;
	bool calcYScroll = false;

	if( _selectInfo==CELL_SEL )
	{
		pSelCell = &_cellArray[_selRowBegin][_selColumnBegin];
			
		// 删除激活的eidt
		pSelCell->DeleteControl();

		bool sendFlag = false;
		if( nChar==VK_DOWN )
		{
			if( _selRowBegin<_rowNum-1 ) 
			{
				++_selRowBegin;
				sendFlag = true;
			}
		}
		else if( nChar==VK_UP )
		{
			if( ( _isFirstRowFix && ( _selRowBegin>1 ) )||( ( !_isFirstRowFix ) && ( _selRowBegin>0 ) ) ) 
			{
				--_selRowBegin;
				sendFlag = true;
			}
		}
		else if( nChar==VK_LEFT )
		{
			if( ( _isFirstColumnFix && ( _selColumnBegin>1 ) )||( ( !_isFirstColumnFix ) && ( _selColumnBegin>0 ) ) ) 
				--_selColumnBegin;
		}
		else if( nChar==VK_RIGHT )
		{
			if( _selColumnBegin<_columnNum-1 ) 
				++_selColumnBegin;
		}

		calcXScroll = true;
		calcYScroll = true;
		pSelCell = &_cellArray[_selRowBegin][_selColumnBegin];

		if( sendFlag )
		{
			_nmGrid._code = 4;
			_nmGrid._row = _selRowBegin;
			_nmGrid._column = _selColumnBegin;
			SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );
		}
		
	}
	else if( _selectInfo==ROW_SEL )
	{
		if( nChar==VK_LEFT || nChar==VK_RIGHT ) 
			return;
	
		if( _selRowList.size()!=1 ) 
			return;

		if( nChar==VK_UP )
		{
			if( ( _isFirstRowFix && _selRowList[0]==1 ) || ( _selRowList[0]==0 ) ) 
				return;
			--_selRowList[0];
		}
		else if( nChar==VK_DOWN )
		{
			if( _selRowList[0]==_rowNum-1 ) 
				return;
			++_selRowList[0];
		}

		calcYScroll = true;
		pSelCell = &_cellArray[_selRowList[0]][0];

		// 通告父窗体选中新的单行
		_nmGrid._code = 2;
		_nmGrid._row = _selRowList[0];
		_nmGrid._column = 0;
		SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );
	}
	else if( _selectInfo==COLUMN_SEL )
	{
		if( nChar==VK_UP || nChar==VK_DOWN ) 
			return;

		if( _selColumnList.size()!=1 ) 
			return;

		if( nChar==VK_LEFT )
		{
			if( ( _isFirstColumnFix && _selColumnList[0]==1 ) || ( _selColumnList[0]==0 ) ) 
				return;
			--_selColumnList[0];
		}
		else if( nChar==VK_RIGHT )
		{
			if( _selColumnList[0]==_columnNum-1 ) 
				return;
			++_selColumnList[0];
		}

		calcXScroll = true;
		pSelCell = &_cellArray[0][_selColumnList[0]];

		// 通告父窗体选中新的单行
		_nmGrid._code = 3;
		_nmGrid._row = 0;
		_nmGrid._column = _selColumnList[0];
		SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );
	}

	if( calcXScroll )
	{
		// 左右arrow
		int dx = pSelCell->_pos.left+_firstColumnPos;
		if( dx<_unfixGridArea.left ) 
			_firstColumnPos-=dx-_unfixGridArea.left;
		dx = pSelCell->_pos.right+_firstColumnPos;
		if( dx>_unfixGridArea.right ) 
			_firstColumnPos-=dx-_unfixGridArea.right;
		// 设置x滚动条
		_xScroll.SetPosScaleDirect( static_cast<double>( -_firstColumnPos )/_widthSum );
	}
	if( calcYScroll )
	{
		// 上下arrow
		int dy = pSelCell->_pos.top+_firstRowPos;
		if( dy<_unfixGridArea.top ) 
			_firstRowPos-=dy-_unfixGridArea.top;
		dy = pSelCell->_pos.bottom+_firstRowPos; 
		if( dy>_unfixGridArea.bottom ) 
			_firstRowPos-=dy-_unfixGridArea.bottom;
		// 设置y滚动条
		_yScroll.SetPosScaleDirect( static_cast<double>( -_firstRowPos )/_heightSum );
	}
}

void UIGrid::OnKeyDownProcessKey()
{
	if( _selectInfo==CELL_SEL )
	{	
		_cellArray[_selRowBegin][_selColumnBegin].CreateControl( &_uiContainer, _firstRowPos, _firstColumnPos );
	}
}

void UIGrid::OnKeyDownBack()
{
	if( _selectInfo==CELL_SEL ) 
	{
		GridCellInfo* pSelCell = &_cellArray[_selRowBegin][_selColumnBegin];
		pSelCell->_strContent = "";
		pSelCell->CreateControl( &_uiContainer, _firstRowPos, _firstColumnPos );
	}
}

void UIGrid::OnKeyDownReturn()
{
	if( _selectInfo==CELL_SEL ) 
	{
		// 删除可能存在的edit控件
		_cellArray[_selRowBegin][_selColumnBegin].DeleteControl();

		_nmGrid._code = 1;
		_nmGrid._row = _selRowBegin;
		_nmGrid._column = _selColumnBegin;
		SendMessageToParent( WM_NOTIFY, _id, (LPARAM)&_nmGrid );

		// 指向下一个cell
		if( _selRowBegin<_rowNum-1 ) 
			++_selRowBegin;
	}
}

void UIGrid::OnKeyDownDelete()
{
	if( _selectInfo==CELL_SEL )
	{
		_cellArray[_selRowBegin][_selColumnBegin]._strContent = "";
	}
	else if( _selectInfo==CELLS_SEL )
	{
		UINT beginRow, endRow, beginColumn,endColumn;
		CalcCellsRange( beginRow, endRow, beginColumn,endColumn );

		for( UINT r=beginRow; r<=endRow; ++r )
		{
			for( UINT c=beginColumn; c<=endColumn; ++c )
			{
				_cellArray[r][c]._strContent = "";
			}
		}
	}
	else if( _selectInfo==ROW_SEL )
	{
		for( UINT r=0; r<_selRowList.size(); ++r )
		{
			for( UINT c=1; c<_columnNum; ++c )
			{
				_cellArray[_selRowList[r]][c]._strContent = "";
			}
		}
	}
	else if( _selectInfo==COLUMN_SEL )
	{
		for( UINT r=1; r<_rowNum; ++r )
		{
			for( UINT c=0; c<_selColumnList.size(); ++c )
			{
					_cellArray[r][_selColumnList[c]]._strContent = "";
			}
		}
	}
	else if( _selectInfo==ALL_SEL )
	{
		for( UINT r=1; r<_rowNum; ++r )
		{
			for( UINT c=1; c<_columnNum; ++c )
			{
				_cellArray[r][c]._strContent = "";
			}
		}
	}
}

void UIGrid::OnChar( TCHAR nChar )
{
	if( nChar==VK_RETURN ) return;

	if( _selectInfo!=CELL_SEL ) return;
	GridCellInfo* pSelCell = &_cellArray[_selRowBegin][_selColumnBegin];

	if( pSelCell->_controlType == GridCellInfo::EDIT )
	{
		// 创建edit控件
		pSelCell->CreateControl( &_uiContainer, _firstRowPos, _firstColumnPos );
		UIEdit* pEdit = (UIEdit*)( pSelCell->_pCtrl );
		pEdit->SetString( pSelCell->_strContent );
		pEdit->SelectAllText();

		// 将触发字符也传送给edit控件
		UIPostMessage( pEdit, WM_CHAR, nChar, 0 );
	}

	UIRefresh();
}

bool UIGrid::OnCopy()
{
	bool copyFlag = false;
	string str;

	if( _selectInfo==CELL_SEL )
	{
		copyFlag = true;

		str = _cellArray[_selRowBegin][_selColumnBegin]._strContent;
		str += "\r\n";
	}
	else if( _selectInfo==CELLS_SEL )
	{
		copyFlag = true;

		UINT beginRow, endRow, beginColumn,endColumn;
		CalcCellsRange( beginRow, endRow, beginColumn,endColumn );
		
		for( UINT r=beginRow; r<=endRow; ++r )
		{
			for( UINT c=beginColumn; c<=endColumn; ++c )
			{
				str += _cellArray[r][c]._strContent;
				if( c!=endColumn )
					str += "	";
			}

			str += "\r\n";
		}
	}
	else if( _selectInfo==ROW_SEL && _selRowList.size()!=0 )
	{
		copyFlag = true;

		for( UINT r=0; r<_selRowList.size(); ++r )
		{
			for( UINT c=1; c<_columnNum; ++c )
			{
				str += _cellArray[_selRowList[r]][c]._strContent;
				if( c!=_columnNum-1 )
					str += "	";
			}

			str += "\r\n";
		}
	}
	else if( _selectInfo==COLUMN_SEL && _selColumnList.size()!=0 )
	{
		copyFlag = true;

		for( UINT r=1; r<_rowNum; ++r )
		{
			for( UINT c=0; c<_selColumnList.size(); ++c )
			{
				str += _cellArray[r][_selColumnList[c]]._strContent;
				if( c!=_selColumnList.size()-1 )
					str += "	";
			}

			str += "\r\n";
		}
	}
	else if( _selectInfo==ALL_SEL )
	{
		copyFlag = true;

		for( UINT r=1; r<_rowNum; ++r )
		{
			for( UINT c=1; c<_columnNum; ++c )
			{
				str += _cellArray[r][c]._strContent;	
				if( c!=_columnNum-1 )
					str += "	";
			}

			str += "\r\n";
		}
	}

	if( copyFlag )
	{
		wstring wStr;
		MByteToWChar()( str, wStr );
		StringCopyToClipboard()( wStr, UIWinShell::_pWinShell->_hWnd );
	}

	return true;
}

bool UIGrid::OnPaste()
{
	// 获取剪切板的字符串
	wstring wStr;
	StringPasteFromClipboard()( wStr, UIWinShell::_pWinShell->_hWnd );
	string line;
	WCharToMByte()( wStr, line );

	// 字符串进行解析
	vector<VECTOR_STRING > allLineWordsList;
	VECTOR_CHAR splitList;
	splitList.push_back( '	' );
	SplitLineWordsForGrid( line, allLineWordsList, splitList );

	// 计算粘贴的起始位置
	UINT beginRow, beginColumn;
	bool pasteFlag = false;
	if( _selectInfo==CELL_SEL )
	{
		pasteFlag = true;

		beginRow = _selRowBegin;
		beginColumn = _selColumnBegin;
	}
	else if( _selectInfo==CELLS_SEL )
	{
		pasteFlag = true;

		beginRow = _selRowBegin<_selRowEnd ? _selRowBegin:_selRowEnd;
		beginColumn = _selColumnBegin<_selColumnEnd ? _selColumnBegin:_selColumnEnd;
	}
	else if( _selectInfo==ROW_SEL )
	{
		pasteFlag = true;

		beginRow = _selRowList[0];
		beginColumn = 1;
	}
	else if( _selectInfo==COLUMN_SEL )
	{
		pasteFlag = true;

		beginRow = 1;
		beginColumn = _selColumnList[0];
	}
	else if( _selectInfo==ALL_SEL )
	{
		pasteFlag = true;

		beginRow = 1;
		beginColumn = 1;
	}

	if( pasteFlag )
	{
		for( UINT c=beginRow; ( c-beginRow<allLineWordsList.size() ) && c<_rowNum; ++c )
		{
			for( UINT r=beginColumn; ( r-beginColumn<allLineWordsList[c-beginRow].size() ) && r<_columnNum; ++r )
			{
				_cellArray[c][r]._strContent = allLineWordsList[c-beginRow][r-beginColumn];
			}
		}

		UIRefresh();
	}

	return true;
}

bool UIGrid::OnCut()
{
	if( _selectInfo!=CELL_SEL ) return true;
	GridCellInfo* pSelCell = &_cellArray[_selRowBegin][_selColumnBegin];

	OnCopy();
	pSelCell->_strContent="";

	UIRefresh();
	return true;
}

void UIGrid::OnNotify( int id, int param ) 
{
	SendMessageToParent( WM_NOTIFY, (WPARAM)id, (LPARAM)param );
}

void UIGrid::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		OnSetRowColumn( data );
	}
	else if( flag==2 )
	{
		OnSetRowFix();
	}
	else if( flag==3 )
	{
		OnSetColumnFix();	
	}
	else if( flag==4 )
	{
		if( !OnSetCellString( data, false ) ) return;
	}
	else if( flag==5 )
	{
		OnAddRows( data );
	}
	else if( flag==6 )
	{
		OnClearUnfixRows();
	}
	else if( flag==7 )
	{
		OnClearAllCells();
	}
	else if( flag==8 )
	{
		OnSetCellCheckButton( data );
	}
	else if( flag==9 )
	{
		OnSetRowHeight( data );
		return;
	}
	else if( flag==10 )
	{
		OnSetColumnWidth( data );
		return;
	}
	else if( flag==11 )
	{
		OnSetCellCheckState( data );
	}
	else if( flag==12 )
	{
		OnSetCellComboBox( data );
		return;
	}
	else if( flag==13 )
	{
		OnSetAllCellsFix();
	}
	else if( flag==14 )
	{
		OnSetCellFix( data );
		return;
	}
	else if( flag==15 )
	{
		OnSetCellButton( data );
	}
	else if( flag==16 )
	{
		OnSetAreaCells( data );
	}
	else if( flag==17 )
	{
		if( !OnSetCellColor( data ) ) return;
	}
	else if( flag==20 )
	{
		if( !OnSetCellString( data, true ) ) return;
	}
	else if( flag==21 )
	{
		OnSetRow( data );
	}
	/*else if( flag==22 )
	{
		OnSetCheckButtonMutex( data );
	}*/
}

void UIGrid::OnSetRowColumn( void* data )
{
	Cache3* pCache = (Cache3*)data;
	UINT& rowNum = pCache->_row;
	UINT& columnNum = pCache->_column;

	// 设置NO_SEL 并清除可能生成的控件
	SetNoSel();	
	//
	UINT r = _isFirstRowFix?1:0;
	for( ; r<_rowNum; ++r )
	{
		for( UINT c=0; c<_columnNum; ++c )
		{
			GridCellInfo& cell = _cellArray[r][c];
			cell._isHold =false;
			cell.DeleteControl();
		}
	}

	// 部分初始化
	_firstRowPos = 0;
	_firstColumnPos = 0;
	_xScroll.SetPosScaleDirect( 0 );
	_yScroll.SetPosScaleDirect( 0 );

	// 初始化行高、列宽
	_rowHeightList.clear();
	_rowHeightList.resize( rowNum, 20 );
	_columnWidthList.clear();
	_columnWidthList.resize( columnNum, 60 );

	// 计算行高 列宽 总和
	_heightSum = accumulate( _rowHeightList.begin(), _rowHeightList.end(), 0 );
	_widthSum = accumulate( _columnWidthList.begin(), _columnWidthList.end(), 0 );

	// 初始化cell矩阵
	_cellArray.clear();
	for( UINT row=0; row<rowNum; ++row )
	{
		_cellArray.push_back( vector<GridCellInfo>() );
		for( UINT colum=0; colum<columnNum; ++colum ) 
			_cellArray[row].push_back( GridCellInfo("") );
	}

	// 设置新的行列数
	_rowNum = rowNum;
	_columnNum = columnNum;
	
	// 重新计算布局信息
	_isCalcArea = true;

	delete pCache;
}

void UIGrid::OnSetRowFix()
{
	if( _rowNum==0 ) return;
	_isFirstRowFix = true;
	CalcGridRect();
}

void UIGrid::OnSetColumnFix()
{
	if( _columnNum==0 ) return;
	_isFirstColumnFix = true;
	CalcGridRect();	
}

bool UIGrid::OnSetCellString( void* data, bool isAutoWidth )
{
	Cache1* pCache = (Cache1*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;
	bool refresh = pCache->_flag==1?true:false;

	if( row<_rowNum && column<_columnNum )
	{
		SetCellStringDirect( row, column, pCache->_str );

		// 自动计算宽度
		if( isAutoWidth )
		{
			UI2DFont font( _z );
			SIZE sz = font.GetDrawAreaSize( pCache->_str );
			if( sz.cx>_columnWidthList[column] )
				SetColumnWidth( column, sz.cx );
		}
	}

	delete pCache;
	return refresh;
}

bool UIGrid::OnSetCellColor( void* data )
{
	Cache5* pCache = (Cache5*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;
	bool refresh = pCache->_flag==1?true:false;

	if( row<_rowNum && column<_columnNum )
		_cellArray[row][column]._color = pCache->_color;

	delete pCache;
	return refresh;
}

void UIGrid::OnAddRows( void* data )
{
	Cache4* pCache = (Cache4*)data;

	for( UINT i=0; i<pCache->_rowList.size(); ++i )
	{
		VECTOR_STRING& rowCache = pCache->_rowList[i];

		// 行高增加
		_rowHeightList.push_back( 20 );
		_heightSum += 20;
		// 先增加相应行的cell
		_cellArray.push_back( vector<GridCellInfo>() );
		for( UINT colum=0; colum<_columnNum; ++colum )
			_cellArray[_rowNum].push_back( GridCellInfo( rowCache[colum] ) );
		// 计算该行每个cell的位置
		for( UINT colum=0; colum<_columnNum; ++colum )
		{
			GridCellInfo& cell = _cellArray[_rowNum][colum];
			cell._pos.left = _cellArray[_rowNum-1][colum]._pos.left;
			cell._pos.top = _cellArray[_rowNum-1][colum]._pos.bottom;
			cell._pos.right = cell._pos.left + _columnWidthList[colum];
			cell._pos.bottom = cell._pos.top + _rowHeightList[_rowNum];

			cell._wordPos = pCache->_wordPos;
		}
		// 最后调整行数
		++_rowNum;	
	}

	_isCalcArea = true;
	delete pCache;
}

void UIGrid::OnClearUnfixRows()
{
	// 设置NO_SEL 并清除可能生成的控件
	SetNoSel();	

	// 清除可能存在的滚动条信息
	_xScroll.SetPosScaleDirect( 0 );
	_yScroll.SetPosScaleDirect( 0 );
	_firstRowPos = 0;
	_firstColumnPos = 0;

	// 删除各unfix行中hold的控件
	UINT r = _isFirstRowFix?1:0;
	for( ; r<_rowNum; ++r )
	{
		for( UINT c=0; c<_columnNum; ++c )
		{
			GridCellInfo& cell = _cellArray[r][c];
			cell._isHold =false;
			cell.DeleteControl();
		}
	}

	// 不存在fix行
	if( !_isFirstRowFix )
	{
		_rowNum = 0;			// 先调整行数 防止同时进行的绘制访问不存在的内存
		_cellArray.clear();
		_rowHeightList.clear();
		_columnWidthList.clear();
		_heightSum = 0;
		_widthSum = 0;
	}
	else // 存在fix行
	{
		if( _rowNum == 1 ) return;

		_rowNum = 1;			// 先调整行数 防止同时进行的绘制访问不存在的内存
		_cellArray.erase( ++_cellArray.begin(), _cellArray.end() );
		_rowHeightList.erase( ++_rowHeightList.begin(), _rowHeightList.end() );
		_heightSum = _rowHeightList[0];
	}

	_isCalcArea = true;
}

void UIGrid::OnClearAllCells()
{
	SetNoSel();

	UINT beginRow=0, beginColum=0;
	if( _isFirstRowFix )	beginRow = 1;
	if( _isFirstColumnFix )	beginColum = 1;

	// 删除各unfix行中hold的控件
	UINT r = _isFirstRowFix?1:0;
	for( UINT r=beginRow; r<_rowNum; ++r )
	{
		for( UINT c=beginColum; c<_columnNum; ++c )
		{
			GridCellInfo& cell = _cellArray[r][c];
			cell._isHold =false;
			cell.DeleteControl();
			cell._strContent = "";
		}
	}
}

void UIGrid::OnSetCellCheckButton( void* data )
{
	Cache1* pCache = (Cache1*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;

	if( row<_rowNum && column<_columnNum )
	{
		GridCellInfo& cell = _cellArray[row][column];
		if( cell._pCtrl!=NULL )
		{
			cell._isHold = false;
			cell.DeleteControl();
		}

		cell._controlType = GridCellInfo::CHECKBUTTON;
		cell._strContent = pCache->_str;
		cell._isHold = true;
		cell.CreateControl(  &_uiContainer, _firstRowPos, _firstColumnPos, pCache->_flag );
	}

	delete pCache;
}

void UIGrid::OnSetRowHeight( void* data )
{
	Cache2* pCache = (Cache2*)data;
	if( pCache->_index<_rowHeightList.size() )
	{
		_heightSum += pCache->_v-_rowHeightList[pCache->_index];
		_rowHeightList[pCache->_index] = pCache->_v;
		
		_isCalcArea = true;
	}

	delete pCache;
}

void UIGrid::OnSetColumnWidth( void* data )
{
	Cache2* pCache = (Cache2*)data;
	if( pCache->_index<_columnWidthList.size() )
	{
		_widthSum += pCache->_v - _columnWidthList[pCache->_index];
		_columnWidthList[pCache->_index] = pCache->_v;
		
		_isCalcArea = true;
	}

	delete pCache;
}

void UIGrid::OnSetCellCheckState( void* data )
{
	Cache3* pCache = (Cache3*)data;
	
	if( pCache->_row<_rowNum && pCache->_column<_columnNum ) 
	{
		GridCellInfo& cell = _cellArray[pCache->_row][pCache->_column];

		if( cell._pCtrl!=NULL && cell._controlType==GridCellInfo::CHECKBUTTON )
			static_cast<UICheckButton*>( cell._pCtrl )->SetCheck( pCache->_flag );
	}

	delete pCache;
}

void UIGrid::OnSetCellComboBox( void* data )
{
	Cache6* pCache = (Cache6*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;

	if( row<_rowNum && column<_columnNum )
	{
		GridCellInfo& cell = _cellArray[row][column];
		if( cell._pCtrl!=NULL )
		{
			cell._isHold = false;
			cell.DeleteControl();
		}

		cell._controlType = GridCellInfo::COMBOBOX;
		cell._cacheStrList = pCache->_strList;
		cell._isHold = pCache->_isHold;
		if( cell._isHold )
			cell.CreateControl(  &_uiContainer, _firstRowPos, _firstColumnPos, pCache->_id );
	}

	delete pCache;
}

void UIGrid::OnSetCellButton( void* data )
{
	Cache1* pCache = (Cache1*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;

	if( row<_rowNum && column<_columnNum )
	{
		GridCellInfo& cell = _cellArray[row][column];
		// 销毁已存在的控件
		if( cell._pCtrl!=NULL )
		{
			cell._isHold = false;
			cell.DeleteControl();
		}
		
		cell._controlType = GridCellInfo::BUTTON;
		cell._strContent = pCache->_str;
		cell._isHold = true;
		cell.CreateControl( &_uiContainer, _firstRowPos, _firstColumnPos, pCache->_flag );
	}

	delete pCache;
}

void UIGrid::OnSetCellFix( void* data )
{
	Cache3* pCache = (Cache3*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;

	if( row<_rowNum && column<_columnNum )
	{
		GridCellInfo& cell = _cellArray[row][column];
		// 销毁已存在的控件
		if( cell._pCtrl!=NULL )
		{
			cell._isHold = false;
			cell.DeleteControl();
		}
		
		cell._controlType = GridCellInfo::FIX;
	}

	delete pCache;
}

void UIGrid::OnSetAllCellsFix()
{
	for( UINT r=0; r<_rowNum; ++r )
	{
		for( UINT c=0; c<_columnNum; ++c )
		{
			GridCellInfo& cell = _cellArray[r][c];
			// 销毁已存在的控件
			if( cell._pCtrl!=NULL )
			{
				cell._isHold = false;
				cell.DeleteControl();
			}
		
			cell._controlType = GridCellInfo::FIX;
		}
	}
}

void UIGrid::OnSetAreaCells( void* data )
{
	Cache4* pCache = (Cache4*)data;
	UINT& row = pCache->_row;
	UINT& column = pCache->_column;

	for( UINT i=0; i<pCache->_rowList.size(); ++i )
	{
		VECTOR_STRING& rowCache = pCache->_rowList[i];

		for( UINT j=0; j<rowCache.size(); ++j )
		{
			if( _cellArray[row+i][column+j]._pCtrl==NULL )
			{
				_cellArray[row+i][column+j]._strContent = rowCache[j];
			}
			else 
			{	// 若cell中控件存在 则设置控件的string
				if( _cellArray[row][column]._controlType==GridCellInfo::EDIT )
				{
					UIEdit& edit = *(UIEdit*)( _cellArray[row+i][column+j]._pCtrl );
					edit.SetString( rowCache[j] );
				}
				else if( _cellArray[row][column]._controlType==GridCellInfo::CHECKBUTTON )
				{
					UICheckButton& but = *(UICheckButton*)( _cellArray[row+i][column+j]._pCtrl );
					but.SetString( rowCache[j] );
				}
			}
		}
	}

	delete pCache;
}

void UIGrid::OnSetRow( void* data )
{
	Cache6* pCache = (Cache6*)data;
	UINT& row = pCache->_row;
	VECTOR_STRING& strList = pCache->_strList;

	if( row<_rowNum && strList.size()<=_columnNum )
	{
		for( UINT i=0; i<strList.size(); ++i )
		{
			SetCellStringDirect( row, i, strList[i] );
		}
	}

	delete pCache;
}

//void UIGrid::OnSetCheckButtonMutex( void* data )
//{
//	Cache7* pCache = (Cache7*)data;
//	vector<pair<UINT, UINT> >& pairList = pCache->_pairList;
//
//	vector<UICheckButton*> mutexList;
//	for( UINT i=0; i<pairList.size(); ++i )
//	{
//		UINT& row = pairList[i].first;
//		UINT& column = pairList[i].second;
//
//		if( row<_rowNum && column<_columnNum )
//		{
//			GridCellInfo& cell = _cellArray[row][column];
//			if( cell._pCtrl!=NULL && cell._controlType==GridCellInfo::CHECKBUTTON )
//				mutexList.push_back( static_cast<UICheckButton*>( cell._pCtrl ) );
//		}
//	}
//	UISetCheckButtonMutex( mutexList );
//
//	delete pCache;
//}

// 仿函数 比较坐标点的x坐标大小
struct LargeComparePointX : public binary_function<POINT_DOUBLE, double, bool>
{
	bool operator()( const POINT_DOUBLE& p, const double& x ) const
	{
		return ( p.first > x );
	}
};

struct LargeEqualComparePointX : public binary_function<POINT_DOUBLE, double, bool>
{
	bool operator()( const POINT_DOUBLE& p, const double& x ) const
	{
		return ( p.first >= x );
	}
};

struct LessComparePointX : public binary_function<POINT_DOUBLE, double, bool>
{
	bool operator()( const POINT_DOUBLE& p, const double& x ) const
	{
		return ( p.first < x );
	}
};

struct LessEqualComparePointX : public binary_function<POINT_DOUBLE, double, bool>
{
	bool operator()( const POINT_DOUBLE& p, const double& x ) const
	{
		return ( p.first <= x );
	}
};



UIChart::CurveInfo::CurveInfo()
{
	_isXCoordInOrder = true;			// 默认每个点集合在x轴坐标上有序

	_beginPointNO = -1;
	_endPointNO = -1;

	_color = _BLACK_;  
	_isSelect = false; 
	_isShow = true; 
	_isLine = true;
}
	
bool UIChart::CurveInfo::CalcCoordRange( double& xMin, double&xMax, double& yMin, double& yMax )		
{
	// 相关检查
	VECTOR_POINT::iterator itor = _pointList.begin();
	if( itor == _pointList.end() ) 
		return false;

	// 初始化一个值
	xMin = itor->first;
	xMax = xMin;
	yMin = itor->second;
	yMax = yMin;

	// 判定后面的点
	while( ++itor != _pointList.end() )
	{
		if( xMin > itor->first )
			xMin = itor->first;

		if( xMax < itor->first )
			xMax = itor->first;

		if( yMin > itor->second )
			yMin = itor->second;

		if( yMax < itor->second )
			yMax = itor->second;
	}

	return true;
}

// 判断点p0是否靠近线( p1,p2 )
bool UIChart::CurveInfo::JudgePointNearLine( POINT& p0, POINT& p1, POINT& p2 )						
{
	// 处理直线垂直的情况
	if( p1.x == p2.x )
	{
		if( abs( p1.x-p0.x )<=4 )
			return true;
		else 
			return false;
	}

	// 处理直线平行的情况
	if( p1.y == p2.y )
	{
		if( abs( p1.y-p0.y )<=4 )
			return true;
		else 
			return false;
	}

	// 普通情况 计算直线K B值
	double k = static_cast<double>( p2.y-p1.y )/( p2.x-p1.x );
	double b = static_cast<double>( p1.y - k*p1.x );
	// 计算与直线垂直的直线K B值
	double kVert = -1/k;
	double bVert = static_cast<double>( p0.y -kVert*p0.x );

	// 计算交点
	POINT intersectPoint;
	intersectPoint.x = static_cast<long>( ( b-bVert )/( kVert-k ) );
	intersectPoint.y = static_cast<long>( k*intersectPoint.x + b );

	// 计算两点间的距离
	double distanceValue = ( intersectPoint.y-p0.y )*( intersectPoint.y-p0.y )+( intersectPoint.x-p0.x )*( intersectPoint.x-p0.x );
	
	// 判断距离是否接近
	if( distanceValue<=25 )
		return true;
	else
		return false;
}
	
bool UIChart::CurveInfo::operator==( std::string strName ) 
{  
	return _name==strName; 
}

UIChart::UIChart()
{
	_isRoomEnoughDraw = true;

	_rowNum  = 8;
	_columnNum = 4;

	_isY1CoordRangeCalc = false;
	_isY2CoordRangeCalc = false;
	_isDrawZoomRect = false;

	_coordToPosScaleX = 0;
	_coordToPosScaleY1 = 0;
	_coordToPosScaleY2 = 0;

	_rPointPosUndo = _NULLPOINT_;
	_rPointPosMove = _NULLPOINT_;
	_lPointBeginPos = _NULLPOINT_;
	_lPointEndPos = _NULLPOINT_;

	_isMove = false;

	_isXCoordLimit = false;
	_isY1CoordLimit = false;
	_isY2CoordLimit = false;
	_xCoordMinLimit = _xCoordMaxLimit = 0.0;
	_y1CoordMinLimit = _y1CoordMaxLimit = 0.0;
	_y2CoordMinLimit = _y2CoordMaxLimit = 0.0;
	_isXCoordSymmetry = false;
	_isY1CoordSymmetry = false;
	_isY2CoordSymmetry = false;
}

void UIChart::Draw()
{
	//JudgeIsCalcArea();

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画边框
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _FRAMEBLUE_ );

	// 检查区域是否足够绘制
	if( _isRoomEnoughDraw==false ) 
		return;

	// 画grid
	DrawGrid();

	if( _isY1CoordRangeCalc || _isY2CoordRangeCalc )
	{
		DrawXCoordLable();							// 画x轴标签
		DrawXCoord();								// 画x轴坐标
	}

	if( _isY1CoordRangeCalc )
	{
		DrawY1CoordLable();							// 画y轴标签
		DrawY1Coord();								// 画y轴坐标

		DrawCurveList1();
	}

	if( _isY2CoordRangeCalc )
	{
		DrawY2CoordLable();							// 画y轴标签
		DrawY2Coord();								// 画y轴坐标

		DrawCurveList2();
	}

	DrawZoomRect();									// 画zoom框
	DrawMousePosAndToolTip();						// 画鼠标位置信息以及tooltip提示信息
}

void UIChart::DrawGrid()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画边框
	RECT rc = _gridRect;
	OffsetRect( &rc, x_, y_ );
	UI2DRect( rc, _z )( _BLACK_ );

	int dxPos = GetWidth()( _gridRect )/_columnNum;
	int dyPos = GetHeight()( _gridRect )/_rowNum;

	// draw _columnNum column
	int xPos = _gridRect.left;
	for( int i=1; i<_columnNum; ++i )
	{
		xPos += dxPos;
		UI2DLine( xPos+x_, _gridRect.top+1+y_, xPos+x_, _gridRect.bottom-1+y_ )( _GRAY_ );
	}

	// draw _rowNum rows
	int yPos = _gridRect.top;
	for( int i=1; i<_rowNum; ++i )
	{
		yPos += dyPos;
		UI2DLine( _gridRect.left+1+x_, yPos+y_, _gridRect.right-1+x_, yPos+y_ )( _GRAY_ );
	}
}

void UIChart::DrawXCoordLable()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z );

	// Draw x axis name
	SIZE szLabel = fontHelp.GetDrawAreaSize( "X" );
	RECT rc = GenRect()( GenPoint()( ( _clientRC.right-szLabel.cx )/2, _clientRC.bottom-szLabel.cy-3 ), szLabel );
	OffsetRect( &rc, x_, y_ );
	fontHelp( "X", rc );
}

void UIChart::DrawY1CoordLable()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z );

	// Draw y1 axis name
	SIZE szLabel = fontHelp.GetDrawAreaSize( "Y1" );
	RECT rc = GenRect()( GenPoint()( 3, ( _clientRC.bottom-szLabel.cy )/2 ), szLabel );
	OffsetRect( &rc, x_, y_ );
	fontHelp( "Y1", rc );
}

void UIChart::DrawY2CoordLable()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z );

	// Draw y2 axis name
	SIZE szLabel = fontHelp.GetDrawAreaSize( "Y2" );
	RECT rc = GenRect()( GenPoint()( _clientRC.right-szLabel.cx-3, ( _clientRC.bottom-szLabel.cy )/2 ), szLabel );
	OffsetRect( &rc, x_, y_ );
	fontHelp( "Y2", rc );
}

void UIChart::DrawXCoord()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z, 16 );

	string str;
    int dxPos = ( _gridRect.right - _gridRect.left )/_columnNum;
    int xPos;

	// draw abscissa
    double dxCoord = ( _xCoordRange.second - _xCoordRange.first )/_columnNum;
    double xCoord = _xCoordRange.first;
    xPos = _gridRect.left;
    for( int i=0; i<_columnNum+1; ++i )
    {
		STRING_FORMAT( str, "%.2lf", xCoord );
        SIZE strSize = fontHelp.GetDrawAreaSize( str );

		RECT rc = GenRect()( GenPoint()( xPos-strSize.cx/2, _gridRect.bottom+strSize.cy/2 ), strSize );
		OffsetRect( &rc, x_, y_ );
		fontHelp( str, rc );

        xCoord += dxCoord;
        xPos += dxPos;
    }

	// calculate the scales
	_coordToPosScaleX = ( _xCoordRange.second-_xCoordRange.first )/( _gridRect.right-_gridRect.left );
}

void UIChart::DrawY1Coord()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z, 16 );

	string str;
    int dyPos = ( _gridRect.bottom - _gridRect.top )/_rowNum;
    int yPos;

	// draw ordinate
	if( _isY1CoordRangeCalc )
	{
		double dyCoord = ( _y1CoordRange.second - _y1CoordRange.first )/_rowNum;
		double yCoord = _y1CoordRange.first;
		yPos = _gridRect.bottom;
		for( int i=0; i<_rowNum+1; ++i )
		{
			STRING_FORMAT( str, "%.2lf", yCoord );
			SIZE strSize = fontHelp.GetDrawAreaSize( str );

			RECT rc = GenRect()( GenPoint()( _gridRect.left-strSize.cx-1, yPos-strSize.cy/2 ), strSize );
			OffsetRect( &rc, x_, y_ );
			fontHelp( str, rc );

			yCoord += dyCoord;
			yPos -= dyPos;
		}
	}

	_coordToPosScaleY1 = ( _y1CoordRange.second-_y1CoordRange.first )/( _gridRect.bottom-_gridRect.top );
}

void UIChart::DrawY2Coord()
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z, 16 );

	string str;
    int dyPos = ( _gridRect.bottom - _gridRect.top )/_rowNum;
    int yPos;

	if( _isY2CoordRangeCalc )
	{
		double dyCoord = ( _y2CoordRange.second - _y2CoordRange.first )/_rowNum;
		double yCoord = _y2CoordRange.first;
		yPos = _gridRect.bottom;
		for( int i=0; i<_rowNum+1; ++i )
		{
			STRING_FORMAT( str, "%.2lf", yCoord );
			SIZE strSize = fontHelp.GetDrawAreaSize( str );

			RECT rc = GenRect()( GenPoint()( _gridRect.right+1, yPos-strSize.cy/2 ), strSize );
			OffsetRect( &rc, x_, y_ );
			fontHelp( str, rc );

			yCoord += dyCoord;
			yPos -= dyPos;
		}
	}

	_coordToPosScaleY2 = ( _y2CoordRange.second-_y2CoordRange.first )/( _gridRect.bottom-_gridRect.top );
}

void UIChart::DrawMousePosAndToolTip()
{
	if( _curveList1.size()==0 && _curveList2.size()==0 ) 
		return;

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	UI2DFont fontHelp( _z, 16 );

	// 显示鼠标所在坐标
	{
		SIZE strSize = fontHelp.GetDrawAreaSize( _mouseCoordStr );

		RECT rc = GenRect()( GenPoint()( _gridRect.right-strSize.cx-2, _gridRect.top-strSize.cy-2 ), strSize );
		OffsetRect( &rc, x_, y_ );
		fontHelp( _mouseCoordStr, rc );
	}

	if( _isShowToolTip )
	{
		SIZE strSize = fontHelp.GetDrawAreaSize( _tooltipStr );

		RECT rc = GenRect()( GenPoint()( _gridRect.left+2, _gridRect.top-strSize.cy-2 ), strSize );
		OffsetRect( &rc, x_, y_ );
		fontHelp( _tooltipStr, rc );
	}
}

void UIChart::DrawCurveList1()
{ 
	if( _curveList1.empty() ) 
		return;

	// Painting all of the curves
	for( CURVE_LIST::iterator itor=_curveList1.begin(); itor!=_curveList1.end(); ++itor )
	{
		if( itor->_isShow==true )
			DrawCurve( *itor, 1 );
	}
}

void UIChart::DrawCurveList2()
{
	if( _curveList2.empty() ) 
		return;

	// Painting all of the curves
	for( CURVE_LIST::iterator itor=_curveList2.begin(); itor!=_curveList2.end(); ++itor )
	{
		if( itor->_isShow==true )
			DrawCurve( *itor, 2 );
	}
}

void UIChart::DrawCurve( CurveInfo& curve, int yFlag )
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 线条检查
	if( curve._pointList.size() == 0 ) 
		return;
	if( curve._beginPointNO<0 ) 
		return;
	
	bool prePointInBoxFlag;
	POINT prePointPos, curPointPos;

	// 计算绘制点的范围 ( firstItor, lastItor ) )
	VECTOR_POINT::iterator firstItor, lastItor;
	firstItor = curve._pointList.begin()+curve._beginPointNO;
	if( firstItor!=curve._pointList.begin() )
		--firstItor;
	// 
	if( curve._endPointNO==curve._pointList.size() )
	{
		lastItor = curve._pointList.end();
	}
	else
	{
		lastItor = curve._pointList.begin()+curve._endPointNO;
		++lastItor;
		if( lastItor!=curve._pointList.end() )
			++lastItor;
	}

	// Processing the first point
	VECTOR_POINT::iterator itor = firstItor;
	VECTOR_POINT::iterator preItor = itor;
	if( IsCoordPointInCoordRange( *itor, yFlag )==false )
	{
		prePointInBoxFlag = false;
	}
	else
	{
		TransfromCoordToPos( curPointPos, *itor, yFlag );
		DrawPoint( curPointPos, curve._color, curve._isSelect );

		prePointPos = curPointPos;
		prePointInBoxFlag = true;
	}
	++itor;
	if( itor>=lastItor )
		return;

	// Processing the remaining point
	do{	
		if( IsCoordPointInCoordRange( *itor, yFlag )==false )  // this point out the range
		{	
			if( curve._isLine==false )
				goto _CALCNEXTPOINT;

			if( prePointInBoxFlag==true ) // the previous point is in the range
			{
				POINT_DOUBLE IntersectionPointCoord = CalcIntersectionPoint( *preItor, *itor, 2, yFlag );
				POINT IntersectionPointPos;
				TransfromCoordToPos( IntersectionPointPos, IntersectionPointCoord, yFlag );

				UI2DLine(	prePointPos.x+x_, prePointPos.y+y_, IntersectionPointPos.x+x_, IntersectionPointPos.y+y_, _z )( curve._color );
			}
			// 
			prePointInBoxFlag = false; 
		}
		else  // this point is in the range
		{
			TransfromCoordToPos( curPointPos, *itor, yFlag );
			DrawPoint( curPointPos, curve._color, curve._isSelect );

			if( curve._isLine==false )
				goto _CALCNEXTPOINT;

			if( prePointInBoxFlag==true )	// the previous point is in the range
			{
				UI2DLine( prePointPos.x+x_, prePointPos.y+y_, curPointPos.x+x_, curPointPos.y+y_, _z )( curve._color );
			}
			else  // the previous point is out the range
			{
				POINT_DOUBLE IntersectionPointCoord = CalcIntersectionPoint( *preItor, *itor, 1, yFlag );
				POINT IntersectionPointPos;
				TransfromCoordToPos( IntersectionPointPos, IntersectionPointCoord, yFlag );

				UI2DLine( IntersectionPointPos.x+x_, IntersectionPointPos.y+y_, curPointPos.x+x_, curPointPos.y+y_, _z )( curve._color );
			}
			// 
			prePointPos = curPointPos;
			prePointInBoxFlag = true;
		}

_CALCNEXTPOINT:  // 计算要绘制下一个点
		preItor = itor;
		++itor;
	}while( itor < lastItor );
}

void UIChart::DrawPoint( POINT& pointPos, D3DCOLOR& color, bool bigPointFlag )
{
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	//UI2DPoint( pointPos.x+x_, pointPos.y+y_, _z )( color );
	if( bigPointFlag==true )
	{
		RECT rect;
		rect.top	= pointPos.y - 2;
		rect.bottom = pointPos.y + 2;
		rect.left	= pointPos.x - 2;
		rect.right	= pointPos.x + 2;
		OffsetRect( &rect, x_, y_ );
		UI2DRect( rect, _z )( color, 255 );
	}
}

void UIChart::DrawZoomRect()
{
	if( _isDrawZoomRect==false )
		return;

	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 起点方块
	UI2DRect( _lPointBeginPos.x-2+x_, _lPointBeginPos.y-2+y_, 4, 4, _z )( D3DCOLOR_XRGB(68, 166, 255), 255 );

	// 终点方块
	UI2DRect( _lPointEndPos.x-2+x_, _lPointEndPos.y-2+y_, 4, 4, _z )( D3DCOLOR_XRGB(68, 166, 255), 255 );

	// 中间透明区域
	RECT rc;
	rc.left = _lPointBeginPos.x<=_lPointEndPos.x ? _lPointBeginPos.x:_lPointEndPos.x;
	rc.right = _lPointBeginPos.x>_lPointEndPos.x ? _lPointBeginPos.x:_lPointEndPos.x;
	rc.top = _lPointBeginPos.y<=_lPointEndPos.y ? _lPointBeginPos.y:_lPointEndPos.y;
	rc.bottom = _lPointBeginPos.y>_lPointEndPos.y ? _lPointBeginPos.y:_lPointEndPos.y;
	OffsetRect( &rc, x_, y_ );

	UI2DRect( rc, _z )( _FRAMEBLUE_, 50 );
}

void UIChart::CalcArea()
{
	// 检查区域大小是否足够
	if( GetWidth()( _clientRC )<100 || GetHeight()( _clientRC )<80 )
		_isRoomEnoughDraw = false;
	else
		_isRoomEnoughDraw = true;

	// grid区域
    _gridRect = _clientRC;
	_gridRect.left += 50;
	_gridRect.right -= 50;
	_gridRect.top += 30;
	_gridRect.bottom -= 50;
	
	// 调整
	int dxPos = GetWidth()( _gridRect )/_columnNum;
	int dyPos = GetHeight()( _gridRect )/_rowNum;
	_gridRect.right = _gridRect.left + dxPos*_columnNum;
	_gridRect.bottom = _gridRect.top + dyPos*_rowNum;
}

void UIChart::CalcCurveListDrawRange( int mode )
{
	// 相关检查
	if( _isY1CoordRangeCalc )
	{
		for( CURVE_LIST::iterator itor=_curveList1.begin(); itor!=_curveList1.end(); ++itor )
			CalcCurveDrawRange( *itor, mode );
	}

	if( _isY2CoordRangeCalc )
	{
		for( CURVE_LIST::iterator itor=_curveList2.begin(); itor!=_curveList2.end(); ++itor )
			CalcCurveDrawRange( *itor, mode );
	}
}

// [_beginPointNO, _endPointNO]
// 绘制范围只是对于x轴来进行计算的 不用区分y轴
void UIChart::CalcCurveDrawRange( CurveInfo& curve, int mode )
{
	// 是否存在点
	if( curve._pointList.size()==0 ) return;

	// x轴上数据无序时的计算
	if( curve._isXCoordInOrder==false )
	{
		curve._beginPointNO = 0;
		curve._endPointNO = curve._pointList.size()-1;
		return;
	}

	// x轴上数据有序时的计算
	// 处理极限情况 坐标点集合不在坐标范围内
	if( curve._pointList.begin()->first>_xCoordRange.second || curve._pointList.back().first<_xCoordRange.first )
	{
		curve._beginPointNO = -1;
		return;
	}

	// 下面所有的情况一定在坐标范围内存在需要绘制的点
	VECTOR_POINT::iterator beginPointItor, endPointItor;
	if( mode == NOTUSELASTDATA )		// 不利用上次的窗口 重新计算
	{	
_NOTUSELASTDATE:
		beginPointItor = find_if( curve._pointList.begin(), curve._pointList.end(), bind2nd( LargeEqualComparePointX(), _xCoordRange.first ) );
		endPointItor = find_if( beginPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.second ) );
		--endPointItor;
	}
	else if( curve._beginPointNO<0 )	// 上一次的窗口不存在
	{
		if( mode == RIGHTMOVE )			// 曲线右移
		{
			beginPointItor = find_if( curve._pointList.rbegin(), curve._pointList.rend(), bind2nd( LessComparePointX(), _xCoordRange.first ) ).base();
			endPointItor = find_if( beginPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.second ) );
			--endPointItor;
		}
		else if( mode == LEFTMOVE )		// 曲线左移
		{
			goto _NOTUSELASTDATE;
		}
		else if( mode == ZOOM )			// 放大部分曲线
		{
		}
		else if( mode == SHRINK )		// 收缩部分曲线
		{
			goto _NOTUSELASTDATE;
		}
		else if( mode == ADDPOINT )		// 添加坐标点
		{
			if( curve._pointList.back().first>=_xCoordRange.first && curve._pointList.back().first<=_xCoordRange.second )
			{
				beginPointItor = curve._pointList.end()-1;
				endPointItor = beginPointItor;
			}
		}
	}
	else	// 利用上次的窗口值 加速计算
	{		// 获取上一次的窗口
		beginPointItor = curve._pointList.begin()+curve._beginPointNO;
		if( curve._endPointNO==curve._pointList.size() )
			endPointItor = curve._pointList.end();
		else
			endPointItor = curve._pointList.begin()+curve._endPointNO;

		if( mode == RIGHTMOVE )				// 曲线右移
		{	
			// 利用上次计算区间计算
			VECTOR_POINT::reverse_iterator ri( beginPointItor+1 );  
			ri = find_if( ri, curve._pointList.rend(), bind2nd( LessComparePointX(), _xCoordRange.first ) );
			beginPointItor = ri.base();	
			// 
			VECTOR_POINT::reverse_iterator ri2( endPointItor+1 );
			ri2 = find_if( ri2, curve._pointList.rend(), bind2nd( LessComparePointX(), _xCoordRange.second ) );
			endPointItor = ri2.base()-1;
		}
		else if( mode == LEFTMOVE )			// 曲线左移
		{
			// 利用上次计算区间计算
			beginPointItor = find_if( beginPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.first ) );
			// 
			endPointItor = find_if( endPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.second ) );
			--endPointItor;
		}
		else if( mode == ZOOM )				// 放大部分曲线
		{
			beginPointItor = find_if( beginPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.first ) );

			VECTOR_POINT::reverse_iterator ri2( endPointItor+1 );
			ri2 = find_if( ri2, curve._pointList.rend(), bind2nd( LessComparePointX(), _xCoordRange.second ) );
			endPointItor = ri2.base()-1;
		}
		else if( mode == SHRINK )			// 收缩部分曲线
		{
			VECTOR_POINT::reverse_iterator ri( beginPointItor+1 );  
			ri = find_if( ri, curve._pointList.rend(), bind2nd( LessComparePointX(), _xCoordRange.first ) );
			beginPointItor = ri.base();	

			endPointItor = find_if( endPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.second ) );
			--endPointItor;
		}
		else if( mode == ADDPOINT )			// 添加坐标点
		{
			// 利用上次计算区间计算
			endPointItor = find_if( endPointItor, curve._pointList.end(), bind2nd( LargeComparePointX(), _xCoordRange.second ) );
			--endPointItor;
		}
	}

	// 检查
	if( endPointItor<beginPointItor )
		endPointItor = beginPointItor;

	// 将窗口迭代器转换回NO
	if( beginPointItor==curve._pointList.end() )
	{
		curve._beginPointNO = -1;
		return;
	}
	else 
	{
		curve._beginPointNO = beginPointItor - curve._pointList.begin();
		curve._endPointNO = endPointItor - curve._pointList.begin();
	}
}

void UIChart::CalcCoordSymmetry( POINT_DOUBLE& pointCoord )
{
	if( pointCoord.first<=0 && pointCoord.second>=0 )
	{
		if( pointCoord.second>-pointCoord.first )
			pointCoord.first = -pointCoord.second;
		else
			pointCoord.second = -pointCoord.first;
	}
	else if( pointCoord.first>0 )
	{
		pointCoord.first = -pointCoord.second;
	}
	else if( pointCoord.second<0 )
	{
		pointCoord.second = -pointCoord.first;
	}
}

bool UIChart::IsCoordPointInCoordRange( POINT_DOUBLE& pointCoord, int yFlag )
{
	POINT_DOUBLE _yCoordRange = ( yFlag==1 )?_y1CoordRange:_y2CoordRange;

	return ( ( pointCoord.first>=_xCoordRange.first && pointCoord.first<=_xCoordRange.second ) &&   
				 ( pointCoord.second>=_yCoordRange.first && pointCoord.second<=_yCoordRange.second ) );
}

bool UIChart::IsCoordPointInbox( POINT_DOUBLE& pointCoord, POINT_DOUBLE& p1, POINT_DOUBLE& p2, bool nearYFlag )
{
	POINT_DOUBLE xCoordRange, yCoordRange;
	xCoordRange.first = p1.first<p2.first ? p1.first:p2.first;
	xCoordRange.second = p1.first>p2.first ? p1.first:p2.first;
	yCoordRange.first = p1.second<p2.second ? p1.second:p2.second;
	yCoordRange.second = p1.second>p2.second ? p1.second:p2.second;

	if( nearYFlag )
	{
		yCoordRange.first -= 3;
		yCoordRange.second += 3;
	}

	return ( ( pointCoord.first>=xCoordRange.first && pointCoord.first<=xCoordRange.second )		&&   
			 ( pointCoord.second>=yCoordRange.first && pointCoord.second<=yCoordRange.second )	 );
}

// judge whether the postion point in a box range 
bool UIChart::IsPosPointInbox( POINT& pointPos, RECT& rect )
{
	return !( pointPos.x<rect.left || pointPos.x>rect.right || pointPos.y<rect.top || pointPos.y>rect.bottom );
}
	
bool UIChart::IsPointNearCurveLine( POINT& point, CurveInfo& curve, int yFlag )
{
	if( !curve._isLine )	
		return false;

	VECTOR_POINT::iterator beginPointItor, endPointItor;
	beginPointItor = curve._pointList.begin()+curve._beginPointNO;
	endPointItor = curve._pointList.begin()+curve._endPointNO;
	
	bool prePointInBoxFlag;
	POINT prePointPos, curPointPos;

	POINT_DOUBLE pointCoord;
	TransfromPosToCoord( point, pointCoord, yFlag );
	
	// 先计算要进行判定的范围
	VECTOR_POINT::iterator firstItor, lastItor;
	firstItor = beginPointItor;
	if( firstItor!=curve._pointList.begin() )
		--firstItor;
	// 
	lastItor = endPointItor+1;
	if( lastItor!=curve._pointList.end() )
		++lastItor;

	// process first point
	VECTOR_POINT::iterator itor = firstItor;
	if( IsCoordPointInCoordRange( *itor, yFlag )==false )
		prePointInBoxFlag = false;
	else
		prePointInBoxFlag = true;

	// process the remaining
	while( ++itor != lastItor )
	{	
		if( IsCoordPointInCoordRange( *itor, yFlag )==false )
		{
			if( prePointInBoxFlag==false ) 
				continue;

			prePointInBoxFlag = false; 
		}
		else 
		{
			prePointInBoxFlag = true;
		}

		// The current point is not within the box
		if( IsCoordPointInbox( pointCoord, *( itor-1 ), *itor, true )==false )
			continue;

		// 
		TransfromCoordToPos( prePointPos, *( itor-1 ), yFlag );
		TransfromCoordToPos( curPointPos, *itor, yFlag );

		// 
		if( curve.JudgePointNearLine( point, prePointPos, curPointPos ) )
			return true;
	}
		
	return false;
}

bool UIChart::IsPointNearCurve( POINT& point, int yFlag )
{
	CURVE_LIST& cuveList = yFlag==1?_curveList1:_curveList2;
	for( CURVE_LIST::iterator i=cuveList.begin(); i!=cuveList.end(); ++i )
	{
		// 相关检查
		if( i->_isShow==false ) continue;
		if( i->_pointList.size()==0 ) continue;
		if( i->_beginPointNO<0 ) continue;
	
		//
		if( IsPointNearCurvePoint( point, *i, yFlag ) )
		{
			i->_isSelect = true;
			return true;
		}
	}

	return false;
}

bool UIChart::IsPointNearCurvePoint( POINT& point, CurveInfo& curve, int yFlag )
{
	VECTOR_POINT::iterator beginPointItor, endPointItor;
	beginPointItor = curve._pointList.begin()+curve._beginPointNO;
	endPointItor = curve._pointList.begin()+curve._endPointNO;

	// 判定所有的点
	for( VECTOR_POINT::iterator it=beginPointItor; it!=endPointItor+1; ++it )
	{
		if( !IsCoordPointInCoordRange( *it, yFlag ) )
			continue;

		// 判断是否接近只能通过pos来比较
		POINT pointPos;
		TransfromCoordToPos( pointPos, *it, yFlag );
		if( ( pointPos.x-point.x )*( pointPos.x-point.x )+( pointPos.y-point.y )*( pointPos.y-point.y )>16 )
			continue;

		STRING_FORMAT( _tooltipStr, "%s name=%s; coord=[%0.3lf, %0.3lf]; index=%d", yFlag==1?"Y1":"Y2", curve._name.c_str(), it->first, it->second, it-curve._pointList.begin() );
		return true;
	}

	return false;
}

void UIChart::TransfromCoordToPos( POINT& pointPos, POINT_DOUBLE& pointCoord, int yFlag )
{
	pointPos.x = _gridRect.left + static_cast<long>( ( pointCoord.first - _xCoordRange.first )/_coordToPosScaleX );

	if( yFlag==1 )
		pointPos.y = _gridRect.bottom - static_cast<long>( ( pointCoord.second - _y1CoordRange.first )/_coordToPosScaleY1 );
	else
		pointPos.y = _gridRect.bottom - static_cast<long>( ( pointCoord.second - _y2CoordRange.first )/_coordToPosScaleY2 );
}

bool UIChart::TransfromPosToCoord( POINT& pointPos, POINT_DOUBLE& pointCoord, int yFlag )
{
	// Judge whether in the box
	if( !IsPosPointInbox( pointPos, _gridRect ) ) 
		return false;

	pointCoord.first = _xCoordRange.first + static_cast<double>( pointPos.x - _gridRect.left )*_coordToPosScaleX;
	if( yFlag==1 )
		pointCoord.second = _y1CoordRange.first + static_cast<double>( _gridRect.bottom - pointPos.y )*_coordToPosScaleY1;
	else
		pointCoord.second = _y2CoordRange.first + static_cast<double>( _gridRect.bottom - pointPos.y )*_coordToPosScaleY2;

	return true;
}

// 计算两点与坐标范围box的交点 一定有一个点在box内 一个在外
/*
  1	|		2	  | 3
 ---|-------------|----
	|			  |
  4	|			  | 5
	|             |
 ---|-------------|----
  6	|		7	  | 8
*/
POINT_DOUBLE UIChart::CalcIntersectionPoint( POINT_DOUBLE& prePoint, POINT_DOUBLE& curPoint, int outBoxIndex, int yFlag )
{
	POINT_DOUBLE intersectPoint;
	POINT_DOUBLE outBoxPoint;

	POINT_DOUBLE yCoordRange = ( yFlag==1 ) ? _y1CoordRange:_y2CoordRange ;

	if( outBoxIndex==1 )
		outBoxPoint = prePoint;
	else 
		outBoxPoint = curPoint;

	// Exclusion of the vertical
	if( prePoint.first == curPoint.first )
	{	
		intersectPoint.first = curPoint.first;
		if( outBoxPoint.second < yCoordRange.first )
		{
			intersectPoint.second = yCoordRange.first;
			return intersectPoint;
		}
		else
		{
			intersectPoint.second = yCoordRange.second;
			return intersectPoint;
		}
	}

	// Find the K B value
	double k = ( curPoint.second - prePoint.second ) / ( curPoint.first - prePoint.first );
	double b = curPoint.second - k*( curPoint.first );
	
	// judge the regional
	if( ( outBoxPoint.first >= _xCoordRange.first ) && ( outBoxPoint.first <= _xCoordRange.second ) )
	{
		if( outBoxPoint.second > yCoordRange.first )  // regional2
		{ 
			CalcTopIntersectionPoint( intersectPoint, k, b, yFlag );
		}
		else  // regional7
		{
			CalcBottomIntersectionPoint( intersectPoint, k, b, yFlag );
		}
	}
	else if( ( outBoxPoint.second >= yCoordRange.first ) && ( outBoxPoint.second <= yCoordRange.second ) )
	{
		if( outBoxPoint.first < _xCoordRange.first )  // regional4
		{ 
			CalcLeftIntersectionPoint( intersectPoint, k, b, yFlag );
		}
		else  // regional5
		{
			CalcRightIntersectionPoint( intersectPoint, k, b, yFlag );
		}		
	}
	else if( ( outBoxPoint.first < _xCoordRange.first )&&( outBoxPoint.second > yCoordRange.second ) ) // regional1
	{
		if( CalcLeftIntersectionPoint( intersectPoint, k, b, yFlag )==false )
			CalcTopIntersectionPoint( intersectPoint, k, b, yFlag );
	}
	else if( ( outBoxPoint.first > _xCoordRange.first )&&( outBoxPoint.second > yCoordRange.second ) ) // regional3
	{
		if( CalcRightIntersectionPoint( intersectPoint, k, b, yFlag )==false )
			CalcTopIntersectionPoint( intersectPoint, k, b, yFlag );		
	}
	else if( ( outBoxPoint.first < _xCoordRange.first )&&( outBoxPoint.second < yCoordRange.first ) ) // regional6
	{
		if( CalcLeftIntersectionPoint( intersectPoint, k, b, yFlag )==false )
			CalcBottomIntersectionPoint( intersectPoint, k, b, yFlag );			
	}
	else if( ( outBoxPoint.first > _xCoordRange.first )&&( outBoxPoint.second < yCoordRange.first ) ) // regional8
	{
		if( CalcRightIntersectionPoint( intersectPoint, k, b, yFlag )==false )
			CalcBottomIntersectionPoint( intersectPoint, k, b, yFlag );	
	}

	return intersectPoint;
}

// calculate the intersection of line and box's left line ( coordinate )
bool UIChart::CalcLeftIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag )
{
	POINT_DOUBLE yCoordRange = ( yFlag==1 ) ? _y1CoordRange:_y2CoordRange ;

	pointCoord.first = _xCoordRange.first;
	pointCoord.second = k*( pointCoord.first )+b;
	return pointCoord.second>=yCoordRange.first && pointCoord.second<=yCoordRange.second;
}

// calculate the intersection of line and box's top line ( coordinate )
bool UIChart::CalcTopIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag )
{
	POINT_DOUBLE yCoordRange = ( yFlag==1 ) ? _y1CoordRange:_y2CoordRange ;

	pointCoord.second = yCoordRange.second;
	pointCoord.first = ( pointCoord.second-b )/k;
	return pointCoord.first>=_xCoordRange.first && pointCoord.first<=_xCoordRange.second;
}

// calculate the intersection of line and box's right line ( coordinate )
bool UIChart::CalcRightIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag )
{
	POINT_DOUBLE yCoordRange = ( yFlag==1 ) ? _y1CoordRange:_y2CoordRange ;

	pointCoord.first = _xCoordRange.second;
	pointCoord.second = k*( pointCoord.first )+b;
	return pointCoord.second>=yCoordRange.first && pointCoord.second<=yCoordRange.second;
}

// calculate the intersection of line and box's bottom line ( coordinate )
bool UIChart::CalcBottomIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag )
{
	POINT_DOUBLE yCoordRange = ( yFlag==1 ) ? _y1CoordRange:_y2CoordRange ;

	pointCoord.second = yCoordRange.first;
	pointCoord.first = ( pointCoord.second-b )/k;
	return pointCoord.first>=_xCoordRange.first && pointCoord.first<=_xCoordRange.second;
}

void UIChart::CalcXYCoordRange()
{
	UIPostMessage( this, WM_UPDATE, 0, 0 );
}

void UIChart::AddCurve1( std::string strName )
{
	Cache1 *pCache = new Cache1;
	pCache->_curveName = strName;
	pCache->_yFlag = 1;
	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UIChart::AddCurve1( std::string strName, double xValue, double yValue )
{
	Cache2 *pCache = new Cache2;
	pCache->_x = xValue;
	pCache->_y = yValue;
	pCache->_curveName = strName;
	pCache->_yFlag = 1;
	UIPostMessage( this, WM_UPDATE, 2, (LPARAM)pCache );
}

void UIChart::AddCurve1( std::string strName, VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, bool isXCoordInOrder )
{
	Cache3 *pCache = new Cache3;
	pCache->_xList = xList;
	pCache->_yList = yList;
	pCache->_curveName = strName;
	pCache->_yFlag = 1;
	pCache->_isXCoordInOrder = isXCoordInOrder;
	UIPostMessage( this, WM_UPDATE, 3, (LPARAM)pCache );
}

void UIChart::AddCurve2( std::string strName )
{
	Cache1 *pCache = new Cache1;
	pCache->_curveName = strName;
	pCache->_yFlag = 2;
	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UIChart::AddCurve2( std::string strName, double xValue, double yValue )
{
	Cache2 *pCache = new Cache2;
	pCache->_x = xValue;
	pCache->_y = yValue;
	pCache->_curveName = strName;
	pCache->_yFlag = 2;
	UIPostMessage( this, WM_UPDATE, 2, (LPARAM)pCache );
}

void UIChart::AddCurve2( std::string strName, VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, bool isXCoordInOrder )
{
	Cache3 *pCache = new Cache3;
	pCache->_xList = xList;
	pCache->_yList = yList;
	pCache->_curveName = strName;
	pCache->_yFlag = 2;
	pCache->_isXCoordInOrder = isXCoordInOrder;
	UIPostMessage( this, WM_UPDATE, 3, (LPARAM)pCache );
}

void UIChart::SetCurve1Color( std::string strName, D3DCOLOR color )
{
	Cache4 *pCache = new Cache4;
	pCache->_color = color;
	pCache->_curveName = strName;
	pCache->_yFlag = 1;
	UIPostMessage( this, WM_UPDATE, 4, (LPARAM)pCache );
}

void UIChart::SetCurve2Color( std::string strName, D3DCOLOR color )
{
	Cache4 *pCache = new Cache4;
	pCache->_color = color;
	pCache->_curveName = strName;
	pCache->_yFlag = 2;
	UIPostMessage( this, WM_UPDATE, 4, (LPARAM)pCache );
}

void UIChart::SetXCoordLimit( double xMinLimit, double xMaxLimit )
{
	Cache5 *pCache = new Cache5;
	pCache->_flag = 1;
	pCache->_min = xMinLimit;
	pCache->_max = xMaxLimit;
	UIPostMessage( this, WM_UPDATE, 6, (LPARAM)pCache );
}

void UIChart::SetY1CoordLimit( double y1MinLimit, double y1MaxLimit )
{
	Cache5 *pCache = new Cache5;
	pCache->_flag = 2;
	pCache->_min = y1MinLimit;
	pCache->_max = y1MaxLimit;
	UIPostMessage( this, WM_UPDATE, 6, (LPARAM)pCache );
}

void UIChart::SetY2CoordLimit( double y2MinLimit, double y2MaxLimit )
{
	Cache5 *pCache = new Cache5;
	pCache->_flag = 3;
	pCache->_min = y2MinLimit;
	pCache->_max = y2MaxLimit;
	UIPostMessage( this, WM_UPDATE, 6, (LPARAM)pCache );
}

void UIChart::SetXCoordSymmetry()
{
	Cache5 *pCache = new Cache5;
	pCache->_flag = 4;
	UIPostMessage( this, WM_UPDATE, 6, (LPARAM)pCache );	
}

void UIChart::SetY1CoordSymmetry()
{
	Cache5 *pCache = new Cache5;
	pCache->_flag = 5;
	UIPostMessage( this, WM_UPDATE, 6, (LPARAM)pCache );	
}

void UIChart::SetY2CoordSymmetry()
{
	Cache5 *pCache = new Cache5;
	pCache->_flag = 6;
	UIPostMessage( this, WM_UPDATE, 6, (LPARAM)pCache );	
}

void UIChart::SetCurve1Select( std::string strName )
{
	Cache1 *pCache = new Cache1;
	pCache->_curveName = strName;
	pCache->_yFlag = 1;
	UIPostMessage( this, WM_UPDATE, 7, (LPARAM)pCache );
}

void UIChart::SetCurve2Select( std::string strName )
{
	Cache1 *pCache = new Cache1;
	pCache->_curveName = strName;
	pCache->_yFlag = 2;
	UIPostMessage( this, WM_UPDATE, 7, (LPARAM)pCache );
}

void UIChart::Clear()
{
	UIPostMessage( this, WM_UPDATE, 5, 0 );
}

void UIChart::SaveCurCoordRange()
{
	if( _isY1CoordRangeCalc || _isY2CoordRangeCalc )
	{
		_xCoordHistory.push_back( make_pair( _xCoordRange.first, _xCoordRange.second ) );
		_y1CoordHistory.push_back( make_pair( _y1CoordRange.first, _y1CoordRange.second ) );
		_y2CoordHistory.push_back( make_pair( _y2CoordRange.first, _y2CoordRange.second ) );
	}
}

void UIChart::SetXYCoordRange( double xMin, double xMax, double y1Min, double y1Max, double y2Min, double y2Max )
{
	SaveCurCoordRange();

	_xCoordRange.first = xMin;
	_xCoordRange.second = xMax;
	_y1CoordRange.first = y1Min;
	_y1CoordRange.second = y1Max;
	_y2CoordRange.first = y2Min;
	_y2CoordRange.second = y2Max;

	_isY1CoordRangeCalc = _curveList1.size()>0 ? true:false;
	_isY2CoordRangeCalc = _curveList2.size()>0 ? true:false;

	CalcCurveListDrawRange( NOTUSELASTDATA );

	UIRefresh();
}

void UIChart::GetXYCoordRange( double& xMin, double& xMax, double& y1Min, double& y1Max, double& y2Min, double& y2Max )
{
	xMin = _xCoordRange.first;
	xMax = _xCoordRange.second;
	y1Min = _y1CoordRange.first;
	y1Max = _y1CoordRange.second;
	y2Min = _y2CoordRange.first;
	y2Max = _y2CoordRange.second;
}

bool UIChart::OnLButtonDown( POINT pt )
{
	// 获取相对位置
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	// 判断
	if( !IsPosPointInbox( point, _gridRect ) ) 
		return true;
	
	// 保存该点
	_lPointBeginPos = point;	

	//
	for_each( _curveList1.begin(), _curveList1.end(), []( CurveInfo& curve ) { curve._isSelect = false; } );
	for_each( _curveList2.begin(), _curveList2.end(), []( CurveInfo& curve ) { curve._isSelect = false; } );

	if( _isY1CoordRangeCalc ) 
	{
		if( IsPointNearCurve( point, 1 ) )
			return true;
	}

	if( _isY2CoordRangeCalc ) 
	{
		IsPointNearCurve( point, 2 );
	}

	return true;
}

bool UIChart::OnLButtonUp( POINT pt )
{
	if( _isDrawZoomRect )
	{
		_isDrawZoomRect = false;

		if( !_isY1CoordRangeCalc && !_isY2CoordRangeCalc )
			return true;

		if( abs( _lPointEndPos.y-_lPointBeginPos.y )<=3 && abs( _lPointEndPos.x-_lPointBeginPos.x )<=3 ) 
			return true;


		bool xFlag = false;
		double xMin, xMax, y1Min=0, y1Max=0;
		if( _isY1CoordRangeCalc )
		{
			// calculate the zoom rect
			POINT_DOUBLE startPointCoord, endPointCoord;
			TransfromPosToCoord( _lPointBeginPos, startPointCoord, 1 );
			TransfromPosToCoord( _lPointEndPos, endPointCoord, 1 );
			
			xMin = startPointCoord.first<endPointCoord.first ? startPointCoord.first:endPointCoord.first;
			xMax = startPointCoord.first>endPointCoord.first ? startPointCoord.first:endPointCoord.first;
			y1Min = startPointCoord.second<endPointCoord.second ? startPointCoord.second:endPointCoord.second;
			y1Max = startPointCoord.second>endPointCoord.second ? startPointCoord.second:endPointCoord.second;

			xFlag = true;
		}
		
		double y2Min=0, y2Max=0;
		if( _isY2CoordRangeCalc )
		{
			// calculate the zoom rect
			POINT_DOUBLE startPointCoord, endPointCoord;
			TransfromPosToCoord( _lPointBeginPos, startPointCoord, 2 );
			TransfromPosToCoord( _lPointEndPos, endPointCoord, 2 );

			if( !xFlag )
			{
				xMin = startPointCoord.first<endPointCoord.first ? startPointCoord.first:endPointCoord.first;
				xMax = startPointCoord.first>endPointCoord.first ? startPointCoord.first:endPointCoord.first;
			}
			y2Min = startPointCoord.second<endPointCoord.second ? startPointCoord.second:endPointCoord.second;
			y2Max = startPointCoord.second>endPointCoord.second ? startPointCoord.second:endPointCoord.second;
		}

		SetXYCoordRange( xMin, xMax, y1Min, y1Max, y2Min, y2Max );
	}

	return true;
}

bool UIChart::OnRButtonDown( POINT pt )
{
	// 获取相对位置
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	_rPointPosUndo = point;

	_isMove = true;
	_rPointPosMove = point;

	return true;
}

bool UIChart::OnRButtonUp( POINT pt )
{
	// 获取相对位置
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	if( _rPointPosUndo.x==point.x && _rPointPosUndo.y==point.y )
	{
		if( _xCoordHistory.size()>0 )
		{
			_xCoordRange = _xCoordHistory.back();
			_y1CoordRange = _y1CoordHistory.back();
			_y2CoordRange = _y2CoordHistory.back();
			_xCoordHistory.pop_back();
			_y1CoordHistory.pop_back();
			_y2CoordHistory.pop_back();

			CalcCurveListDrawRange( SHRINK );
			UIRefresh();
		}
	}

	_isMove = false;
	return true;
}

bool UIChart::OnMouseMove( POINT pt )
{
	// 获取相对位置
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	if( IsKeyDown()( VK_LBUTTON ) )
	{	// 画zoom框
		if( IsPosPointInbox( point, _gridRect ) )
		{
			_isDrawZoomRect = true;
			_lPointEndPos = point;
		}
	}
	else if( IsKeyDown()( VK_RBUTTON ) )
	{	// 移动曲线
		if( _isMove )
		{
			if( IsPosPointInbox( point, _gridRect ) )
			{
				if( _rPointPosMove.x!=point.x || _rPointPosMove.y!=point.y )
				{
					double dxPos = point.x-_rPointPosMove.x;
					double dyPos = point.y-_rPointPosMove.y;

					double dxCoord = dxPos*_coordToPosScaleX;
					double dy1Coord = dyPos*_coordToPosScaleY1;
					double dy2Coord = dyPos*_coordToPosScaleY2;

					// new coord range
					_xCoordRange.first -= dxCoord;
					_xCoordRange.second -= dxCoord;
					_y1CoordRange.first += dy1Coord;
					_y1CoordRange.second += dy1Coord;
					_y2CoordRange.first += dy2Coord;
					_y2CoordRange.second += dy2Coord;

					if( dxPos>0 )
						CalcCurveListDrawRange( RIGHTMOVE );
					else
						CalcCurveListDrawRange( LEFTMOVE );

					_rPointPosMove = point;
					UIRefresh();
				}
			}
		}
	}
	else 
	{	// 显示曲线上的点
		_isShowToolTip = false;

		//
		CURVE_LIST::iterator it;
		bool breakFlag = false;
		int yFlag = 1;

		// 寻找选中的曲线
		for( it=_curveList1.begin(); it!=_curveList1.end(); ++it )
		{
			if( it->_isSelect==true ) 
			{
				breakFlag=true;
				yFlag = 1;
				break;
			}
		}
		if( !breakFlag )
		{
			for( it=_curveList2.begin(); it!=_curveList2.end(); ++it )
			{
				if( it->_isSelect==true ) 
				{
					breakFlag=true;
					yFlag = 2;
					break;
				}
			}
		}
	
		// 选中的曲线上判定是否显示信息
		if( breakFlag )
		{
			// 检查判定
			if( it->_isShow && it->_pointList.size()>0 && it->_beginPointNO>=0 ) 
			{
				if( IsPointNearCurvePoint( point, *it, yFlag ) )
					_isShowToolTip = true;
			}
		}
	}	

	// 显示鼠标位置
	POINT_DOUBLE coordPoint1 = make_pair( 0.f, 0.f );
	POINT_DOUBLE coordPoint2 = make_pair( 0.f, 0.f );
	TransfromPosToCoord( point, coordPoint1, 1 );
	TransfromPosToCoord( point, coordPoint2, 2 );
	STRING_FORMAT( _mouseCoordStr, "%0.2f, %0.2f, %0.2f", coordPoint1.first, coordPoint1.second, coordPoint2.second );

	UIRefresh();
	return true;
}

void UIChart::OnUpdate( UINT flag, void* data )
{
	if( flag==0 )
	{
		OnCalcXYCoordRange( data );
	}
	else if( flag==1 )
	{
		OnAddCurve1( data );
	}
	else if( flag==2 )
	{
		OnAddCurve2( data );
	}
	else if( flag==3 )
	{
		OnAddCurve3( data );
	}
	else if( flag==4 )
	{
		OnSetCurveColor( data );
	}
	else if( flag==5 )
	{
		OnClear();
	}
	else if( flag==6 )
	{
		OnSetCoordSymmetry( data );
	}
	else if( flag==7 )
	{
		OnSetCurveSelect( data );
	}
}

bool UIChart::OnCalcXYCoordRange( void* data )
{
	VECTOR_DOUBLE xMinList, xMaxList;
	VECTOR_DOUBLE y1MinList, y1MaxList;
	VECTOR_DOUBLE y2MinList, y2MaxList;

	double xMin, xMax, yMin, yMax;
	for( auto itor=_curveList1.begin(); itor!=_curveList1.end(); ++itor )
	{
		if( !itor->CalcCoordRange( xMin, xMax, yMin, yMax ) ) continue;

		xMinList.push_back( xMin );
		xMaxList.push_back( xMax );
		y1MinList.push_back( yMin );
		y1MaxList.push_back( yMax );
	}

	for( auto itor=_curveList2.begin(); itor!=_curveList2.end(); ++itor )
	{
		if( !itor->CalcCoordRange( xMin, xMax, yMin, yMax ) ) continue;

		xMinList.push_back( xMin );
		xMaxList.push_back( xMax );
		y2MinList.push_back( yMin );
		y2MaxList.push_back( yMax );
	}

	if( xMinList.size()>0 )
	{
		_xCoordRange.first = *min_element( xMinList.begin(), xMinList.end() );
		_xCoordRange.second = *max_element( xMaxList.begin(), xMaxList.end() );
	}

	if( y1MinList.size()>0 )
	{
		_isY1CoordRangeCalc = true;	
		_y1CoordRange.first = *min_element( y1MinList.begin(), y1MinList.end() );
		_y1CoordRange.second = *max_element( y1MaxList.begin(), y1MaxList.end() );
	}

	if( y2MinList.size()>0 )
	{
		_isY2CoordRangeCalc = true;	
		_y2CoordRange.first = *min_element( y2MinList.begin(), y2MinList.end() );
		_y2CoordRange.second = *max_element( y2MaxList.begin(), y2MaxList.end() );
	}

	// 外部设置条件过滤
	if( _isXCoordLimit )
	{
		if( _xCoordRange.first<_xCoordMinLimit ) 
			_xCoordRange.first=_xCoordMinLimit;
		if( _xCoordRange.second>_xCoordMaxLimit ) 
			_xCoordRange.second=_xCoordMaxLimit;
	}
	if( _isY1CoordLimit )
	{
		if( _y1CoordRange.first<_y1CoordMinLimit ) 
			_y1CoordRange.first=_y1CoordMinLimit;
		if( _y1CoordRange.second>_y1CoordMaxLimit )
			_y1CoordRange.second=_y1CoordMaxLimit;
	}
	if( _isY2CoordLimit )
	{
		if( _y2CoordRange.first<_y2CoordMinLimit ) 
			_y2CoordRange.first=_y2CoordMinLimit;
		if( _y2CoordRange.second>_y2CoordMaxLimit ) 
			_y2CoordRange.second=_y2CoordMaxLimit;
	}
	if( _isXCoordSymmetry )
	{
		CalcCoordSymmetry( _xCoordRange );
	}
	if( _isY1CoordSymmetry )
	{
		CalcCoordSymmetry( _y1CoordRange );
	}
	if( _isY2CoordSymmetry )
	{
		CalcCoordSymmetry( _y2CoordRange );
	}

	CalcCurveListDrawRange( NOTUSELASTDATA );

	return ( (int)data==1 ) ? true:false;
}

void UIChart::OnAddCurve1( void* data )
{
	Cache1* pCache = (Cache1*)data;

	CURVE_LIST& curveList = pCache->_yFlag==1 ? _curveList1:_curveList2;

	CURVE_LIST::iterator itor = find( curveList.begin(), curveList.end(), pCache->_curveName );
	if( itor==curveList.end() )
	{	// 不存在则创建一个新curve
		CurveInfo curve;
		curve._name = pCache->_curveName;
		curveList.push_back( curve );
	}

	delete pCache;
}

void UIChart::OnAddCurve2( void* data )
{
	Cache2* pCache = (Cache2*)data;
	double& x = pCache->_x;
	double& y = pCache->_y;

	CURVE_LIST& curveList = pCache->_yFlag==1 ? _curveList1:_curveList2;

	CURVE_LIST::iterator itor = find( curveList.begin(), curveList.end(), pCache->_curveName );
	if( itor!=curveList.end() )
	{	
		CurveInfo& curve = *itor;

		// 加入点对线条x轴有序影响
		if( curve._pointList.size()>0 && curve._isXCoordInOrder )
		{
			if( x<curve._pointList.back().first )
				curve._isXCoordInOrder = false;
		}

		// 插入点
		curve._pointList.push_back( make_pair( x, y ) );

		// 检查绘制范围是否计算
		if( ( pCache->_yFlag==1&&_isY1CoordRangeCalc ) || ( pCache->_yFlag==2&&_isY2CoordRangeCalc ) )
			CalcCurveDrawRange( curve, curve._beginPointNO>0?NOTUSELASTDATA:ADDPOINT );
	}

	delete pCache;
}

void UIChart::OnAddCurve3( void* data )
{
	Cache3* pCache = (Cache3*)data;
	VECTOR_DOUBLE& xList = pCache->_xList;
	VECTOR_DOUBLE& yList = pCache->_yList;

	CURVE_LIST& curveList = pCache->_yFlag==1 ? _curveList1:_curveList2;

	CURVE_LIST::iterator itor = find( curveList.begin(), curveList.end(), pCache->_curveName );
	if( itor!=curveList.end() )
	{
		curveList.erase( itor );
	}
	else
	{
		CurveInfo curve;
		curve._isXCoordInOrder = pCache->_isXCoordInOrder;
		curve._name = pCache->_curveName;
		for( UINT i=0; i<xList.size(); ++i )
			curve._pointList.push_back( make_pair( xList[i], yList[i] ) );
		curveList.push_back( curve );

		CalcCurveDrawRange( curve, NOTUSELASTDATA );
	}

	delete pCache;
}

void UIChart::OnSetCurveColor( void* data )
{
	Cache4 *pCache = (Cache4*)data;

	CURVE_LIST& curveList = pCache->_yFlag==1 ? _curveList1:_curveList2;

	CURVE_LIST::iterator itor = find( curveList.begin(), curveList.end(), pCache->_curveName );
	if( itor!=curveList.end() )
	{	
		CurveInfo& curve = *itor;
		curve._color = pCache->_color;
	}

	delete pCache;
}

void UIChart::OnClear()
{
	_isY1CoordRangeCalc = false;
	_curveList1.clear();
	_curveList2.clear();

	_xCoordHistory.clear();
	_y1CoordHistory.clear();
	_y2CoordHistory.clear();
}

void UIChart::OnSetCoordSymmetry( void* data )
{
	Cache5 *pCache = (Cache5*)data;

	if( pCache->_flag==1 )
	{
		_xCoordMinLimit = pCache->_min;
		_xCoordMaxLimit = pCache->_max;
		_isXCoordLimit = true;
	}
	else if( pCache->_flag==2 )
	{
		_y1CoordMinLimit = pCache->_min;
		_y1CoordMaxLimit = pCache->_max;
		_isY1CoordLimit = true;
	}
	else if( pCache->_flag==3 )
	{
		_y2CoordMinLimit = pCache->_min;
		_y2CoordMaxLimit = pCache->_max;
		_isY2CoordLimit = true;
	}
	else if( pCache->_flag==4 )
	{
		_isXCoordSymmetry = true;
	}
	else if( pCache->_flag==5 )
	{
		_isY1CoordSymmetry = true;
	}
	else if( pCache->_flag==6 )
	{
		_isY2CoordSymmetry = true;
	}

	delete pCache;
}

void UIChart::OnSetCurveSelect( void* data )
{
	Cache1 *pCache = (Cache1*)data;

	CURVE_LIST& curveList = pCache->_yFlag==1 ? _curveList1:_curveList2;
	CURVE_LIST::iterator itor = find( curveList.begin(), curveList.end(), pCache->_curveName );
	if( itor!=curveList.end() )
		itor->_isSelect = true;

	delete pCache;
}





UITab::CellData::CellData()
{
	_pWin = NULL;
}

UITab::UITab()
{
	_isShowTab = true;
	_isDraw = true;

	_selIndex = -1;
	_hoverIndex = -1;
	_xyFlag = X_FLAG;

	_isShowArrow = false;
	_isHoverArrowRC1 = false;
	_isHoverArrowRC2 = false;

	_isUpTransmissionMsg = true;
}

void UITab::CalcArea()
{
	// cell数检查
	if( _cellRCList.size()==0 ) 
	{
		_isDraw =false;
		return;
	}

	// 区域大小检查
	if( GetWidth()( _clientRC )<( TAB_1+TAB_2 ) || GetHeight()( _clientRC )<( TAB_1+TAB_2 ) ) 
	{
		_isDraw =false;
		return;
	}
	
	// 检查通过
	_isDraw = true;

	CalcTabRect();
	CalcTabSelLine();
	CalcCellListRect();

	CalcArrowRect();
}

void UITab::CalcCellListRect()
{
	_cellRC = _clientRC;

	if( _isShowTab )
	{
		if( _xyFlag==X_FLAG )
			_cellRC.top += TAB_1;
		else
			//_cellRC.left += TAB_1;
			_cellRC.right -= TAB_1;
	}

	if( _xyFlag==X_FLAG )
	{
		int left = _cellRCList[_selIndex].left;
		_cellRCList[_selIndex] = _cellRC;
		OffsetRect( &_cellRCList[_selIndex], left, 0 );

		int cellWidth = GetWidth()( _cellRC );
		for( UINT i=0; i<_cellRCList.size(); ++i )
		{
			if( i==_selIndex ) continue;

			int dx = ( i-_selIndex )*cellWidth;
			_cellRCList[i] = _cellRC;
			OffsetRect( &_cellRCList[i], dx+left, 0 );
		}
	}
	else
	{
		int top = _cellRCList[_selIndex].top;
		_cellRCList[_selIndex] = _cellRC;
		OffsetRect( &_cellRCList[_selIndex], 0, top );

		int cellHeight = GetHeight()( _cellRC );
		for( UINT i=0; i<_cellRCList.size(); ++i )
		{
			if( i==_selIndex ) continue;

			int dx = ( i-_selIndex )*cellHeight;
			_cellRCList[i] = _cellRC;
			OffsetRect( &_cellRCList[i], 0, dx+top );
		}
	}
}

void UITab::CalcTabRect()
{
	if( !_isShowTab )
		return;

	_tabRC = _clientRC;

	if( _xyFlag==X_FLAG )
	{
		_tabRC.bottom = TAB_1;
		
		for( UINT i=0; i<_tabRCList.size(); ++i )
		{
			_tabRCList[i] = _tabRC;
			_tabRCList[i].left = GetWidth()( _tabRC )*i/_tabRCList.size();
			_tabRCList[i].right = GetWidth()( _tabRC )*(i+1)/_tabRCList.size();
		}
	}
	else
	{
		//_tabRC.right = TAB_1;
		_tabRC.left = _tabRC.right-TAB_1;
		
		for( UINT i=0; i<_tabRCList.size(); ++i )
		{
			_tabRCList[i] = _tabRC;
			_tabRCList[i].top = GetHeight()( _tabRC )*i/_tabRCList.size();
			_tabRCList[i].bottom = GetHeight()( _tabRC )*(i+1)/_tabRCList.size();
		}
	}
}

void UITab::CalcTabSelLine()
{
	if( !_isShowTab )
		return;

	if( _xyFlag==X_FLAG )
	{
		int index = _selIndex!=-1 ? _selIndex:0;
		_lineRC = _tabRCList[index];
		_lineRC.top = _lineRC.bottom;
		_lineRC.top -= TAB_2;
	}
	else
	{
		int index = _selIndex!=-1 ? _selIndex:0;
		_lineRC = _tabRCList[index];
		/*_lineRC.left = _lineRC.right;
		_lineRC.left -= TAB_2;*/
		_lineRC.right = _lineRC.left+TAB_2;
	}
}

void UITab::CalcArrowRect()
{
	if( !_isShowArrow )
		return;

	_arrowRC1 = _clientRC;
	_arrowRC2 = _clientRC;

	if( _xyFlag==X_FLAG )
	{
		_arrowRC1.right = 80;
		_arrowRC2.left = _arrowRC2.right-80;
	}
	else
	{
		_arrowRC1.bottom = 80;
		_arrowRC2.top = _arrowRC2.bottom-80;
	}
}

void UITab::Draw()
{
	JudgeIsCalcArea();

	if( !_isDraw ) 
		return;

	// 所有子窗口全部置不接受消息
	for( UINT i=0; i<_cellList.size(); ++i )
	{
		if( _cellList[i]._pWin!=NULL )
			_cellList[i]._pWin->ShowWindowDirect( false );
	}

	DrawTab();

	if( !IsRunAnimate() )
	{
		DrawTabSelLine();
		DrawArrow();

		if( _selIndex!=-1 )
		{	
			CellData& cell = _cellList[_selIndex];
			if( cell._pWin!=NULL )
			{
				cell._pWin->MoveWindowDirect( _cellRC );
				cell._pWin->ShowWindowDirect( true );
				cell._pWin->Draw();
			}
		}
	}
	else
	{
		if( _animateMode==1 )
			DrawAnimate1();
	}
}

void UITab::DrawTab()
{
	if( !_isShowTab ) 
		return;
	
	for( UINT i=0; i<_tabRCList.size(); ++i )
	{	
		RECT rc = _tabRCList[i];
		OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );

		if( _hoverIndex==i )
			UI2DRect( rc, _z )( D3DCOLOR_XRGB(203, 196, 231), 255 );
		else
			UI2DRect( rc, _z )( _FRAMEPREPLE_, 255 );

		UI2DFont( _z, 18 ).operator()( _cellList[i]._title, rc );
	}
}

void UITab::DrawTabSelLine()
{
	if( !_isShowTab || _selIndex==-1 ) 
		return;

	RECT rc = _lineRC;
	OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );
	UI2DRect( rc, _z )( _FRAMEGREEN_, 255 );
}

void UITab::DrawArrow()
{
	if( !_isShowArrow )
		return;

	if( _cellList.size()<=1 )
		return;

	if( _selIndex>0 )
	{
		int dstCenterX = _arrowRC1.left + GetWidth()( _arrowRC1 )/2;
		int dstCenterY = _arrowRC1.top + GetHeight()( _arrowRC1 )/2;

		if( _xyFlag==X_FLAG )
			UI2DImage( "GUIResource.dll", IDB_ARROW2_LEFT, _z )( dstCenterX, dstCenterY, 1, _isHoverArrowRC1?255:80 );
		else
			UI2DImage( "GUIResource.dll", IDB_ARROW2_UP, _z )( dstCenterX, dstCenterY-10, 1, _isHoverArrowRC1?255:80 );
	}

	if( _selIndex<(int)_cellRCList.size()-1 )
	{
		int dstCenterX = _arrowRC2.left + GetWidth()( _arrowRC2 )/2;
		int dstCenterY = _arrowRC2.top + GetHeight()( _arrowRC2 )/2;

		if( _xyFlag==X_FLAG )
			UI2DImage( "GUIResource.dll", IDB_ARROW2_RIGHT, _z )( dstCenterX, dstCenterY, 1, _isHoverArrowRC2?255:80 );
		else
			UI2DImage( "GUIResource.dll", IDB_ARROW2_DOWN, _z )( dstCenterX, dstCenterY-15, 1, _isHoverArrowRC2?255:80 );
	}
}

void UITab::DrawAnimate1()
{
	DrawAnimate1Cell();
	DrawAnimate1Tab();

	DrawArrow();
}

void UITab::DrawAnimate1Cell()
{
	// 计算 
	if( _xyFlag==X_FLAG )
	{
		int dx = _cellRCList[_selIndex].left/( MAX_FRAME1-_frameCount+1 );

		for( UINT i=0; i<_cellRCList.size(); ++i )
			OffsetRect( &_cellRCList[i], -dx, 0 ); 
	}
	else
	{
		int dx = _cellRCList[_selIndex].top/( MAX_FRAME1-_frameCount+1 );

		for( UINT i=0; i<_cellRCList.size(); ++i )
			OffsetRect( &_cellRCList[i], 0, -dx ); 
	}

	// 移动并绘制cell
	UI2DClipRect uiClip;
	RECT rc = _clientRC;
	OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );
	uiClip.BeginClipRect( rc );
	
	for( UINT i=0; i<_cellRCList.size(); ++i )
	{
		UIWinBase* _pWin = _cellList[i]._pWin;
		if( _pWin==NULL ) continue;

		if( _xyFlag==X_FLAG )
		{
			if( _cellRCList[i].right<_cellRC.left || _cellRCList[i].left>_cellRC.right )
				continue;
		}
		else
		{
			if( _cellRCList[i].bottom<_cellRC.top || _cellRCList[i].top>_cellRC.bottom )
				continue;
		}

		_pWin->MoveWindowDirect( _cellRCList[i] );
		_pWin->ShowWindowDirect( true );
		_pWin->Draw();
	}

	uiClip.EndClipRect();
}

void UITab::DrawAnimate1Tab()
{
	if( !_isShowTab ) return;

	if( _xyFlag==X_FLAG )
	{
		int dx = ( _tabRCList[_selIndex].left-_lineRC.left )/( MAX_FRAME1-_frameCount+1 );
		OffsetRect( &_lineRC, dx, 0 );
	}
	else
	{
		int dx = ( _tabRCList[_selIndex].top-_lineRC.top )/( MAX_FRAME1-_frameCount+1 );
		OffsetRect( &_lineRC, 0, dx );
	}
	
	RECT rc = _lineRC;
	OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );
	UI2DRect( rc, _z )( _FRAMEGREEN_, 255 );
}

void UITab::SetCellNum( UINT num )
{
	UIPostMessage( this, WM_UPDATE, 1, num );
}

void UITab::SetCell( UINT index, string title, UIWinBase* pWin )
{
	Cache1* pCache = new Cache1;
	pCache->_index = index;
	pCache->_str = title;
	pCache->_pWin = pWin;
	UIPostMessage( this, WM_UPDATE, 2, (LPARAM)pCache );
}

void UITab::SetCurCell( UINT index )
{
	UIPostMessage( this, WM_UPDATE, 3, index );
}

void UITab::SetX()
{
	UIPostMessage( this, WM_UPDATE, 4, 0 );
}

void UITab::SetY()
{
	UIPostMessage( this, WM_UPDATE, 4, 1 );
}

void UITab::ShowTab()
{
	UIPostMessage( this, WM_UPDATE, 5, 1 );
}

void UITab::HideTab()
{
	UIPostMessage( this, WM_UPDATE, 5, 0 );
}

void UITab::ShowArrow()
{
	UIPostMessage( this, WM_UPDATE, 6, 1 );
}

bool UITab::OnMouseMove( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	if( _isShowArrow )
	{
		_isHoverArrowRC1 = false;
		_isHoverArrowRC2 = false;

		if( IsPointInRect()( point, _arrowRC1 ) )
		{
			_isHoverArrowRC1 = true;
			UIRefresh();
		}
		else if( IsPointInRect()( point, _arrowRC2 ) )
		{
			_isHoverArrowRC2 = true;
			UIRefresh();
		}
	}

	if( _isShowTab )
	{
		if( !IsPointInRect()( point, _tabRC ) )
		{
			if( _hoverIndex!=-1 )
			{
				_hoverIndex = -1;
				UIRefresh();
			}
		}
		else
		{
			for( UINT i=0; i<_tabRCList.size(); ++i )
			{
				if( IsPointInRect()( point, _tabRCList[i] ) )
				{
					if( _hoverIndex!=i )
					{
						_hoverIndex = i;
						UIRefresh();
					}
					break;
				}
			}
		}
	}

	return true;
}

void UITab::OnMouseLeave( POINT pt )
{
	if( _isShowArrow )
	{
		_isHoverArrowRC1 = false;
		_isHoverArrowRC2 = false;
	}

	if( _isShowTab )
	{
		if( _hoverIndex!=-1 )
		{
			_hoverIndex = -1;
			UIRefresh();
		}
	}
}

bool UITab::OnLButtonDown( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	if( _isShowArrow )
	{
		if( IsPointInRect()( point, _arrowRC1 ) && _selIndex>0 )
		{
			SetCurCell( _selIndex-1 );
			return true;
		}

		if( IsPointInRect()( point, _arrowRC2 ) && _selIndex<(int)_cellRCList.size()-1 )
		{
			SetCurCell( _selIndex+1 );
			return true;
		}
	}

	if( _isShowTab && IsPointInRect()( point, _tabRC ) )
	{
		for( UINT i=0; i<_tabRCList.size(); ++i )
		{
			if( IsPointInRect()( point, _tabRCList[i] ) )
			{
				if( _selIndex==i ) return true;

				SetCurCell( i );
				return true;
			}
		}
	}

	return true;
}

void UITab::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		UINT num = (UINT)data;
		_tabRCList.resize( num );
		_cellList.resize( num );
		_cellRCList.resize( num );
		_selIndex = 0;

		_isCalcArea = true;
	}
	else if( flag==2 )
	{
		Cache1* pCache = (Cache1*)data;

		if( pCache->_index<_cellList.size() )
		{
			_cellList[pCache->_index]._title = pCache->_str;
			_cellList[pCache->_index]._pWin = pCache->_pWin;
		}

		delete pCache;
	}
	else if( flag==3 )
	{
		UINT index = (UINT)data;
		if( index<_cellList.size() )
		{
			if( _selIndex==index ) return;

			_selIndex = index;
			PlayAnimate( 1, MAX_FRAME1 );
		}

		return;
	}
	else if( flag==4 )
	{
		int flag = (int)data;
		_xyFlag = flag==0 ? X_FLAG:Y_FLAG;

		_isCalcArea = true;
	}
	else if( flag==5 )
	{
		int flag = (int)data;
		_isShowTab = flag==0 ? false:true;

		_isCalcArea = true;
	}
	else if( flag==6 )
	{
		int flag = (int)data;
		_isShowArrow = flag==0 ? false:true;

		_isCalcArea = true;
	}
}



UICanvas::UICanvas()
{
	_isUpTransmissionMsg = true;
}



UILayoutGrid::CellInfo::CellInfo()
{
	_pWin = NULL;
	_endRow = -1;
	_endColumn = -1;
}

void UILayoutGrid::InitPoint( POINT& relativePT )
{
	_relativePT = relativePT;
}

// 设置cell信息
void UILayoutGrid::SetRowColumn( UINT row, UINT column, int width, int widthInterval, int height, int heightInterval )
{
	if( row==0 ) return;

	// 设置grid大小
	_grid.resize( row );
	for( UINT r=0; r<_grid.size(); ++r )
		_grid[r].resize( column );

	// 初始化cell长宽及间隔
	_columnWidthlList.resize( column, width );
	_columnIntervalList.resize( column, widthInterval );
	//
	_rowHeightList.resize( row, height );
	_rowIntervalList.resize( row, heightInterval );
}

void UILayoutGrid::SetCell( UINT bRow, UINT bColumn, UINT eRow, UINT eColumn, UIWinBase* pWin )//
{
	if( _grid.size()==0 ) return;
	if( eRow>=_grid.size() || eColumn>=_grid[0].size() ) return;
	if( bRow>eRow || bColumn>eColumn ) return;

	_grid[bRow][bColumn]._pWin = pWin;
	_grid[bRow][bColumn]._endRow = eRow;
	_grid[bRow][bColumn]._endColumn = eColumn;
}

void UILayoutGrid::SetCell( UINT row, UINT column, UIWinBase* pWin )
{
	SetCell( row, column, row, column, pWin );
}

// 设置长宽及间隔
void UILayoutGrid::SetColumnWidthInterval( UINT column, int width, int interval )
{
	if( column>=_columnIntervalList.size() ) return;

	_columnWidthlList[column] = width;
	_columnIntervalList[column] = interval;
}

void UILayoutGrid::SetRowHeightInterval( UINT row, int heigth, int interval )
{
	if( row>=_rowIntervalList.size() ) return;

	_rowHeightList[row] = heigth;
	_rowIntervalList[row] = interval;
}

// 计算位置信息
void UILayoutGrid::CalcGridPos()
{
	if( _grid.size()==0 ) return;

	LONG& x_ = _relativePT.x;
	LONG& y_ = _relativePT.y;

	for( UINT r=0; r<_grid.size(); ++r )
	{
		for( UINT c=0; c<_grid[0].size(); ++c )
		{
			if( _grid[r][c]._pWin==NULL ) continue;

			int sum1 = std::accumulate( _columnWidthlList.begin(), _columnWidthlList.begin()+c, 0 );
			sum1 += std::accumulate( _columnIntervalList.begin(), _columnIntervalList.begin()+c, 0 );

			int sum2 = std::accumulate( _rowHeightList.begin(), _rowHeightList.begin()+r, 0 );
			sum2 += std::accumulate( _rowIntervalList.begin(), _rowIntervalList.begin()+r, 0 );

			int sum3 = std::accumulate( _columnWidthlList.begin()+c, _columnWidthlList.begin()+_grid[r][c]._endColumn+1, 0 );
			sum3 += std::accumulate( _columnIntervalList.begin()+c, _columnIntervalList.begin()+_grid[r][c]._endColumn, 0 );

			int sum4 = std::accumulate( _rowHeightList.begin()+r, _rowHeightList.begin()+_grid[r][c]._endRow+1, 0 );
			sum4 += std::accumulate( _rowIntervalList.begin()+r, _rowIntervalList.begin()+_grid[r][c]._endRow, 0 );

			_grid[r][c]._pWin->MoveWindowDirect( GenRect()( GenPoint()( x_+sum1, y_+sum2 ), GenSize()( sum3, sum4 ) ) );
		}
	}
}