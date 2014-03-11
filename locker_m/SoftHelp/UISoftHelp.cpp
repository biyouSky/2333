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
		_lable101.SetString( "�������˵��" );
		_lable101.SetColor( _BLUE_ );

		_lable102.Create( 0, &_canvas101 );
		_lable102.SetString( "������Ϊ���¹�ͳ�ƹ������, �߱����¹���:" );
		_lable103.Create( 0, &_canvas101 );
		_lable103.SetString( "(1) ��ѡ������Ŀ¼����������Ӧ����;" );
		_lable104.Create( 0, &_canvas101 );
		_lable104.SetString( "(2) ֧������Լ����ʽ��Excel����;" );
		_lable105.Create( 0, &_canvas101 );
		_lable105.SetString( "(3) ���������ݰ�Լ��������в�ѯ���޸�;" );
		_lable106.Create( 0, &_canvas101 );
		_lable106.SetString( "(4) �������Զ���ѯ�����ɹ�����������;" );
		_lable107.Create( 0, &_canvas101 );
		_lable107.SetString( "(5) �Զ����ɹ���ֻ֧����Ĭ�ϱ���Ŀ¼;" );
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
		_lable201.SetString( "������˵��" );
		_lable201.SetColor( _BLUE_ );

		_lable202.Create( 0, &_canvas201 );
		_lable202.SetString( "�����ҹ������ Version V1.1" );
		_lable203.Create( 0, &_canvas201 );
		_lable203.SetString( "Copyright 2013 biyou. All Rights Reserved." );
		_lable204.Create( 0, &_canvas201 );
		_lable204.SetString( "Licensed to �й��������" );
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
	if( id==ID_MAINBAR )	// �������Բ˵�������Ϣ
	{
		_tabX0.SetCurCell( param-1 );
	}
}