#pragma once




class UIOS : public UIWindow<UIOS>, public _QueueThreadManage<UIOS>
{
	friend UIWindow;

public:
	void ShowKB( float k, float b ); 
	bool ShowResistance( ULONG IBMON, ULONG IMPDMON, ULONG resistance );
	void ShowVerifyKB( float* readPM, float* referPM );
	void ShowOSWaveLenList( UCHAR portNum, std::string* waveLenList );
	void ShowOSOutPutPMList( UCHAR portNum, float* pmList );

	void ShowSelDev();

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnGetSample();
	void OnGetPM();
	void OnOpenOS();
	void OnCloseOS();
	void OnGetOSWaveLen();
	void OnSetOSWaveLen();
	void OnGetOutPM();
	void OnGetResistance();
	void OnCalcKB();
	void OnSetCalData();
	void OnVerifyKB();
	void OnSaveToOSDB();
	void OnLoadFromOSDB();
	void OnR1();
	void OnR2();
	//
	void OnStartMonitor();
	void OnStopMonitor();
	void OnReadOSMaxMin();
	void OnAddOS();
	void OnDelOS();
	void OnClearOSList();

	void GetIPList( VECTOR_ULONG& ipList );

	UITab _tabX1;

	UICanvas _canvas1;
	//
	UIButton _but1;
	UIButton _but2;
	UIButton _but3;
	UIButton _but4;
	UIButton _but5;
	UIButton _but6;
	UIButton _but7;
	UIButton _but8;
	UIButton _but9;
	UIButton _but10;
	UIButton _but11;
	UIButton _but12;
	UIButton _but13;

	UIGrid _grid1;
	UIGrid _grid2;
	UIGrid _grid3;
	UIGrid _grid4;

	UICheckButton _check1;
	UICheckButton _check2;
	
	UILable _lable1;
	UILable _lable2;
	UILable _lable3;
	UILable _lable4;
	UILable _lable5;
	UILable _lable6;

	UIEdit _edit1;

	UICanvas _canvas2;
	//
	UIButton _but21;
	UIButton _but22;
	UIButton _but23;
	UIButton _but24;
	UIButton _but25;
	UIButton _but26;

	UILable _lable21;
	UIGrid _grid21;
	UIEdit _edit21;

	UILayoutGrid _layoutGrid1;
	UILayoutGrid _layoutGrid2;
	UILayoutGrid _layoutGrid3;
	UILayoutGrid _layoutGrid4;
};
extern UIOS gUIScan;