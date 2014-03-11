
#include "stdafx.h"
#include "UIPlugIN.h"

using namespace std;


bool PlugINOBJ::Load( std::string path )
{
	_dllPath = path;

	_handle = ::LoadLibraryA( path.c_str() );
	CheckNULL( _handle )

	PFunc_PlugIN_Infor PlugINInfor = ( PFunc_PlugIN_Infor )GetProcAddress( _handle, "PlugINInfor" );
	CheckNULL( PlugINInfor )

	PFunc_PlugIN_Init PlugINInit = ( PFunc_PlugIN_Init )GetProcAddress( _handle, "PlugINInit" );
	CheckNULL( PlugINInit )

	PlugINCreate = ( PFunc_PlugIN_Create )GetProcAddress( _handle, "PlugINCreate" );
	CheckNULL( PlugINCreate )

	PlugINSetShow = ( PFunc_PlugIN_SetShow )GetProcAddress( _handle, "PlugINSetShow" );
	CheckNULL( PlugINSetShow )

	PlugINDraw = ( PFunc_PlugIN_Draw )GetProcAddress( _handle, "PlugINDraw" );
	CheckNULL( PlugINDraw )

	PFunc_PlugIN_TileTitle PlugINTileTitle = ( PFunc_PlugIN_TileTitle )GetProcAddress( _handle, "PlugINTileTitle" );
	CheckNULL( PlugINTileTitle )
		
	PFunc_PlugIN_TileImageID PlugINTileImageID = ( PFunc_PlugIN_TileImageID )GetProcAddress( _handle, "PlugINTileImageID" );
	CheckNULL( PlugINTileImageID )

	PFunc_PlugIN_MenuCount PlugINMenuCount = ( PFunc_PlugIN_MenuCount )GetProcAddress( _handle, "PlugINMenuCount" );
	CheckNULL( PlugINMenuCount )

	PFunc_PlugIN_MenuCell PlugINMenuCell = ( PFunc_PlugIN_MenuCell )GetProcAddress( _handle, "PlugINMenuCell" );
	CheckNULL( PlugINMenuCell )

	PlugINMenuNotify = ( PFunc_PlugIN_MenuNotify )GetProcAddress( _handle, "PlugINMenuNotify" );
	CheckNULL( PlugINMenuNotify )


	// 初始化
	PlugINInit( UIDXInit::GetSingleton(), UIAnimateManage::GetSingleton(), UIMessageLoopManage::GetSingleton(), 
				UICamera::GetSingleton(), UIRender::GetSingleton(), UIWinShell::_pWinShell );

	// 获得插件信息 作者 日期等
	_infor = PlugINInfor();

	// 获得tile信息
	_tileTitle = PlugINTileTitle();
	_tileImageID = PlugINTileImageID();

	// 获得menu
	for( int i=0; i<PlugINMenuCount(); ++i )
		_menu.push_back( PlugINMenuCell( i+1 ) );
	_menuSelIndex = -1;

	return true;
}


UIMainBar::MenuCell::MenuCell( std::string str ) 
{ 
	_str=str; 
	_isHover=false; 
	_isSelect=false; 
}

UIMainBar::UIMainBar()
{
	_dx = 18;
	_isHover = false;

	_msgTip = "show message..";
}

