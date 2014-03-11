#pragma once

#include "..\\UIConfigPub.h"

class UIWSSConfig : public UIWindow<UIWSSConfig>
{
	friend UIWindow;

public:
	void UIInitPortFreq();

private:
	void OnCreate();
	void OnNotify( int id, int param );

	void OnSetCommunicationMethod( int param );
	void OnSetWSSIP( int param );

	// 布局信息
	UILayoutGrid _layoutGrid1;
	UILayoutGrid _layoutGrid2;

	//
	UIConfigBase _configBase;

	// 控制方式
	UILable _lable1;
	UICheckButton _checkBut1;
	UICheckButton _checkBut2;

	// 控制盘ip
	UIButton _but1;		 
	UIEdit _edit1;
};
extern UIWSSConfig gUIConfig;