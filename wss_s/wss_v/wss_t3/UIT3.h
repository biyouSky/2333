#pragma once


class UIT3 : public UIWindow<UIT3>
{
	friend UIWindow;

	struct UIIndicatorShow : public UICanvas
	{
		UIGrid _grid1;
		void OnCreate(){ _grid1.Create( 0, this, GenRect()( 20, 20, 980, 980 ), UILayoutCalc::SIZE_X|UILayoutCalc::SIZE_Y ); }
	};

public:
	void UIInitPortFreq();

	void ClearIndicatorChart();
	void AddIndicatorCurve( double* curveX,  double* curveY, int num, const char* name, COLORREF color, bool curveFlag );

	void SetIndicatorGrid( int port, int row, int column );
	void SetIndicatorGridCell( int port, int row, int column, const char* v, COLORREF color );

private:
	void OnCreate();
	void OnNotify( int id, int param );
	void SetIndicatorITU();

	// ÔØÈëÎÄ¼þ
	UITab _tabX1;							// 1

	UICanvas _canvas1;						// 2
	//
	UIGrid _grid1;							// 3
	UIGrid _grid2;							// 3
	UIButton _but1;							// 3
	UIButton _but2;							// 3

	UITab _tabY1;							// 2
	//
	UIIndicatorShow _indicatorShowList[9];	// 3

	UICanvas _canvas2;
	//
	UILable _lable21;
	UILable _lable22;
	UIEdit _edit21;
	UIEdit _edit22;
	//
	UILable _lable23;
	UIComboBox _combo21;
	//
	UILable _lable24;
	UIComboBox _combo22;
	//
	UIGrid _grid21;
	UIGrid _grid22;
	UIGrid _grid23;

	UILayoutGrid _layoutGrid1;
};

extern UIT3 gUIT3;