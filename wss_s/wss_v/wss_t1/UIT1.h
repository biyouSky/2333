#pragma once

#include "..\\UIConfigPub.h"

class UIT1  : public UIWindow<UIT1>, public _QueueThreadManage<UIT1>
{
	friend UIWindow;

	/*--------------------------------UI--------------------------------*/
public:
	void UIInitPortFreq();

	void ShowYLCurve( VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, VECTOR_DOUBLE& xYList, VECTOR_DOUBLE& yYList );
	void ShowYLGrid();
	
	void GetAllChannelsVol( int xVol[104], int yVol[104] );

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnMainBar( int param );
	void OnSetGrid1Channels( int param );
	void OnSetGrid2Channels( int param );
	void OnSetAllChannels( int param );
	void OnSetOddChannels( int param );
	void OnSetEvenChannels( int param );
	void OnSetSelectChannels( int param );
	void OnClearResults( int param );
	void OnLoadRoughVol( int param );
	void OnLoadExactVol( int param );
	void OnSaveVol( int param );
	void OnChooseFilePath();
	void OnStartYellowLightProcess();
	void OnSetYLOffset();
	void OnSetYLFlag();
	void OnSetRippleDX();
	void OnSetLowPeakLimit();
	void OnStartSaveLightProcess();
	void OnSetY1Range();
	void OnSetXRange();
	void OnConnectDevice();
	void OnDisconnectDevice();
	void OnAutoVol();
	void OnSetOSWChannel1();
	void OnSetOSWChannel2();

	void ShowSetVolResult( int xyFlag, int c, int vol, bool rt );
	void ShowAllChannelsVol( int xVol[104], int yVol[104] );

	void GetDeviceAddr();

	UITab _tabX1;

	// 相关设置
	UICanvas _canvas1;
	UILayoutGrid _layoutGrid11;
	//
	UILable _lable11;
	UIEdit _edit11;
	UILable _lable12;
	UIEdit _edit12;
	UIButton _but11;
	UIEdit _edit13;
	UILable _lable13;
	UIComboBox _combox11;
	UIButton _but12;
	UIButton _but13;
	UILable _lable14;
	UILable _lable15;
	//
	UIConfigBase _baseConfig;

	// 黄线
	UICanvas _canvas2;
	UILayoutGrid _layoutGrid21;
	//
	UIButton _button21;
	UIButton _button22;
	UILable _lable21;
	UIComboBox _combox21;
	UICheckButton _button23;
	UICheckButton _button24;
	UIButton _button25;
	UIEdit _edit21;
	UIButton _button26;
	UIEdit _edit22;
	UIButton _button27;
	UIEdit _edit23;
	UIGrid _grid22;
	//
	UIGrid _grid21;
	UIChart _chart21;

	// 粗略电压
	UICanvas _canvas3;
	UILayoutGrid _layoutGrid31;
	UILayoutGrid _layoutGrid32;
	//
	UILable _lable31;
	UIGrid _grid31;
	UIGrid _grid32;
	//
	//UILable _lable32;
	UICheckButton _checkBut33;
	UIEdit _edit31;
	UICheckButton _checkBut34;
	UIEdit _edit33;
	//
	UICheckButton _checkBut31;
	UICheckButton _checkBut32;
	//
	UIButton _but31;
	UIButton _but32;
	UIButton _but33;
	UIButton _but34;
	UIButton _but35;
	UIButton _but36;
	UIButton _but37;

	UIButton _but38;
	UIEdit _edit32;

	UIButton _but39;
	UIEdit _edit34;

	UIButton _but40;

	// 精确电压
	UICanvas _canvas4;
	//
	UILable _lable41;
	UILable _lable42;
	UILable _lable43;
	UILable _lable44;
	//
	UIEdit _edit41;
	UIEdit _edit42;
	UIEdit _edit43;
	UIEdit _edit44;
	//
	UIButton _but41;
	//
	UILayoutGrid _layoutGrid41;


	/*--------------------------------UI浅层逻辑--------------------------------*/
public:
	bool StartSetChannelsVoltageThread( VECTOR_INT xList, VECTOR_INT yList, VECTOR_INT volXList, VECTOR_INT volYList );
	
	bool StartLoadVoltageFileThread( int flag=0 );
	bool StartSaveVoltageFileThread();
	bool StartAutoVolThread();

private:
	void SetChannelsVoltageThread();
	void SetChanneVoltage( int c, int xyFlag, int vol );

	void LoadRoughVolThread();
	void LoadExactVolThread();
	void LoadVol( int port, std::string file, std::string sheet );
	void SaveVolThread();
	void AutoVolThread();

	// 电压设置
	VECTOR_INT _xList;
	VECTOR_INT _yList;
	VECTOR_INT _volXList;
	VECTOR_INT _volYList;
};
extern UIT1 gUIT1;