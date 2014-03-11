#pragma once




class UIHardwareTest : public UIWindow<UIHardwareTest>, public _QueueThreadManage<UIHardwareTest>
{
	friend UIWindow;

public:
	void ShowOPMTest( USHORT* pList, int num );

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnDs164E2promTest();
	void OnDs164TempTest();
	void OnLEDTest();
	void OnWatchDogTest();
	void OnI2C0Ds164E2promTest();
	void OnI2C0Ds164TempTest();
	void OnFanTest();
	void OnBackboardE2prom2Test();
	void OnOpmTest();
	void OnOswTest();

	inline ULONG GetTestIP();
	
	UILable _lable1;
	UIButton _but1;
	UIButton _but2;
	UIButton _but3;
	UIButton _but4;

	UILable _lable2;
	UIButton _but5;
	UIButton _but6;

	UILable _lable3;
	UIButton _but7;
	UIButton _but8;

	UILable _lable4;
	UIButton _but9;
	UILable _lable5;
	UIComboBox _combo1;
	
	UILable _lable6;
	UIButton _but10;
	UILable _lable7;
	UIComboBox _combo2;
	UILable _lable8;
	UIEdit _edit1;

	UILable _lable9;
	UIEdit _edit2;

	UIGrid _grid1;

	UILayoutGrid _layoutGrid1;
};
extern UIHardwareTest gUIHardwareTest;