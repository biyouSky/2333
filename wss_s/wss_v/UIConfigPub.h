#pragma once

class UIConfigBase : public UIWindow<UIConfigBase>
{
	friend UIWindow;

public:
	void UIInitPortFreq();

private:
	void OnCreate();
	void OnNotify( int id, int param );

	void OnConnectSerial( int param );
	void OnSetWssType( int param );
	void OnSetParseType( int param );

	// ������Ϣ
	UILayoutGrid _layoutGrid1;

	// �������
	UILable _lable1;
	UIComboBox _comBox1;
	UIImage _status1;

	// ��������
	UILable _lable3;				// inport
	UIEdit _edit1;
	UILable _lable4;				// outport
	UIEdit _edit2;
	UILable _lable5;				// freq
	UIComboBox _comboBox2;
	UILable _lable6;				// channel
	UIEdit _edit3;
	UIButton _but1;					// set button
};