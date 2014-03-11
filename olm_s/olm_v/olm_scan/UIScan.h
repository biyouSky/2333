#pragma once




class UIScan : public UIWindow<UIScan>, public _QueueThreadManage<UIScan>
{
	friend UIWindow;

public:
	void ShowProduct( DeviceInfo* pDev );

private:
	void OnCreate();
	void OnNotify( int id, int param );

	UIGrid _grid1;
	UILable _lable1;
	UIEdit _edit1;
	UIButton _but1;
	UIButton _but2;

	UIComboBox _combo1;
	UIButton _but3;

	UILayoutGrid _layoutGrid1;
};
extern UIScan gUIScan;