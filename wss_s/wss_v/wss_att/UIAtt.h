#pragma once

class UIWSSAtt : public UIWindow<UIWSSAtt>
{
	friend UIWindow;

public:
	void UIInitPortFreq();

	void ShowMsg( const string msg );
	void ShowChannelAtten( UCHAR p, UCHAR d );
	void ShowAllChannelAtten( UCHAR *p, float *d );
	void ShowProfileID( UCHAR pID );

private:
	void OnCreate();
	void OnNotify( int id, int param );

	void OnMainBar( int param );
	void OnGetAllChannels( int id );
	void OnSetAllChannelsOnGrid( int id );
	void OnSetSingleChannel( int id );
	void OnSetOddEvenChannels( int id );
	void OnSetAllChannelsOnSingleValue( int id );	
	void OnReadProfileID( int id );
	void OnReadProfile( int id );
	void OnSelectProfile( int id );
	void OnGenerateProfile( int id );

	// 布局信息
	UILayoutGrid _layoutGrid1;
	UILayoutGrid _layoutGrid2;

	UIGrid _grid1;

	UITab _tabX1;
	
	UICanvas _canvas1;
	//
	UILable _lable1;				// 对应表格操作
	UIButton _but1;	
	UIButton _but2;	
	//
	UILable _lable2;				// 单通道操作
	UIGrid _grid2;
	UIButton _but3;
	//
	UILable _lable3;				// 奇偶通道操作
	UIGrid _grid3;
	UIButton _but4;
	//
	UILable _lable4;				// 全通道操作
	UIGrid _grid4;
	UIButton _but5;

	UICanvas _canvas2;
	//
	UILable _lable5;				// profile
	UIEdit _edit1;
	UIButton _but6;
	UIButton _but7;
	UIButton _but8;
	UIButton _but9;
};
extern UIWSSAtt gUIWSSAtt;