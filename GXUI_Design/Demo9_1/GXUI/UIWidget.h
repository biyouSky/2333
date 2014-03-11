
#pragma once
#include "UIBase.h"
#include "UI2D.h"
#include "UIRender.h"

/*------------------------------------------------------- UIControl -------------------------------------------------------*/
/*

*/
class UIScrollBar;
template<class T, class TBase=UIWinBase>
class UIControl : public TBase
{
public:
	virtual bool HandleMessage( UINT message, WPARAM wParam, LPARAM lParam )
	{
		T* pT = static_cast<T*>( this );

		bool isMsgHandled = DefHandleMessage( message, wParam, lParam );
		if( isMsgHandled ) return true;

		switch( message )
		{
		case WM_CREATE:
			pT->OnCreate();
			break;
		case WM_DESTROY:
			pT->OnDestroy();
			break;
		case WM_SIZE:
			pT->OnSize( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_MOUSEMOVE:
			isMsgHandled = pT->OnMouseMove( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_MOUSELEAVE:
			pT->OnMouseLeave( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_MOUSEWHEEL:
			isMsgHandled = pT->OnMouseWheel( (short)HIWORD(wParam) );
			break;
		case WM_LBUTTONDOWN:
			isMsgHandled = pT->OnLButtonDown( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_LBUTTONUP:
			isMsgHandled = pT->OnLButtonUp( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_RBUTTONDOWN:
			isMsgHandled = pT->OnRButtonDown( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_RBUTTONUP:
			isMsgHandled = pT->OnRButtonUp( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_LBUTTONDBLCLK:
			isMsgHandled = pT->OnLButtonDbClk( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		case WM_KEYDOWN:
			pT->OnKeyDown( (TCHAR)wParam );
			break;
		case WM_CHAR:
			pT->OnChar( (TCHAR)wParam );
			break;
		case WM_SETFOCUS:
			pT->OnSetFocus();
			break;
		case WM_KILLFOCUS:
			pT->OnKillFocus();
			break;
		case WM_COPY:
			isMsgHandled = pT->OnCopy();
			break;
		case WM_PASTE:
			isMsgHandled = pT->OnPaste();
			break;
		case WM_CUT:
			isMsgHandled = pT->OnCut();
			break;		
		case WM_HSCROLL:
			pT->OnHscroll( (int)wParam, (UIScrollBar*)lParam );
			isMsgHandled = true;
			break;
		case WM_VSCROLL:
			pT->OnVscroll( (int)wParam, (UIScrollBar*)lParam );
			isMsgHandled = true;
			break;
		case WM_NOTIFY:
			pT->OnNotify( (int)wParam, (int)lParam );
			isMsgHandled = true;
			break;
		case WM_UPDATE:
			pT->OnUpdate( (UINT)wParam, (void*)lParam );
			isMsgHandled = true;
			break;
		}

		return isMsgHandled;
	}

	bool Create( UINT id, UIContainer* pUIContainer, RECT& relativeRect=_NULLRECT_, int layoutFlag=UILayoutCalc::NO_ZOOM, bool isShow=true, bool isOnHeap=false )
	{
		_id = id;
		return ( (TBase*)this )->CreateDirect( pUIContainer, relativeRect, layoutFlag, isShow, isOnHeap );
	}

	bool Create( UINT id, UIWinBase* pParent, RECT& relativeRect=_NULLRECT_, int layoutFlag=UILayoutCalc::NO_ZOOM, bool isShow=true, bool isOnHeap=false )
	{
		_id = id;
		return ( (TBase*)this )->CreateDirect( pParent, relativeRect, layoutFlag, isShow, isOnHeap );
	}

	UINT _id;

protected:
	// 消息处理
	void OnCreate() { static_cast<T*>( this )->CalcArea(); }
	void OnDestroy() {}
	void OnSize( POINT size ) { static_cast<T*>( this )->CalcArea(); }
	void OnMouseLeave( POINT pt ) {}
	bool OnMouseMove( POINT pt ) { return true; }
	bool OnMouseWheel( short zDelta ) { return false; }
	bool OnLButtonDown( POINT pt ) { return false; }
	bool OnLButtonUp( POINT pt ) { return false; }
	bool OnLButtonDbClk( POINT pt ) { return false; }
	bool OnRButtonDown( POINT pt ) { return false; }
	bool OnRButtonUp( POINT pt ) { return false; }
	void OnKeyDown( TCHAR nChar ) {}
	void OnChar( TCHAR nChar ) {}
	void OnSetFocus() {}
	void OnKillFocus() {}
	bool OnCopy() { return false; }
	bool OnPaste() { return false; }
	bool OnCut() { return false; }
	void OnUpdate( UINT flag, void* data ) {}
	void OnHscroll( int code, UIScrollBar* pScrollBar ) {}
	void OnVscroll( int code, UIScrollBar* pScrollBar ) {}
	void OnNotify( int id, int param ) {}

	// 内部工具函数
	void SendMessageToParent( UINT msg, WPARAM wParam, LPARAM lParam )
	{
		_pParentUIContainer->SendMessageToBindWin( msg, wParam, lParam );
	}
	void CalcArea() {}
};




/*------------------------------------------------------- UILable -------------------------------------------------------*/
/*

*/
class UILable : public UIControl< UILable >
{
	friend UIControl;

public:
	UILable();
	void Draw();

	// 设置函数
	void SetString( std::string str );
	void SetColor( D3DCOLOR color );
	void SetFontHeight( LONG h );

private:
	void OnUpdate( UINT flag, void* data );

	std::string _str;
	D3DCOLOR _color;
	LONG _fontHeight; 

	std::string _strCache;
};



/*------------------------------------------------------- UIImage -------------------------------------------------------*/
/*

*/
class UIImage : public UIControl<UIImage>, public UIAnimateEffect
{
public:
	void Draw();

	// 设置函数
	// void SetImageIDAndDLL( int id, std::string path="GUIResource.dll", D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	// void SetImagePath( std::string path, D3DCOLOR colorKey=D3DFMT_UNKNOWN );
	void PlayAnimateHitDrum();
};



/*------------------------------------------------------- UIButton -------------------------------------------------------*/
/*

*/
class UIButton : public UIControl< UIButton >, public UIAnimateEffect
{
	friend UIControl;

public:
	UIButton();
	void Draw();
	void CalcArea();

	void SetString( std::string windowText );
	void GetString( std::string& windowText );

private:
	bool OnMouseMove( POINT pt );
	void OnMouseLeave( POINT pt );
	bool OnLButtonDown( POINT pt );
	bool OnLButtonUp( POINT pt );
	void OnKeyDown( TCHAR nChar );
	void OnUpdate( UINT flag, void* data );

	// 状态标志
	bool _isHover;						// 鼠标悬停
	bool _isLButtonDown;
	
	// 区域信息
	RECT _strRect;						// 正常状态下显示区域
	RECT _strRect2;						// 按下状态下显示区域
	
	std::string _windowText;			// 显示的string

	// update缓存
	std::string _windowTextCache;
};


/*------------------------------------------------------- UICheckButton -------------------------------------------------------*/
/*

*/
class UICheckButton : public UIControl< UICheckButton >
{
	friend UIControl;

public:
	UICheckButton();
	void Draw();
	void CalcArea();

	void SetString( std::string winString );
	void GetString( std::string& winString );

	void SetCheck( bool f );
	void GetCheck( bool& f );

	void SetMutexList( std::vector<UICheckButton*> mutexList );

private:
	bool OnMouseMove( POINT pt );
	void OnMouseLeave( POINT pt );
	bool OnLButtonDown( POINT pt );
	void OnUpdate( UINT flag, void* data );

	RECT _checkRect;
	RECT _strRect;									// 显示区域
	bool _isHover;									// 鼠标悬停
	bool _isCheck;
	std::string _windowText;						// 显示的string
	
	std::vector<UICheckButton*> _mutexList;			// 互斥相关

	// update缓存
	std::vector<UICheckButton*> _mutexListCache;	
	std::string _windowTextCache;					
};
void UISetCheckButtonMutex( std::vector<UICheckButton*>& mutexList );
void UISetCheckButtonMutex( UICheckButton* but1, UICheckButton* but2 );


/*------------------------------------------------------- UIEdit -------------------------------------------------------*/
/*
	void OnSetCursor(); 未实现
*/
class UIEdit : public UIControl< UIEdit >
{
	friend UIControl;

public:
	UIEdit();
	void Draw();
	void CalcArea();

	void SetString( std::string winString );
	void GetString( std::string& winString );

	void SelectAllText();

private:
	void OnDestroy();
	void OnSetFocus();
	void OnKillFocus();
	bool OnLButtonDown( POINT pt );
	bool OnLButtonDbClk( POINT pt );
	bool OnMouseMove( POINT pt );
	void OnKeyDown( TCHAR nChar );
	void OnChar( TCHAR nChar );
	bool OnPaste();
	bool OnCopy();
	bool OnCut();
	void OnUpdate( UINT flag, void* data );

	void EraseSelectArea();
	void CalcCaretPassChar( POINT& pt );
	void CalcBeginDrawXPos();

	int _fontHeight;					// 字体相关
	D3DCOLOR _fontColor;

	RECT _drawRectAllow;				// 绘制区域 允许绘制字符串的区域
	RECT _drawRectReal;					// 字符串实际绘制区域 可能超过允许绘制区域

	std::wstring _strContent;			// 绘制字符串
	int _beginDrawXPos;					// 绘制起点x坐标

	bool _isCNInput;					// 中文输入法
#ifndef _UNICODE
	std::string _CNStrCache;			// 缓冲字符串 用于非_UNICODE下中文输入 
#endif	
	
	UINT _caretPassChar;				// 插入符略过的字符数	
	bool _isDrawCaretImmd;

	bool _isSelArea;					// 选定区域信息 是否存在选定区域
	UINT _beginAreaPassChar;			// 开始掠过的字符数
	
	// update缓存
	std::wstring _strContentCache;		
};



/*------------------------------------------------------- UISelectList -------------------------------------------------------*/
/*
	UISelectList只支持popup创建
*/
class UISelectList : public UIControl< UISelectList >, public UIAnimateEffect
{
	friend UIControl;

public:
	UISelectList();
	void Draw();

	void AddString( std::string str );
	void GetString( int index, std::string& str );
	void ClearList();
	UINT GetListCount();

private:
	bool OnMouseMove( POINT pt );
	void OnMouseLeave( POINT pt );
	bool OnLButtonDown( POINT pt ); 
	void OnUpdate( UINT flag, void* data );

	VECTOR_STRING _list;
	int _selectIndex;

	bool _isHover;
	int _hoverIndex;

	// update缓存
	struct Cache1
	{
		std::string _string;
	};
};

/*------------------------------------------------------- UIComboBox -------------------------------------------------------*/
/*
	UIComboBox包含了UISelectList
*/
class UIComboBox : public UIControl< UIComboBox >, public UIContainerHelp< UIComboBox >
{
	friend UIControl;

public:
	UIComboBox();
	void Draw();

	void AddString( std::string str );
	bool GetString( std::string& str );
	void SetSelectIndex( int index );
	void ClearList();
	int GetSelectIndex();

	void IsDrawBoader( bool flag );				// 是否绘制边框

private:
	void OnCreate();
	void OnKillFocus();
	bool OnMouseMove( POINT pt );
	void OnMouseLeave( POINT pt );
	bool OnLButtonDbClk( POINT pt );
	bool OnLButtonDown( POINT pt ); 
	void OnNotify( int id, int param );
	void OnUpdate( UINT flag, void* data );

	void SetDropDown( bool flag );

	UISelectList _dropDownList;
	int _selectIndex;

	bool _isDropDown;
	bool _isHover;

	bool _isDrawBoader;
};




/*------------------------------------------------------- UIScrollBar -------------------------------------------------------*/
/*
	
*/
class UIScrollBar : public UIControl< UIScrollBar >
{
	friend UIControl;

public:
	UIScrollBar( int coordFlag=0 );
	void Draw();
	void CalcArea();
	
	void SetPageScale( double s );
	double GetPageScale();
	void SetPosScale( double s );
	double GetPosScale();
	void SetCoordXY( int coordFlag );

private:
	bool OnMouseMove( POINT pt );
	bool OnLButtonDown( POINT pt );
	bool OnLButtonDbClk( POINT pt );

	void OnUpdate( UINT flag, void* data );

	void CalcBarArea();

	int _coordFlag;						// 0: x轴  1: y轴
	double _pageScale;					// scroll与中间区域的长度比例
	double _posScale;					// scroll起始位置与中间区域的长度比例

	bool _isHover;						// 鼠标悬停
	POINT _prePoint;					// 记录前一点
	
	// 区域信息
	RECT _firstRect;					// 
	RECT _secondRect;					// 
	RECT _scrollRect;
	RECT _barRect;					

	// update缓存
	double _pageScaleCache;				// scroll与中间区域的长度比例
	double _posScaleCache;				// scroll起始位置与中间区域的长度比例
};




/*------------------------------------------------------- UIGrid -------------------------------------------------------*/
// 通告信息
struct NM_GRID
{
	int _code;			// 1： VK_RETURN  2： 选中行  3： 选中列  4：选中cell
	int _row;
	int _column;
};

class UIGrid : public UIControl< UIGrid >, public UIContainerHelp< UIGrid >
{
	friend UIControl;

	// Grid_Cell内容
	struct GridCellInfor
	{
		// 
		GridCellInfor( std::string str );
		
		// 创建cell内控件 释放控件
		void CreateControl( UIContainer *pUIContainer, int firstRowPos, int firstColumnPos, int id=0 );
		bool DeleteControl();
		void MoveControl( RECT& rc );		// 移动控件
		void DrawControl();					// 绘制控件

		RECT _pos;							// cell位置

		std::string _strContent;			// string内容
		UINT _wordPos;						// string位置 左中右

		// cell内控件信息
		enum
		{
			EDIT = 0,
			CHECKBUTTON,
			COMBOBOX,
			BUTTON
		};
		int _controlType;					// 控件类型
		UIWinBase* _pCtrl;					// dui控件
		bool _isHold;						// 是否一直维持
	};

public:
	UIGrid();
	void Draw();
	void CalcArea();

	// 设置读取行列数
	void SetRowColumn( int rowNum, int columnNum );
	void GetRowColumn( int& rowNum, int& columnNum );

	// 行高列宽
	void SetRowHeight( UINT row, int height );
	void SetColumnWidth( UINT column, int width );

	// 行列设置fix
	void SetRowFix();
	void SetColumnFix();

	// 内容设置
	// 设置读取cell的字符
	void SetCellString( UINT row, UINT column, std::string str, bool refresh=true );
	void GetCellString( UINT row, UINT column, std::string& str );
	// 添加一行或多行
	void AddRow( VECTOR_STRING& row );						
	void AddRows( std::vector<VECTOR_STRING >& rowList );
	// 清除掉所有行或所有cell内容 fix行除外
	void ClearUnfixRows();									
	void ClearAllCells();
	// 设置区域内的cell信息
	void SetAreaCells( UINT row, UINT column, std::vector<VECTOR_STRING >& rowList );

	// 设置cell内控件
	// 设置cell中button控件
	void SetCellButton( int id, UINT row, UINT column, std::string str );
	// 设置cell中checkButton控件
	void SetCellCheckButton( int id, UINT row, UINT column, std::string str );
	bool GetCellCheckState( UINT row, UINT column, bool& checkState );
	void SetCellCheckState( UINT row, UINT column, bool checkState );
	// 设置cell中comboBox控件
	void SetCellComboBox( int id, UINT row, UINT column );
	void AddCellComboBoxString( UINT row, UINT column, std::string str );
	void ClearCellComboBoxList( UINT row, UINT column );

	// 获得所有选中信息
	bool GetSelectCell( UINT& row, UINT& column );
	bool GetSelectCells( UINT& beginRow, UINT& beginColumn, UINT& endRow, UINT& endColumn );
	void GetSelectRows( std::vector<UINT>& selectRowList );

	UIScrollBar _xScroll;									// 滚动条
	UIScrollBar _yScroll;
	bool _isXScrollShow;									// 滚动条 显示标志
	bool _isYScrollShow;

private:
	void OnCreate();
	bool OnLButtonDown( POINT pt );
	bool OnLButtonDownUnfix( POINT point );
	void OnLButtonDownFix( POINT point );
	bool OnLButtonDbClk( POINT pt );
	void OnHscroll( int code, UIScrollBar* pScrollBar );
	void OnVscroll( int code, UIScrollBar* pScrollBar );
	bool OnMouseWheel( short zDelta );
	bool OnMouseMove( POINT pt );
	void OnKeyDown( TCHAR nChar );
	//
	void OnKeyDownArrows( TCHAR nChar );
	void OnKeyDownProcessKey();
	void OnKeyDownBack();
	void OnKeyDownDelete();
	void OnKeyDownReturn();
	//
	void OnChar( TCHAR nChar );
	bool OnPaste();
	bool OnCopy();
	bool OnCut();
	void OnNotify( int id, int param );
	void OnUpdate( UINT flag, void* data );
	//
	void OnSetRowColumn( void* data );
	void OnSetRowFix();
	void OnSetColumnFix();
	bool OnSetCellString( void* data );
	void OnAddRows( void* data );
	void OnClearUnfixRows();
	void OnClearAllCells();
	void OnSetCellCheckButton( void* data );
	void OnSetRowHeight( void* data );
	void OnSetColumnWidth( void* data );
	void OnSetCellCheckState( void* data );
	void OnSetCellComboBox( void* data );
	void OnAddCellComboBoxString( void* data );
	void OnClearCellComboBoxList( void* data );
	void OnSetCellButton( void* data );
	void OnSetAreaCells( void* data );

	// 内部绘制函数
	void DrawScrollBar();
	void DrawSelect();
	void DrawSelectALL( COLORREF& selectColor );
	void DrawSelectCELL();
	void DrawSelectCELLS( COLORREF& selectColor );
	void DrawSelectROW( COLORREF& selectColor );
	void DrawSelectCOLUMN( COLORREF& selectColor );
	void DrawGrid();
	void DrawCells();
	// 内部计算函数
	void CalcClientRect();
	void CalcScrollBar();
	void CalcGridRect();
	void CalcDrawBeginRowColumn( UINT& beginRow, UINT& beginColumn );
	void CalcCellsPos();
	bool CalcCellIndexUnfix( UINT& row, UINT& column, POINT point );				// 在unFixGrid范围内计算点中的cell
	inline void CalcCellsRange( UINT& beginRow, UINT& endRow, UINT& beginColumn, UINT& endColumn );
	// 粘贴时字符流数据解析
	void SplitLineWordsForGrid( std::string str, std::vector<VECTOR_STRING >& allLineWordsList, const VECTOR_CHAR& splitList );
	void SplitWordsForGrid( std::string line, VECTOR_STRING& words, const VECTOR_CHAR& splitList );
	//	
	void SetNoSel();

	enum
	{
		NO_SEL = 0,
		CELL_SEL,
		CELLS_SEL,
		ROW_SEL,
		COLUMN_SEL,
		ALL_SEL
	};
	int _selectInfo;											// 选中信息
	UINT _selRowBegin, _selColumnBegin;							// 选中的cell
	UINT _selRowEnd, _selColumnEnd;
	std::vector<UINT> _selRowList, _selColumnList;				// 选中的行或列
	bool _allRowColumnActive;									// 选中全部行列标志

	// 绘制区域
	RECT _noScrollArea;											// 去掉滚动条所占的客户区
	RECT _gridArea;												// 客户区中的grid区域
	RECT _unfixGridArea;										// grid区域中的unfix区域
	RECT _xScrollBarArea, _yScrollBarArea;						// 滚动条区域

	bool _isDraw;
	
	UINT _rowNum;												// 行列数
	UINT _columnNum;
	int _firstRowPos;											// 第一行 列的位置
	int _firstColumnPos;
	bool _isFirstRowFix;										// 第一行一列 fix标志
	bool _isFirstColumnFix;
	VECTOR_INT _rowHeightList;									// 行高 列宽
	VECTOR_INT _columnWidthList;
	int _heightSum;												// 所有行高度
	int _widthSum;												// 所有列宽度	
	UINT _beginDrawRow, _beginDrawColumn;						// 绘制的起始行列

	// cell矩阵
	typedef std::vector<std::vector<GridCellInfor> > _cellArrayType;
	_cellArrayType _cellArray;

	// 父窗口通告消息
	NM_GRID _nmGrid;

	// 数据缓存区 在对数据进行变更时先保存在此 
	struct Cache1
	{
		UINT _row;
		UINT _column;
		std::string _str;
		int _flag;
	};
	struct Cache2
	{
		UINT _index;
		int _v;
	};
	struct Cache3
	{
		UINT _row;
		UINT _column;
		bool _flag;
	};
	struct Cache4
	{
		UINT _row;
		UINT _column;
		std::vector<VECTOR_STRING > _rowList;
	};
};




/*------------------------------------------------------- UIChart -------------------------------------------------------*/
// 类型定义
typedef std::pair<double, double>	POINT_DOUBLE;
typedef std::vector<POINT_DOUBLE>	VECTOR_POINT; 

class UIChart : public UIControl< UIChart >
{
	friend UIControl;	

	struct CurveInfor
	{	
		CurveInfor();

		bool CalcCoordRange( double& xMin, double& xMax, double& yMin, double& yMax );		// 计算线条的坐标范围
		bool JudgePointNearLine( POINT& p0, POINT& p1, POINT& p2 );							// 判断点p0是否靠近线( p1,p2 )
		bool operator==( std::string strName );												// 重载比较  name为key

		VECTOR_POINT _pointList;
		bool _isXCoordInOrder;																// 标示x轴方向上数据是否有序( 从小到大 )
		int _beginPointNO, _endPointNO;														// 绘制区间[beginPointNO, endPointNO]已序号进行标识

		std::string _name;																	// 线条名字，作为key使用
		D3DCOLOR _color;																	// 线条颜色
	
		bool _isSelect;																		// 选中标志
		bool _isShow;																		// 显示标志
		bool _isLine;																		// 连线标志
	};
	typedef std::list<CurveInfor> CURVE_LIST;

public:
	UIChart();
	void CalcArea();
	void Draw();

	// 添加相对于左轴的曲线
	void AddCurve1( std::string strName );
	void AddCurve1( std::string strName, double xValue, double yValue, bool isRefresh=false );
	void AddCurve1( std::string strName, VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, bool isXCoordInOrder=true );
	// 添加相对于右轴的曲线
	void AddCurve2( std::string strName );
	void AddCurve2( std::string strName, double xValue, double yValue, bool isRefresh=false );
	void AddCurve2( std::string strName, VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, bool isXCoordInOrder=true );
	// 清除所有曲线
	void Clear();

	// 设置曲线颜色
	void SetCurve1Color( std::string strName, D3DCOLOR color );
	void SetCurve2Color( std::string strName, D3DCOLOR color );

	// 设置曲线选中状态
	void SetCurve1Select( std::string strName );
	void SetCurve2Select( std::string strName ); 

	// 设置、获取曲线的范围
	void SetXYCoordRange( double xMin, double xMax, double y1Min, double y1Max, double y2Min, double y2Max );
	void GetXYCoordRange( double& xMin, double& xMax, double& y1Min, double& y1Max, double& y2Min, double& y2Max );

	// 设置坐标限制
	void SetXCoordLimit( double xMinLimit, double xMaxLimit );
	void SetY1CoordLimit( double y1MinLimit, double y1MaxLimit );
	void SetY2CoordLimit( double y2MinLimit, double y2MaxLimit );

	// 设置坐标的对称性
	void SetXCoordSymmetry();
	void SetY1CoordSymmetry();
	void SetY2CoordSymmetry();

	// 计算坐标的范围
	void CalcXYCoordRange( bool isRefresh=true );

private:
	bool OnLButtonDown( POINT pt );
	bool OnLButtonUp( POINT pt );
	bool OnRButtonDown( POINT pt );
	bool OnRButtonUp( POINT pt );
	bool OnMouseMove( POINT pt );
	void OnUpdate( UINT flag, void* data );
	//
	bool OnCalcXYCoordRange( void* data );
	void OnAddCurve1( void* data );
	bool OnAddCurve2( void* data );
	void OnAddCurve3( void* data );
	void OnSetCurveColor( void* data );
	void OnClear();
	void OnSetCoordSymmetry( void* data );
	void OnSetCurveSelect( void* data );

	// 具体绘制
	void DrawGrid();
	void DrawXCoordLable();
	void DrawY1CoordLable();
	void DrawY2CoordLable();
	void DrawXCoord();
	void DrawY1Coord();
	void DrawY2Coord();
	void DrawZoomRect();
	void DrawMousePosAndToolTip();
	void DrawCurveList1();
	void DrawCurveList2();
	void DrawCurve( CurveInfor& curve, int yFlag );
	void DrawPoint( POINT& pointPos, D3DCOLOR& color, bool bigPointFlag );

	// 坐标范围相关
	void SaveCurCoordRange();														// 保存当前坐标范围
	void CalcCurveListDrawRange( int mode = LEFTMOVE );
	void CalcCurveDrawRange( CurveInfor& curve, int mode = LEFTMOVE );
	void CalcCoordSymmetry( POINT_DOUBLE& coordRange );								// 计算坐标对称性

	// 坐标值与真实位置之间的相互转换
	void TransfromCoordToPos( POINT& pointPos, POINT_DOUBLE& pointCoord, int yFlag );
	bool TransfromPosToCoord( POINT& pointPos, POINT_DOUBLE& pointCoord, int yFlag );

	// 点范围判定
	bool IsPosPointInbox( POINT& pointPos, RECT& rect );
	bool IsCoordPointInbox( POINT_DOUBLE& pointCoord, POINT_DOUBLE& p1, POINT_DOUBLE& p2, bool nearYFlag=false );
	bool IsCoordPointInCoordRange( POINT_DOUBLE& pointCoord, int yFlag );

	// 判定点是否在曲线附近
	bool IsPointNearCurve( POINT& point, int yFlag );
	bool IsPointNearCurvePoint( POINT& point, CurveInfor& curve, int yFlag );
	bool IsPointNearCurveLine( POINT& point, CurveInfor& curve, int yFlag );

	// 计算交点
	POINT_DOUBLE CalcIntersectionPoint( POINT_DOUBLE& prePoint, POINT_DOUBLE& curPoint, int outBoxIndex, int yFlag );
	inline bool CalcLeftIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );
	inline bool CalcTopIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );
	inline bool CalcRightIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );
	inline bool CalcBottomIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );

	// 内部计算模式
	enum CalcModeInside
	{
		NOTUSELASTDATA = 0,
		LEFTMOVE,
		RIGHTMOVE,
		ZOOM,
		SHRINK,
		ADDPOINT
	};
		
	RECT _gridRect;															// 区域信息
	
	int _rowNum;															// grid框行列数
	int _columnNum;

	CURVE_LIST _curveList1;													// 曲线列表 左y轴
	CURVE_LIST _curveList2;													// 曲线列表 右y轴
	
	double _coordToPosScaleX;												// 比例: coordinate/positon  用于提高计算效率
	double _coordToPosScaleY1;												
	double _coordToPosScaleY2;

	POINT_DOUBLE _xCoordRange, _y1CoordRange, _y2CoordRange;				// 记录x y轴坐标范围
	VECTOR_POINT _xCoordHistory, _y1CoordHistory, _y2CoordHistory;			// 记录历史坐标范围
	bool _isY1CoordRangeCalc;												// xy坐标范围计算标志
	bool _isY2CoordRangeCalc;												// xy坐标范围计算标志
	
	bool _isDrawZoomRect;													// zoom框信息
	POINT _lPointBeginPos, _lPointEndPos;
	POINT _rPointPosUndo;
	
	bool _isMove;															// move信息
	POINT _rPointPosMove;
	
	bool _isShowToolTip;													// tooltip 替代
	std::string _tooltipStr; 
	
	std::string _mouseCoordStr;												// 坐标显示
	bool _isRoomEnoughDraw;													// 空间是否足够绘制标志

	bool _isXCoordLimit, _isY1CoordLimit, _isY2CoordLimit;					// 外部计算条件
	double _xCoordMinLimit, _xCoordMaxLimit;
	double _y1CoordMinLimit, _y1CoordMaxLimit;
	double _y2CoordMinLimit, _y2CoordMaxLimit;
	bool _isXCoordSymmetry, _isY1CoordSymmetry, _isY2CoordSymmetry;

	// update缓存
	struct Cache1
	{
		std::string _curveName;
		int _yFlag;
	};
	struct Cache2
	{
		double _x;
		double _y;
		std::string _curveName;
		int _yFlag;

		bool _isRefresh;
	};
	struct Cache3
	{
		VECTOR_DOUBLE _xList;
		VECTOR_DOUBLE _yList;
		std::string _curveName;
		int _yFlag;

		bool _isXCoordInOrder;
	};
	struct Cache4
	{
		D3DCOLOR _color;
		std::string _curveName;
		int _yFlag;
	};
	struct Cache5
	{
		int _flag;		// 1-3 coord Limit  4-6
		double _min;
		double _max;
	};
};





/*------------------------------------------------------- UITabX -------------------------------------------------------*/
class UITab : public UIControl<UITab>, public UIContainerHelp<UITab>, public UIAnimateEffect
{
	friend UIControl;

	/*--------------------------------- 控件数据 ---------------------------------*/
public:
	UITab();

	void CalcArea();
	void Draw();

	void SetCellNum( UINT num );
	void SetCell( UINT index, std::string title, UIWinBase* pWin );
	void SetCurCell( UINT index );
	void SetX();
	void SetY();
	void ShowTab();
	void HideTab();

private:
	bool OnMouseMove( POINT pt );
	void OnMouseLeave( POINT pt );
	bool OnLButtonDown( POINT pt );
	void OnUpdate( UINT flag, void* data );

	void DrawTab();
	void DrawTabSel();
	void DrawAnimate1();
	void DrawAnimate1Cell();
	void DrawAnimate1Tab();

	void CalcCellListRect();
	void CalcTabRect();
	void CalcTabSelLine();

	enum 
	{
		X_FLAG = 0,
		Y_FLAG = 1
	} _xyFlag;									// 方向标志
	enum										
	{											
		TAB_1 = 30,
		TAB_2 = 3
	};											// 宽度值
	enum
	{
		MAX_FRAME1 = 5
	};											// 动画帧数

	bool _isDraw;

	// tab相关
	bool _isShowTab;
	RECT _tabRC;
	std::vector<RECT> _tabRCList;
	int _hoverIndex;
	RECT _lineRC;

	// cellList
	struct CellData
	{
		std::string _title;
		UIWinBase* _pWin;

		CellData();
	};
	std::vector<CellData > _cellList;
	std::vector<RECT> _cellRCList;
	RECT _cellRC;
	int _selIndex;

	// update缓存
	struct Cache1
	{
		UINT _index;
		std::string _str;
		UIWinBase* _pWin;
	};
};

/*------------------------------------------------------- UICanvas -------------------------------------------------------*/
// canvas画布  消息会直接透射
class UICanvas : public UIWinBase, public UIContainerHelp<UICanvas >
{
public:
	UICanvas();
};



/*------------------------------------------------------- UIWindow -------------------------------------------------------*/
template<class T>
class UIWindow : public UIWinBase, public UIContainerHelp<UIWindow<T> >
{
protected:
	// 消息处理
	virtual bool HandleMessage( UINT message, WPARAM wParam, LPARAM lParam )
	{
		T* pT = static_cast<T*>( this );

		bool isMsgHandled = DefHandleMessage( message, wParam, lParam );
		if( isMsgHandled==true ) return true;

		switch( message )
		{
		case WM_CREATE:
			pT->OnCreate();
			break;
		case WM_NOTIFY:
			pT->OnNotify( wParam, lParam );
			break;
		case WM_SIZE:
			pT->OnSize( GenPoint()( LOWORD(lParam), HIWORD(lParam) ) );
			break;
		};

		return isMsgHandled;
	}
	void OnCreate() {}
	void OnNotify( int id, int param ) {}
	void OnSize( POINT size ) {}
};



/*------------------------------------------------------- UILayoutGrid -------------------------------------------------------*/
/*
	UILayoutGrid 提供类似grid的布局计算

	row1: ctrl11 Interval1 ctrl12  Interval2 ctrl13 Interval3 ......
	Interval_row
	row2: ctrl21 Interval1 ctrl22  Interval2 ctrl23 Interval3 ......
	Interval_row
	......

*/
class UILayoutGrid
{
public:
	// 初始化
	void InitPoint( POINT& relativePT );

	// 设置cell信息
	void SetRowColumn( UINT row, UINT column, int width=100, int widthInterval=20, int height=30, int heightInterval=20 );
	void SetCell( UINT bRow, UINT bColumn, UINT eRow, UINT eColumn, UIWinBase* pWin );
	void SetCell( UINT row, UINT column, UIWinBase* pWin );

	// 设置长宽及间隔
	void SetColumnWidthInterval( UINT column, int width=100, int interval=20 );
	void SetRowHeightInterval( UINT row, int heigth=30, int interval=20 );

	// 计算位置信息
	void CalcGridPos();

private:
	struct CellInfor
	{
		UIWinBase* _pWin;
		int _endRow;
		int _endColumn;

		CellInfor();
	};

	typedef std::vector<CellInfor> ROW_TYPE;
	std::vector<ROW_TYPE > _grid;

	VECTOR_INT _rowHeightList;
	VECTOR_INT _columnWidthlList;
	
	VECTOR_INT _rowIntervalList;
	VECTOR_INT _columnIntervalList;

	POINT _relativePT;
};