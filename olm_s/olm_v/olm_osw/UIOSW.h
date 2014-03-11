#pragma once



class UIOSW : public UIWindow<UIOSW>, public _QueueThreadManage<UIOSW>
{
	friend UIWindow;

public:
	void ShowOSWInfo( UCHAR oswType, USHORT temp, USHORT inportNum, USHORT outportNum, USHORT inportNo, USHORT outportNo );

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnSetOSWType();
	void OnGetOSWState();
	void OnSetOSWState();
	void OnGetPon();
	void OnOSWDB();

	UIButton _but1;
	UIButton _but2;
	UIButton _but3;
	UIButton _but4;

	UILable _lable1;
	UILable _lable2;
	UILable _lable3;
	UILable _lable4;
	UILable _lable5;
	
	UIEdit _edit1;
	UIEdit _edit2;
	UIEdit _edit3;
	UIEdit _edit4;
	UIEdit _edit5;

	UIComboBox _combo1;

	UILayoutGrid _layoutGrid1;
};
extern UIOSW gUIOSW;