void UIMainBar::Draw()
{
	// 获取相对位置
	LONG& x_ = _abusolutePoint.x;
	LONG& y_ = _abusolutePoint.y;

	// 画主背景
	RECT rc = _clientRC;
	OffsetRect( &rc, x_, y_ );
	DrawFrameHelp( "GUIResource.dll", IDR_MAIN_BAR, 2, 2, 8, 2, _z, D3DCOLOR_XRGB(255, 0, 255) )( rc, 200 );
	
	// 画hot效果
	if( _isHover==true )
	{
		RECT rc = _hoverRC;
		OffsetRect( &rc, x_ ,y_ );
		DrawFrameHelp( "GUIResource.dll", IDB_HOT_EFFECT, 2, 2, 2, 2, _z, D3DCOLOR_XRGB(255, 0, 255) )( rc, 150 );
	}

	// 画home按钮
	UI2DImage( "GUIResource.dll", IDB_HOME_MENU, _z )( _dx+18+x_, 2+18+y_ );

	// 画menu
	UI2DFont fontHelp( _z, 24, 400 );
	for( UINT i=0; i<_menu.size(); ++i )
	{
		MenuCell& cell = _menu[i];
		rc = cell._rc;
		OffsetRect( &rc, x_, y_ );

		if( cell._isHover && IsKeyDown()( VK_LBUTTON ) )
			OffsetRect( &rc, 1, 1 );

		fontHelp( cell._str, rc, UI2DFont::FONT_CENTER, cell._isSelect?D3DCOLOR_XRGB(65, 48, 148) :_GOLD_ );
	}

	// 画msg
	if( _msgTip!="" )
	{
		rc = _msgTipRC;
		OffsetRect( &rc, x_, y_ );
		fontHelp( "  "+_msgTip, rc, UI2DFont::FONT_LEFT, _GOLD_ );
	}
}

void UIMainBar::CalcArea()
{
	_homeButtonRC = _clientRC;
	_homeButtonRC.right = 36+2*_dx;

	_msgTipRC = _clientRC;
	_msgTipRC.left = _msgTipRC.right - (int)( 0.55*GetWidth()( _clientRC ) );

	_menuRC = _clientRC;
	_menuRC.left = _homeButtonRC.right;
	_menuRC.right = _msgTipRC.left;
	//
	CalcMenu();
}

void UIMainBar::CalcMenu()
{
	if( _menu.size()==0 )
		return;

	// 计算单个cell的宽度
	int width = GetWidth()( _homeButtonRC );
	int dx = GetWidth()( _menuRC )/_menu.size();
	if( dx>width+10 )
		dx = width+10;

	MenuCell& cell = _menu[0];
	cell._rc = _menuRC;
	cell._rc.right = cell._rc.left+dx;
	//
	for( UINT i=1; i<_menu.size(); ++i )
	{
		MenuCell& cell = _menu[i];
		cell._rc = _menu[i-1]._rc;
		cell._rc.left = _menu[i-1]._rc.right;
		cell._rc.right = cell._rc.left+dx;
	}
}

void UIMainBar::AttachMenu( VECTOR_STRING& strList, int selectIndex )
{
	Cache1* pCache = new Cache1;
	pCache->_strListCache = strList;
	pCache->_selectIndex = selectIndex;

	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pCache );
}

void UIMainBar::ClearMenu()
{
	UIPostMessage( this, WM_UPDATE, 3, 0 );
}

void UIMainBar::SetShowTip( std::string str )
{
	Cache2* pCache = new Cache2;
	pCache->_msg = str;

	UIPostMessage( this, WM_UPDATE, 2, (LPARAM)pCache );
}

bool UIMainBar::OnMouseMove( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	// 初始化状态
	_isHover = false;
	for( UINT i=0; i<_menu.size(); ++i )
	{
		MenuCell& cell = _menu[i];
		cell._isHover = false;
	}
	// 判定点位置
	if( IsPointInRect()( point, _homeButtonRC )==true )
	{
		_isHover = true;
		_hoverRC = _homeButtonRC;
	}
	else if( IsPointInRect()( point, _msgTipRC )==true )
	{
		_isHover = true;
		_hoverRC = _msgTipRC;
	}
	else
	{
		for( UINT i=0; i<_menu.size(); ++i )
		{
			MenuCell& cell = _menu[i];
			RECT rc = cell._rc;
			
			if( IsPointInRect()( point, rc )==true )
			{
				_isHover = true;
				_hoverRC = rc;
				cell._isHover = true;
				break;
			}
		}
	}

	UIRefresh();
	return true;
}

