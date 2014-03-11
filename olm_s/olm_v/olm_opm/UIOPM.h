#pragma once



class UIOPM : public UIWindow<UIOPM>, public _QueueThreadManage<UIOPM>
{
	friend UIWindow;

public:
	void ShowVerifyResult( std::vector<std::pair<float, float> >& exceptionPointList, UCHAR channel );
	void ShowOPMPMList( UCHAR pmType, UCHAR beginPort, UCHAR endPort, ULONG* pmList );
	void ShowDXPM();
	void ShowSelDev();

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnSetPortNum();
	void OnGetPowerMeter();
	void OnSetPMWaveLen();
	//
	void OnConnectVOA();
	void OnNoOpticInit();
	void OnCollectData();
	void OnVerifyData();
	void OnAutoCalcDX();
	void OnMonitorPM();
	void OnLoadData();
	void OnGrid22( int param );

	void GetConfigData();
	void GetCalParam();

	UITab _tabX1;

	UICanvas _canvas1;
	//
	UIButton _but1;
	UIButton _but2;
	UIButton _but3;
	//
	UILable _lable1;
	UIComboBox _combo1;
	UIEdit _edit1;
	UIGrid _grid1;

	UICanvas _canvas2;
	//
	UILable _lable21;
	UILable _lable22;
	UILable _lable23;
	UILable _lable24;
	UILable _lable25;
	//
	UIComboBox _combo21;
	UIComboBox _combo22;
	//
	UIEdit _edit21;
	UIEdit _edit22;
	//
	UIGrid _grid21;
	UIGrid _grid22;
	UIGrid _grid23;
	UIGrid _grid24;

	UIButton _but21;
	UIButton _but22;
	UIButton _but23;
	UIButton _but24;
	UIButton _but25;
	UIButton _but26;

	UILayoutGrid _layoutGrid1;
	UILayoutGrid _layoutGrid2;
	UILayoutGrid _layoutGrid3;
	UILayoutGrid _layoutGrid4;
};
extern UIOPM gUIOPM;




