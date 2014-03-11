#pragma once



class UIT4 : public UIWindow<UIT4>, public _SingleThreadManage<UIT4>
{
	friend UIWindow;

private:
	void OnCreate();
	void OnNotify( int id, int param );
	void OnSaveLight1All();
	void OnSaveLight2All();
	void OnSaveLight1( UINT index );
	void OnSaveLight2( UINT index );
	void OnSaveData1();
	void OnSaveData2();
	void OnSaveData3();
	void OnSaveData4();
	void OnSaveData5();
	void OnSaveData6();
	void OnSaveData7();
	void OnSaveData8();
	void OnSaveSL();
	void OnLoadSL();

	void InspectPM();

	ULONG GetOSWIP();

	UIGrid _grid1;

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
	UIButton _but14;

	UILable _lable1;
	UIEdit _edit1;

	UILayoutGrid _layoutGrid1;

	float _slPM1[16];
	float _slPM2[16];
};


extern UIT4 gUIT4;
