#pragma once




class UIWSSInfo : public UIWindow<UIWSSInfo>, public _QueueThreadManage<UIWSSInfo>
{
	friend UIWindow;

public:
	void ShowMsg( const string msg );
	void ShowSerialNumber( int serialNumber );
	void ShowType( int type );
	void ShowVersion( UCHAR hardVH, UCHAR hardVL, UCHAR firmVH, UCHAR firmVL );
	void ShowHardDateCode( UCHAR hardDateCodeH, UCHAR hardDateCodeL );
	void ShowFirmwareDateCode( UCHAR FirmwareDateCodeH, UCHAR FirmwareDateCodeL );
	void ShowChannelAtten( UCHAR p, UCHAR d );
	void ShowAllChannelAtten( UCHAR *p, float *d );
	void ShowAlarm( UCHAR d );
	void ShowVoltage( USHORT vol );
	void ShowTemperature( float temp );
	void ShowLowTemperature( float temp );
	void ShowHighTemperature( float temp );
	void ShowMemWear( UCHAR d );
	void ShowManufactureID( UCHAR d );

private:
	void OnCreate();
	void OnNotify( int id, int param );

	void OnMainBar( int param );
	void OnReadModuleInfo( int param );
	void OnReadWarningInfo( int param );
	void OnBeginLoad( int param );
	void OnLoadingNow( int param );
	void OnSwitchNew( int param );
	void OnSetModuleInfo1( int param );
	void OnSetModuleInfo2( int param );
	void OnSetModuleInfo3( int param );
	void OnSetModuleInfo4( int param );
	void OnSetTemperatureThreshold( int param );
	void OnConnectSerial( int param );

	//
	bool StartSetCalibrateVoltageRangeThread();
	void SetCalibrateVoltageRangeThread();

	// 布局信息
	UILayoutGrid _layoutGrid0;
	UILayoutGrid _layoutGrid1;
	UILayoutGrid _layoutGrid2;
	UILayoutGrid _layoutGrid3;
	UILayoutGrid _layoutGrid4;

	UITab _tabX1;

	UICanvas _canvas1;
	// 串口相关
	UILable _lable0;
	UIComboBox _comBox0;
	UIImage _status0;
	// 模块信息
	UILable _lable1;
	UIEdit _edit1;
	UILable _lable2;
	UIEdit _edit2;
	UILable _lable3;
	UIEdit _edit3;
	UILable _lable4;
	UIEdit _edit4;
	UILable _lable5;
	UIEdit _edit5;
	UILable _lable6;
	UIEdit _edit6;
	UIButton _but1;
	// 告警信息
	UILable _lable7;
	UIEdit _edit7;
	UILable _lable8;
	UIEdit _edit8;
	UILable _lable9;
	UIEdit _edit9;
	UILable _lable10;
	UIEdit _edit10;
	UILable _lable11;
	UIEdit _edit11;
	UIButton _but2;
	UIButton _but10;

	UICanvas _canvas2;
	// 固件升级
	UILable _lable12;
	UIButton _but3;
	UIButton _but4;
	UIButton _but5;
	// 序列号+类型+硬件版本+硬件日期
	UILable _lable13;
	UIButton _but6;
	UIGrid _grid1;
	// 定标温度配置
	UILable _lable14;
	UIButton _but7;
	UIGrid _grid2;
	// 限压值+采集电压范围
	UILable _lable15;
	UIButton _but8;
	UIButton _but9;
};
extern UIWSSInfo gUIInfo;