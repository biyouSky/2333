#include "stdafx.h"
#include "UISoftHelp.h"
using namespace std;

UISoftHelp gUISoftHelp;



void UISoftHelp::OnCreate()
{
	_tabX0.Create( 0, &_uiContainer, _clientRC, UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y );
	_tabX0.HideTab();
	_tabX0.SetCellNum( 2 );
	_tabX0.SetCell( 0, "0", &_canvas101 );
	_tabX0.SetCell( 1, "1", &_canvas201 );

	{
		_canvas101.CreateDirect( &_tabX0, GenRect()( 0, 0, 1000, 1000 ) );

		_lable101.Create( 0, &_canvas101 );
		_lable101.SetString( "软件功能说明" );
		_lable101.SetColor( _BLUE_ );

		_lable102.Create( 0, &_canvas101 );
		_lable102.SetString( "本程序为更衣柜统计管理软件, 具备以下功能:" );
		_lable103.Create( 0, &_canvas101 );
		_lable103.SetString( "(1) 能选定报表目录，并进行相应管理;" );
		_lable104.Create( 0, &_canvas101 );
		_lable104.SetString( "(2) 支持载入约定格式的Excel报表;" );
		_lable105.Create( 0, &_canvas101 );
		_lable105.SetString( "(3) 对载入数据按约定规则进行查询、修改;" );
		_lable106.Create( 0, &_canvas101 );
		_lable106.SetString( "(4) 按规则自动查询并生成过往日期数据;" );
		_lable107.Create( 0, &_canvas101 );
		_lable107.SetString( "(5) 自动生成功能只支持于默认报表目录;" );
		_lable108.Create( 0, &_canvas101 );
		_lable108.SetString( "" );
		_lable109.Create( 0, &_canvas101 );
		_lable109.SetString( "" );

		_layoutGrid101.InitPoint( GenPoint()( 50, 50 ) );
		_layoutGrid101.SetRowColumn( 20, 20 );
		_layoutGrid101.SetRowHeightInterval( 0, 30, 50 );
		//
		_layoutGrid101.SetCell( 0, 3, 0, 4, &_lable101 );
		_layoutGrid101.SetCell( 1, 2, 1, 15, &_lable102 );
		_layoutGrid101.SetCell( 2, 2, 2, 15, &_lable103 );
		_layoutGrid101.SetCell( 3, 2, 3, 15, &_lable104 );
		_layoutGrid101.SetCell( 4, 2, 4, 15, &_lable105 );
		_layoutGrid101.SetCell( 5, 2, 5, 15, &_lable106 );
		_layoutGrid101.SetCell( 6, 2, 6, 15, &_lable107 );
		_layoutGrid101.SetCell( 7, 2, 7, 15, &_lable108 );
		_layoutGrid101.SetCell( 8, 2, 8, 15, &_lable109 );
		//
		_layoutGrid101.CalcGridPos();
	}

	{
		_canvas201.CreateDirect( &_tabX0, GenRect()( 0, 0, 1000, 1000 ) );

		_lable201.Create( 0, &_canvas201 );
		_lable201.SetString( "软件相关说明" );
		_lable201.SetColor( _BLUE_ );

		_lable202.Create( 0, &_canvas201 );
		_lable202.SetString( "更衣室管理程序 Version V1.1" );
		_lable203.Create( 0, &_canvas201 );
		_lable203.SetString( "Copyright 2013 biyou. All Rights Reserved." );
		_lable204.Create( 0, &_canvas201 );
		_lable204.SetString( "Licensed to 中国光大银行" );
		_lable205.Create( 0, &_canvas201 );
		_lable205.SetString( "" );
		_lable206.Create( 0, &_canvas201 );
		_lable206.SetString( "" );
		_lable207.Create( 0, &_canvas201 );
		_lable207.SetString( "" );
		_lable208.Create( 0, &_canvas201 );
		_lable208.SetString( "" );
		_lable209.Create( 0, &_canvas201 );
		_lable209.SetString( "" );

		_layoutGrid201.InitPoint( GenPoint()( 50, 50 ) );
		_layoutGrid201.SetRowColumn( 20, 20 );
		_layoutGrid201.SetRowHeightInterval( 0, 30, 50 );
		//
		_layoutGrid201.SetCell( 0, 3, 0, 4, &_lable201 );
		_layoutGrid201.SetCell( 1, 2, 1, 15, &_lable202 );
		_layoutGrid201.SetCell( 2, 2, 2, 15, &_lable203 );
		_layoutGrid201.SetCell( 3, 2, 3, 15, &_lable204 );
		_layoutGrid201.SetCell( 4, 2, 4, 15, &_lable205 );
		_layoutGrid201.SetCell( 5, 2, 5, 15, &_lable206 );
		_layoutGrid201.SetCell( 6, 2, 6, 15, &_lable207 );
		_layoutGrid201.SetCell( 7, 2, 7, 15, &_lable208 );
		_layoutGrid201.SetCell( 8, 2, 8, 15, &_lable209 );
		//
		_layoutGrid201.CalcGridPos();
	}

	UIRefresh();
}

void UISoftHelp::OnNotify( int id, int param )
{
	if( id==ID_MAINBAR )	// 处理来自菜单栏的消息
	{
		_tabX0.SetCurCell( param-1 );
	}
}