void UIMainBar::OnMouseLeave( POINT pt )
{
	_isHover = false;
	for( UINT i=0; i<_menu.size(); ++i )
	{
		MenuCell& cell = _menu[i];
		cell._isHover = false;
	}

	UIRefresh();
}

bool UIMainBar::OnLButtonUp( POINT pt )
{
	POINT point = pt;
	point.x -= _abusolutePoint.x;
	point.y -= _abusolutePoint.y;

	if( IsPointInRect()( point, _homeButtonRC ) )
	{
		SendMessageToParent( WM_NOTIFY, (WPARAM)_id, 0 );
	}
	else
	{
		for( UINT i=0; i<_menu.size(); ++i )
		{
			MenuCell& cell = _menu[i];
			RECT rc = cell._rc;
			
			if( cell._isHover )
			{
				for( UINT j=0; j<_menu.size(); ++j ) _menu[j]._isSelect = false;
				cell._isSelect = true;

				SendMessageToParent( WM_NOTIFY, (WPARAM)_id, i+1 );
			}
		}
	}

	UIRefresh();
	return true;
}

void UIMainBar::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		Cache1* pCache = (Cache1*)data;
		VECTOR_STRING& _strListCache = pCache->_strListCache;
		int& selectIndex = pCache->_selectIndex;

		_menu.clear();
		for( UINT i=0; i<_strListCache.size(); ++i )
			_menu.push_back( MenuCell( _strListCache[i] ) );

		if( selectIndex>0 && selectIndex<=(int)_strListCache.size() )
			_menu[selectIndex-1]._isSelect = true;
		
		CalcMenu();
		delete pCache;
	}
	else if( flag==2 )
	{
		Cache2* pCache = (Cache2*)data;
		_msgTip = pCache->_msg;

		delete pCache;
	}
	else if( flag==3 )
	{
		_menu.clear();
	}

	UIRefresh();
}




UIMetro::UITile::UITile()
{
	_isShow = false;
	_isHover = false;
	_pPlugIN = NULL;
}

void UIMetro::UITile::Draw()
{
	if( _isShow==false ) 
		return;

	if( !IsRunAnimate() )
	{
		DrawBackGround();
		if( _isHover ) 
			DrawHotEffect( 255 );	
		DrawImage();
		DrawTitle();
	}
	else
	{
		if( _animateMode==SET_HOT || _animateMode==SET_COOL )
		{
			DrawBackGround();

			RECT rc = _clientRC;
			OffsetRect( &rc, _abusolutePoint.x, _abusolutePoint.y );

			if( _animateMode==UIAnimateEffect::SET_HOT )
				DrawHotAnimate( rc, _z );
			else
				DrawCoolAnimate( rc, _z );

			DrawImage();
			DrawTitle();
		}
		else
		{
			DrawBackGround();	
			DrawImage();
			DrawTitle();
		}
	}
}

void UIMetro::UITile::DrawNoHot()
{
	if( _isShow==false ) 
		return;

	DrawBackGround();
	DrawImage();
	DrawTitle();
}

void UIMetro::UITile::DrawBackGround() 
{ 
	RECT tempRC = _clientRC;
	OffsetRect( &tempRC, _abusolutePoint.x, _abusolutePoint.y );
	UI2DRect( tempRC, _z )( _FRAMEBLUE_, 255 ); 
}

void UIMetro::UITile::DrawHotEffect( UCHAR alphy ) 
{ 
	RECT tempRC = _clientRC;
	OffsetRect( &tempRC, _abusolutePoint.x, _abusolutePoint.y );
	DrawFrameHelp( "GUIResource.dll", IDB_HOT_EFFECT2, 2, 2, 2, 2, _z, D3DCOLOR_XRGB(255, 0, 255) )( tempRC, alphy ); 
}

