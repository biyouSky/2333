#pragma once



class UIMaster : public UIWindow<UIMaster>, public _QueueThreadManage<UIMaster>
{
	friend UIWindow;

public:
	void ShowSelDev();

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnSetFrameSN();

	UIButton _but1;
	
	UILable _lable1;

	UIEdit _edit1;
	UIEdit _edit2;

	UILayoutGrid _layoutGrid1;
};
extern UIMaster gUIMaster;




