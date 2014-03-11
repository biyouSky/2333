#pragma once

#include "..\\UIConfigPub.h"

class UIChipTest  : public UIWindow<UIChipTest>, public _QueueThreadManage<UIChipTest>
{
	friend UIWindow;

	/*-------------------------------------------UI部分-------------------------------------------*/
public:
		

private:
	void OnCreate();
	void OnNotify( int id, int param );

	void OnMainBar( int param );
	void OnPreChannel( int param );
	void OnNxtChannel( int param );
	void OnStartChipTest( int param );
	void OnShowOdd( int param );
	void OnShowEven( int param );

	void ShowChart();

	UILayoutGrid _layoutGrid1;
	//
	/*UILable _lable1;
	UIEdit _edit1;*/
	UILable _lable2;
	UIEdit _edit2;
	UIButton _but1;
	UIButton _but2;
	UIButton _but3;
	UIChart _chart1;
	UICheckButton _checkBut1;
	UICheckButton _checkBut2;

	/*-------------------------------------------Logic部分-------------------------------------------*/
public:

private:
	void ChipTestThread();

	VECTOR_DOUBLE _xList1[2], _yList1[2];
	VECTOR_DOUBLE _xList2[2], _yList2[2];

};
extern UIChipTest gUITChipTest;


class LaserITLA
{
	bool SetWavelength( std::string waveLen );			// 设置波长
};