void UIMetro::UITile::DrawImage()
{
	if( IsRunAnimate() && _animateMode==HIT_DRUM )
		DrawHitDrumAnimate( GetWidth()( _rc1 )/2+_abusolutePoint.x, GetHeight()( _rc1 )/2+_abusolutePoint.y, _z );
	else
		UI2DImage( _pPlugIN->_dllPath, _pPlugIN->_tileImageID, _z )( GetWidth()( _rc1 )/2+_abusolutePoint.x, GetHeight()( _rc1 )/2+_abusolutePoint.y );
}

void UIMetro::UITile::DrawTitle()
{
	if( _pPlugIN->_tileTitle!="" )
	{
		RECT tempRC = _rc2;
		OffsetRect( &tempRC, _abusolutePoint.x, _abusolutePoint.y );

		UI2DFont fontHelp( _z, 30, 400, _T("微软雅黑") );
		fontHelp( _pPlugIN->_tileTitle, tempRC );
	}
}

void UIMetro::UITile::CalcArea()
{
	_rc1 = _clientRC;
	_rc1.right = 64+40;

	_rc2 = _clientRC;
	_rc2.left = _rc1.right;
}

void UIMetro::UITile::BindPlugIN( PlugINOBJ* pPlugIN )
{
	UIPostMessage( this, WM_UPDATE, 1, (LPARAM)pPlugIN );
}

void UIMetro::UITile::UninitAnimate()
{
	if( _animateMode==HIT_DRUM )
	{	// 通告父窗口选中的tile
		SendMessageToParent( WM_NOTIFY, _id, 0 );
	}
}

void UIMetro::UITile::SetHoverDirect( bool flag )
{
	if( _isHover==flag )
		return;

	if( flag )
	{
		_isHover = true;
		PlayAnimate( UIAnimateEffect::SET_HOT );
	}
	else
	{
		_isHover = false;
		// 离开动画不能打断击鼓动画
		if( !( IsRunAnimate() && _animateMode==HIT_DRUM ) )
			PlayAnimate( UIAnimateEffect::SET_COOL );
	}
}

bool UIMetro::UITile::OnMouseMove( POINT pt )
{
	SetHoverDirect( true );
	return true;
}

void UIMetro::UITile::OnMouseLeave( POINT pt )
{
	SetHoverDirect( false );
}

bool UIMetro::UITile::OnLButtonDown( POINT pt )
{
	PlayAnimate( HIT_DRUM );
	return false;
}

void UIMetro::UITile::OnUpdate( UINT flag, void* data )
{
	if( flag==1 )
	{
		_pPlugIN = (PlugINOBJ*)data;
		SetImageIDAndDLL( _pPlugIN->_tileImageID, _pPlugIN->_dllPath );
		_isShow = true;
	}

	UIRefresh();
}

UIMetro::UIMetro()
{
}

void UIMetro::Draw()
{
	if( !IsRunAnimate() )			// 正常绘制
	{
		DrawCommon();
	}
	else							// 动画绘制
	{	
		if( _animateMode==START_BUBBLE )
			DrawStartBubble();
	}
}

void UIMetro::DrawCommon()
{
	for( int i=0; i<6; ++i )
	{
		_tileList[i].MoveWindowDirect( _tileRCList[i] );
		_tileList[i].Draw();
	}
}

void UIMetro::DrawStartBubble()
{
	float zoomScaleList[5] = { 0.7f, 0.8f, 0.9f, 1.0f, 1.05f };

	for( int i=0; i<6; ++i )
	{
		if( !_tileList[i].IsWindowShow() )
			return;

		int frame = _frameCount-i;
		if( frame<1 || frame>5 )
		{
			_tileList[i].MoveWindowDirect( _tileRCList[i] );
			_tileList[i].DrawNoHot();
		}
		else
		{
			RECT rc = CalcZoomRC( i+1, zoomScaleList[frame-1] );
			_tileList[i].MoveWindowDirect( rc );
			_tileList[i].DrawNoHot();
		}
		
		_tileList[i].DrawNoHot();
	}
}

