#pragma once

class UIWSSCal : public UIWindow<UIWSSCal>
{
	friend UIWindow;

public:
	void UIInitPortFreq();

	void ShowCollectMsg( ULONG ip, const char* msg );
	void ShowCollectGrid( ULONG ip, int c, const char* msg );
	void ClearCollectChart( ULONG ip );				
	void AddCollectCurve( ULONG ip, int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag );

	void SetParseGridChannel( bool* channelUseFlag );
	void ShowParseGrid( int c, const char* msg );
	void ClearParseChart();					
	void AddParseCurve( int* curveX,  float* curveY, int num, const char* name, COLORREF color, bool curveFlag );

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnMainBar( int param );
	void OnSetCalBoard( int param );
	void OnSetCalChannels( int id, int param );
	void OnReadBoardInfo( int param );
	void OnStartSaveLight( int param );
	void OnStartCal( int param );
	void OnSetDrawBoard1( int param );
	void OnGetFileList( int param );
	void OnLoadFileList( int param );
	void OnGrid5( int param );
	void OnSetParseChannels( int param );
	void OnParseData( int param );
	void OnSendData( int param );
	void OnSetLSB( int param );

	void Func1();
	void Func2();

	int SearchIPIndex( ULONG ip );
	int SearchChannelIndex( ULONG ip, int c );

	// 布局信息
	UILayoutGrid _layoutGrid1;
	UILayoutGrid _layoutGrid2;

	UITab _tabX1;

	UICanvas _canvas1;
	//
	UILable _lable1;				// 温循箱时间
	UIComboBox _comBox1;
	//
	UILable _lable2;				// 监控IP
	UIEdit _edit1;
	//
	UILable _lable3;				// 电路板
	UIGrid _grid1;
	UIButton _but3;
	//
	UILable _lable4;				// 温度点
	UIGrid _grid2;
	//
	UIButton _but1;					// 存光 定标
	UIButton _but2;
	//
	UITab _tabX2;					// 通道选择
	UIGrid _gridList1[4];
	//
	UILable _lable6;				// LSB设置
	UIComboBox _comBox3;

	UICanvas _canvas2;
	//
	UIGrid _grid3;					// 电路板
	UITab _tabX3;
	UITab _tabX4;
	UIGrid _gridList2[4];
	UIChart _chartList1[4];

	UICanvas _canvas3;
	//
	UIGrid _grid4;
	UIGrid _grid5;
	UIChart _chart1;
	UIButton _but4;
	UIButton _but5;
	UIButton _but6;
	UIButton _but7;
	UILable _lable5;
	UIEdit _edit2;
	UICheckButton _checkBut1;
	UICheckButton _checkBut2;
	UIComboBox _comBox2;
	//
	int _fileNum;
	bool _parseChannelList[104];
};

extern UIWSSCal gUICal;