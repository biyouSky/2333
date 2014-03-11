#pragma once


class UISoftHelp : public UIWindow<UISoftHelp>
{
	friend UIWindow;

public:


private:
	void OnCreate();
	void OnNotify( int id, int param );

	UITab _tabX0;
	
	// 1
	UICanvas _canvas101;
	//
	UILable _lable101;
	UILable _lable102;
	UILable _lable103;
	UILable _lable104;
	UILable _lable105;
	UILable _lable106;
	UILable _lable107;
	UILable _lable108;
	UILable _lable109;
	// 
	UILayoutGrid _layoutGrid101;

	// 2
	UICanvas _canvas201;
	UILable _lable201;	
	UILable _lable202;	
	UILable _lable203;	
	UILable _lable204;	
	UILable _lable205;	
	UILable _lable206;	
	UILable _lable207;	
	UILable _lable208;	
	UILable _lable209;	
	//
	UILayoutGrid _layoutGrid201;

};
extern UISoftHelp gUISoftHelp;