/*
	1 2 5
	3 4 6
*/
void UIMetro::CalcArea()
{
	int width = GetWidth()( _clientRC );
	int height = GetHeight()( _clientRC );

	int edgeX = 60;
	int edgeY = 100;

	int midX = 15;
	int midY = 15;

	int dx = ( width-2*edgeX-2*midX )/3;
	int dy = ( height-2*edgeY-midY )/2;

	_tileRCList[0].left = edgeX;
	_tileRCList[0].right = edgeX+dx;
	_tileRCList[0].top = edgeY;
	_tileRCList[0].bottom = edgeY+dy;

	_tileRCList[1] = _tileRCList[0];
	_tileRCList[1].left = _tileRCList[0].right+midY;
	_tileRCList[1].right = _tileRCList[1].left+dx;

	_tileRCList[2] = _tileRCList[0];
	_tileRCList[2].top = _tileRCList[0].bottom+midY;
	_tileRCList[2].bottom = _tileRCList[2].top+dy;

	_tileRCList[3] = _tileRCList[1];
	_tileRCList[3].top = _tileRCList[1].bottom+midY;
	_tileRCList[3].bottom = _tileRCList[3].top+dy;

	_tileRCList[4] = _tileRCList[1];
	_tileRCList[4].left = _tileRCList[1].right+midY;
	_tileRCList[4].right = _tileRCList[4].left+dx;

	_tileRCList[5] = _tileRCList[4];
	_tileRCList[5].top = _tileRCList[4].bottom+midY;
	_tileRCList[5].bottom = _tileRCList[5].top+dy;
}

RECT UIMetro::CalcZoomRC( int index, float zoomScale )
{
	RECT rc = _tileRCList[index-1];

	int w = GetWidth()( rc );
	int h = GetHeight()( rc );

	int dw = static_cast<int>( ( zoomScale-1 )*w );
	int dh = static_cast<int>( ( zoomScale-1 )*h );

	rc.left -= dw/2;
	rc.right += dw/2;
	rc.top -= dh/2;
	rc.bottom += dh/2;

	return rc;
}

void UIMetro::SetTitle( int index, PlugINOBJ* pPlugIN )
{
	_tileList[index].BindPlugIN( pPlugIN );
}

void UIMetro::OnCreate()
{
	CalcArea();

	for( int i=0; i<6; ++i )
		_tileList[i].Create( i+1, &_uiContainer, _NULLRECT_, UILayoutCalc::NO_ZOOM, false );
}

bool UIMetro::OnMouseMove( POINT pt )
{
	for( int i=0; i<6; ++i )
		_tileList[i].SetHoverDirect( false );
	return false;
}

void UIMetro::OnNotify( int id, int param )
{
	SendMessageToParent( WM_NOTIFY, ID_METROSTART, (LPARAM)_tileList[id-1]._pPlugIN );
}



UIStartScreen::UIStartScreen()
{
	_pSelPlugIN = NULL;
}

int UIStartScreen::FindSelPlugINIndex()
{
	if( _pSelPlugIN==NULL )
		return -1;

	UINT index = 0;
	for( ; index<_plugINList.size(); ++index )
	{
		if( &_plugINList[index]==_pSelPlugIN )
			break;
	}
	return (int)index+1;
}

void UIStartScreen::Draw()
{
	// 画main bar
	_mainBar.Draw();

	if( _pSelPlugIN==NULL )
	{	
		UI2DFont fontHelp( _z, 42 );
		SIZE sz = fontHelp.GetDrawAreaSize( "Start" );
		RECT rc = GenRect()( 30, 20, 35+sz.cx, 25+sz.cy );
		fontHelp( "Start", rc, UI2DFont::FONT_CENTER, _FRAMEPREPLE_ );
		
		// 绘制tab对应的metro
		_tab.Draw();
	}
	else
	{	// 绘制插件
		_pSelPlugIN->PlugINDraw();
	}
}

void UIStartScreen::OnCreate()
{
	CalcArea();

	RECT rc = _clientRC;
	rc.top = rc.bottom-40;
	_mainBar.Create( ID_MAINBAR, &_uiContainer, rc, UILayoutCalc::SIZE_X|UILayoutCalc::MOVE_Y );

	rc = _clientRC;
	rc.bottom -= 40;
	_tab.Create( 0, &_uiContainer, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );	
	_tab.SetY();
	_tab.HideTab();
	_tab.ShowArrow();
}

void UIStartScreen::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )
	{
		if( param==0 )
		{	// 重新回到metro画面
			int index = FindSelPlugINIndex()-1;
			_pSelPlugIN = NULL;

			for( UINT i=0; i<_plugINList.size(); ++i )
				_plugINList[i].PlugINSetShow( false );

			_mainBar.ClearMenu();

			_tab.ShowWindowDirect( true );
			_tab.SetCurCell( index/6 );

			if( _metroList.size()>0 )
				_metroList[index/6].PlayAnimate( UIMetro::START_BUBBLE, 11 );
		}
		else
		{	// 菜单通告载入的插件
			if( _pSelPlugIN!=NULL )
			{
				_pSelPlugIN->PlugINMenuNotify( param );
				_pSelPlugIN->_menuSelIndex = param;
			}
		}
	}
	else if( id==ID_METROSTART )
	{
		_pSelPlugIN = ( PlugINOBJ* )param;

		// 判定是否第一次创建
		int index = FindSelPlugINIndex()-1;
		if( !_createFlagList[index] )
		{
			RECT rc = _clientRC;
			rc.bottom -= 40;

			_plugINList[index].PlugINCreate( &_uiContainer, &rc );
			_createFlagList[index] = true;
		}

		// 其它窗口全部置为不可见
		_tab.ShowWindowDirect( false );
		//
		for( UINT i=0; i<_plugINList.size(); ++i )
			_plugINList[i].PlugINSetShow( false );

		// 显示选中的窗口
		_pSelPlugIN->PlugINSetShow( true );

		// menu
		_mainBar.AttachMenu( _pSelPlugIN->_menu, _pSelPlugIN->_menuSelIndex );

		UIRefresh();
	}
}

void UIStartScreen::LoadPlugINs( std::string folder )
{
	VECTOR_STRING fileList;
	ReadFolderAllFiles2( "dll", folder, fileList );

	RECT rc = _clientRC;
	rc.bottom -= 40;

	// 读取并创建插件
	for( UINT i=0; i<fileList.size(); ++i )
	{
		PlugINOBJ plugIN;
		if( plugIN.Load( folder+"\\"+fileList[i] ) )
		{
			_plugINList.push_back( plugIN );
			_createFlagList.push_back( false );
		}
	}

	// 创建metro 并与tab关联
	UINT count = _plugINList.size()/6;
	if( _plugINList.size()%6!=0 )
		++count;
	//
	_metroList.resize( count );
	_tab.SetCellNum( count );
	for( UINT i=0; i<_metroList.size(); ++i )
	{
		_metroList[i].Create( ID_METROSTART+i, &_tab, rc, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
		string str;
		STRING_FORMAT( str, "%d", i+1 );
		_tab.SetCell( i, str.c_str(), &_metroList[i] );
	}

	// 设置metro内的tile与plugIN关联
	for( UINT i=0; i<_plugINList.size(); ++i )
	{
		_metroList[i/6].SetTitle( i%6, &_plugINList[i] );
	}

	// 绘制开始冒泡动画
	if( _metroList.size()>0 )
	{
		_tab.SetCurCell( 0 );
		_metroList[0].PlayAnimate( UIMetro::START_BUBBLE, 11 );
	}
}