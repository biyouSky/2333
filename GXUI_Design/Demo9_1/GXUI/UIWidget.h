
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
	// ��Ϣ����
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

	// �ڲ����ߺ���
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

	// ���ú���
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

	// ���ú���
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

	// ״̬��־
	bool _isHover;						// �����ͣ
	bool _isLButtonDown;
	
	// ������Ϣ
	RECT _strRect;						// ����״̬����ʾ����
	RECT _strRect2;						// ����״̬����ʾ����
	
	std::string _windowText;			// ��ʾ��string

	// update����
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
	RECT _strRect;									// ��ʾ����
	bool _isHover;									// �����ͣ
	bool _isCheck;
	std::string _windowText;						// ��ʾ��string
	
	std::vector<UICheckButton*> _mutexList;			// �������

	// update����
	std::vector<UICheckButton*> _mutexListCache;	
	std::string _windowTextCache;					
};
void UISetCheckButtonMutex( std::vector<UICheckButton*>& mutexList );
void UISetCheckButtonMutex( UICheckButton* but1, UICheckButton* but2 );


/*------------------------------------------------------- UIEdit -------------------------------------------------------*/
/*
	void OnSetCursor(); δʵ��
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

	int _fontHeight;					// �������
	D3DCOLOR _fontColor;

	RECT _drawRectAllow;				// �������� ��������ַ���������
	RECT _drawRectReal;					// �ַ���ʵ�ʻ������� ���ܳ��������������

	std::wstring _strContent;			// �����ַ���
	int _beginDrawXPos;					// �������x����

	bool _isCNInput;					// �������뷨
#ifndef _UNICODE
	std::string _CNStrCache;			// �����ַ��� ���ڷ�_UNICODE���������� 
#endif	
	
	UINT _caretPassChar;				// ������Թ����ַ���	
	bool _isDrawCaretImmd;

	bool _isSelArea;					// ѡ��������Ϣ �Ƿ����ѡ������
	UINT _beginAreaPassChar;			// ��ʼ�ӹ����ַ���
	
	// update����
	std::wstring _strContentCache;		
};



/*------------------------------------------------------- UISelectList -------------------------------------------------------*/
/*
	UISelectListֻ֧��popup����
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

	// update����
	struct Cache1
	{
		std::string _string;
	};
};

/*------------------------------------------------------- UIComboBox -------------------------------------------------------*/
/*
	UIComboBox������UISelectList
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

	void IsDrawBoader( bool flag );				// �Ƿ���Ʊ߿�

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

	int _coordFlag;						// 0: x��  1: y��
	double _pageScale;					// scroll���м�����ĳ��ȱ���
	double _posScale;					// scroll��ʼλ�����м�����ĳ��ȱ���

	bool _isHover;						// �����ͣ
	POINT _prePoint;					// ��¼ǰһ��
	
	// ������Ϣ
	RECT _firstRect;					// 
	RECT _secondRect;					// 
	RECT _scrollRect;
	RECT _barRect;					

	// update����
	double _pageScaleCache;				// scroll���м�����ĳ��ȱ���
	double _posScaleCache;				// scroll��ʼλ�����м�����ĳ��ȱ���
};




/*------------------------------------------------------- UIGrid -------------------------------------------------------*/
// ͨ����Ϣ
struct NM_GRID
{
	int _code;			// 1�� VK_RETURN  2�� ѡ����  3�� ѡ����  4��ѡ��cell
	int _row;
	int _column;
};

class UIGrid : public UIControl< UIGrid >, public UIContainerHelp< UIGrid >
{
	friend UIControl;

	// Grid_Cell����
	struct GridCellInfor
	{
		// 
		GridCellInfor( std::string str );
		
		// ����cell�ڿؼ� �ͷſؼ�
		void CreateControl( UIContainer *pUIContainer, int firstRowPos, int firstColumnPos, int id=0 );
		bool DeleteControl();
		void MoveControl( RECT& rc );		// �ƶ��ؼ�
		void DrawControl();					// ���ƿؼ�

		RECT _pos;							// cellλ��

		std::string _strContent;			// string����
		UINT _wordPos;						// stringλ�� ������

		// cell�ڿؼ���Ϣ
		enum
		{
			EDIT = 0,
			CHECKBUTTON,
			COMBOBOX,
			BUTTON
		};
		int _controlType;					// �ؼ�����
		UIWinBase* _pCtrl;					// dui�ؼ�
		bool _isHold;						// �Ƿ�һֱά��
	};

public:
	UIGrid();
	void Draw();
	void CalcArea();

	// ���ö�ȡ������
	void SetRowColumn( int rowNum, int columnNum );
	void GetRowColumn( int& rowNum, int& columnNum );

	// �и��п�
	void SetRowHeight( UINT row, int height );
	void SetColumnWidth( UINT column, int width );

	// ��������fix
	void SetRowFix();
	void SetColumnFix();

	// ��������
	// ���ö�ȡcell���ַ�
	void SetCellString( UINT row, UINT column, std::string str, bool refresh=true );
	void GetCellString( UINT row, UINT column, std::string& str );
	// ���һ�л����
	void AddRow( VECTOR_STRING& row );						
	void AddRows( std::vector<VECTOR_STRING >& rowList );
	// ����������л�����cell���� fix�г���
	void ClearUnfixRows();									
	void ClearAllCells();
	// ���������ڵ�cell��Ϣ
	void SetAreaCells( UINT row, UINT column, std::vector<VECTOR_STRING >& rowList );

	// ����cell�ڿؼ�
	// ����cell��button�ؼ�
	void SetCellButton( int id, UINT row, UINT column, std::string str );
	// ����cell��checkButton�ؼ�
	void SetCellCheckButton( int id, UINT row, UINT column, std::string str );
	bool GetCellCheckState( UINT row, UINT column, bool& checkState );
	void SetCellCheckState( UINT row, UINT column, bool checkState );
	// ����cell��comboBox�ؼ�
	void SetCellComboBox( int id, UINT row, UINT column );
	void AddCellComboBoxString( UINT row, UINT column, std::string str );
	void ClearCellComboBoxList( UINT row, UINT column );

	// �������ѡ����Ϣ
	bool GetSelectCell( UINT& row, UINT& column );
	bool GetSelectCells( UINT& beginRow, UINT& beginColumn, UINT& endRow, UINT& endColumn );
	void GetSelectRows( std::vector<UINT>& selectRowList );

	UIScrollBar _xScroll;									// ������
	UIScrollBar _yScroll;
	bool _isXScrollShow;									// ������ ��ʾ��־
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

	// �ڲ����ƺ���
	void DrawScrollBar();
	void DrawSelect();
	void DrawSelectALL( COLORREF& selectColor );
	void DrawSelectCELL();
	void DrawSelectCELLS( COLORREF& selectColor );
	void DrawSelectROW( COLORREF& selectColor );
	void DrawSelectCOLUMN( COLORREF& selectColor );
	void DrawGrid();
	void DrawCells();
	// �ڲ����㺯��
	void CalcClientRect();
	void CalcScrollBar();
	void CalcGridRect();
	void CalcDrawBeginRowColumn( UINT& beginRow, UINT& beginColumn );
	void CalcCellsPos();
	bool CalcCellIndexUnfix( UINT& row, UINT& column, POINT point );				// ��unFixGrid��Χ�ڼ�����е�cell
	inline void CalcCellsRange( UINT& beginRow, UINT& endRow, UINT& beginColumn, UINT& endColumn );
	// ճ��ʱ�ַ������ݽ���
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
	int _selectInfo;											// ѡ����Ϣ
	UINT _selRowBegin, _selColumnBegin;							// ѡ�е�cell
	UINT _selRowEnd, _selColumnEnd;
	std::vector<UINT> _selRowList, _selColumnList;				// ѡ�е��л���
	bool _allRowColumnActive;									// ѡ��ȫ�����б�־

	// ��������
	RECT _noScrollArea;											// ȥ����������ռ�Ŀͻ���
	RECT _gridArea;												// �ͻ����е�grid����
	RECT _unfixGridArea;										// grid�����е�unfix����
	RECT _xScrollBarArea, _yScrollBarArea;						// ����������

	bool _isDraw;
	
	UINT _rowNum;												// ������
	UINT _columnNum;
	int _firstRowPos;											// ��һ�� �е�λ��
	int _firstColumnPos;
	bool _isFirstRowFix;										// ��һ��һ�� fix��־
	bool _isFirstColumnFix;
	VECTOR_INT _rowHeightList;									// �и� �п�
	VECTOR_INT _columnWidthList;
	int _heightSum;												// �����и߶�
	int _widthSum;												// �����п��	
	UINT _beginDrawRow, _beginDrawColumn;						// ���Ƶ���ʼ����

	// cell����
	typedef std::vector<std::vector<GridCellInfor> > _cellArrayType;
	_cellArrayType _cellArray;

	// ������ͨ����Ϣ
	NM_GRID _nmGrid;

	// ���ݻ����� �ڶ����ݽ��б��ʱ�ȱ����ڴ� 
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
// ���Ͷ���
typedef std::pair<double, double>	POINT_DOUBLE;
typedef std::vector<POINT_DOUBLE>	VECTOR_POINT; 

class UIChart : public UIControl< UIChart >
{
	friend UIControl;	

	struct CurveInfor
	{	
		CurveInfor();

		bool CalcCoordRange( double& xMin, double& xMax, double& yMin, double& yMax );		// �������������귶Χ
		bool JudgePointNearLine( POINT& p0, POINT& p1, POINT& p2 );							// �жϵ�p0�Ƿ񿿽���( p1,p2 )
		bool operator==( std::string strName );												// ���رȽ�  nameΪkey

		VECTOR_POINT _pointList;
		bool _isXCoordInOrder;																// ��ʾx�᷽���������Ƿ�����( ��С���� )
		int _beginPointNO, _endPointNO;														// ��������[beginPointNO, endPointNO]����Ž��б�ʶ

		std::string _name;																	// �������֣���Ϊkeyʹ��
		D3DCOLOR _color;																	// ������ɫ
	
		bool _isSelect;																		// ѡ�б�־
		bool _isShow;																		// ��ʾ��־
		bool _isLine;																		// ���߱�־
	};
	typedef std::list<CurveInfor> CURVE_LIST;

public:
	UIChart();
	void CalcArea();
	void Draw();

	// �����������������
	void AddCurve1( std::string strName );
	void AddCurve1( std::string strName, double xValue, double yValue, bool isRefresh=false );
	void AddCurve1( std::string strName, VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, bool isXCoordInOrder=true );
	// �����������������
	void AddCurve2( std::string strName );
	void AddCurve2( std::string strName, double xValue, double yValue, bool isRefresh=false );
	void AddCurve2( std::string strName, VECTOR_DOUBLE& xList, VECTOR_DOUBLE& yList, bool isXCoordInOrder=true );
	// �����������
	void Clear();

	// ����������ɫ
	void SetCurve1Color( std::string strName, D3DCOLOR color );
	void SetCurve2Color( std::string strName, D3DCOLOR color );

	// ��������ѡ��״̬
	void SetCurve1Select( std::string strName );
	void SetCurve2Select( std::string strName ); 

	// ���á���ȡ���ߵķ�Χ
	void SetXYCoordRange( double xMin, double xMax, double y1Min, double y1Max, double y2Min, double y2Max );
	void GetXYCoordRange( double& xMin, double& xMax, double& y1Min, double& y1Max, double& y2Min, double& y2Max );

	// ������������
	void SetXCoordLimit( double xMinLimit, double xMaxLimit );
	void SetY1CoordLimit( double y1MinLimit, double y1MaxLimit );
	void SetY2CoordLimit( double y2MinLimit, double y2MaxLimit );

	// ��������ĶԳ���
	void SetXCoordSymmetry();
	void SetY1CoordSymmetry();
	void SetY2CoordSymmetry();

	// ��������ķ�Χ
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

	// �������
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

	// ���귶Χ���
	void SaveCurCoordRange();														// ���浱ǰ���귶Χ
	void CalcCurveListDrawRange( int mode = LEFTMOVE );
	void CalcCurveDrawRange( CurveInfor& curve, int mode = LEFTMOVE );
	void CalcCoordSymmetry( POINT_DOUBLE& coordRange );								// ��������Գ���

	// ����ֵ����ʵλ��֮����໥ת��
	void TransfromCoordToPos( POINT& pointPos, POINT_DOUBLE& pointCoord, int yFlag );
	bool TransfromPosToCoord( POINT& pointPos, POINT_DOUBLE& pointCoord, int yFlag );

	// �㷶Χ�ж�
	bool IsPosPointInbox( POINT& pointPos, RECT& rect );
	bool IsCoordPointInbox( POINT_DOUBLE& pointCoord, POINT_DOUBLE& p1, POINT_DOUBLE& p2, bool nearYFlag=false );
	bool IsCoordPointInCoordRange( POINT_DOUBLE& pointCoord, int yFlag );

	// �ж����Ƿ������߸���
	bool IsPointNearCurve( POINT& point, int yFlag );
	bool IsPointNearCurvePoint( POINT& point, CurveInfor& curve, int yFlag );
	bool IsPointNearCurveLine( POINT& point, CurveInfor& curve, int yFlag );

	// ���㽻��
	POINT_DOUBLE CalcIntersectionPoint( POINT_DOUBLE& prePoint, POINT_DOUBLE& curPoint, int outBoxIndex, int yFlag );
	inline bool CalcLeftIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );
	inline bool CalcTopIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );
	inline bool CalcRightIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );
	inline bool CalcBottomIntersectionPoint( POINT_DOUBLE& pointCoord, double& k, double& b, int yFlag );

	// �ڲ�����ģʽ
	enum CalcModeInside
	{
		NOTUSELASTDATA = 0,
		LEFTMOVE,
		RIGHTMOVE,
		ZOOM,
		SHRINK,
		ADDPOINT
	};
		
	RECT _gridRect;															// ������Ϣ
	
	int _rowNum;															// grid��������
	int _columnNum;

	CURVE_LIST _curveList1;													// �����б� ��y��
	CURVE_LIST _curveList2;													// �����б� ��y��
	
	double _coordToPosScaleX;												// ����: coordinate/positon  ������߼���Ч��
	double _coordToPosScaleY1;												
	double _coordToPosScaleY2;

	POINT_DOUBLE _xCoordRange, _y1CoordRange, _y2CoordRange;				// ��¼x y�����귶Χ
	VECTOR_POINT _xCoordHistory, _y1CoordHistory, _y2CoordHistory;			// ��¼��ʷ���귶Χ
	bool _isY1CoordRangeCalc;												// xy���귶Χ�����־
	bool _isY2CoordRangeCalc;												// xy���귶Χ�����־
	
	bool _isDrawZoomRect;													// zoom����Ϣ
	POINT _lPointBeginPos, _lPointEndPos;
	POINT _rPointPosUndo;
	
	bool _isMove;															// move��Ϣ
	POINT _rPointPosMove;
	
	bool _isShowToolTip;													// tooltip ���
	std::string _tooltipStr; 
	
	std::string _mouseCoordStr;												// ������ʾ
	bool _isRoomEnoughDraw;													// �ռ��Ƿ��㹻���Ʊ�־

	bool _isXCoordLimit, _isY1CoordLimit, _isY2CoordLimit;					// �ⲿ��������
	double _xCoordMinLimit, _xCoordMaxLimit;
	double _y1CoordMinLimit, _y1CoordMaxLimit;
	double _y2CoordMinLimit, _y2CoordMaxLimit;
	bool _isXCoordSymmetry, _isY1CoordSymmetry, _isY2CoordSymmetry;

	// update����
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

	/*--------------------------------- �ؼ����� ---------------------------------*/
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
	} _xyFlag;									// �����־
	enum										
	{											
		TAB_1 = 30,
		TAB_2 = 3
	};											// ���ֵ
	enum
	{
		MAX_FRAME1 = 5
	};											// ����֡��

	bool _isDraw;

	// tab���
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

	// update����
	struct Cache1
	{
		UINT _index;
		std::string _str;
		UIWinBase* _pWin;
	};
};

/*------------------------------------------------------- UICanvas -------------------------------------------------------*/
// canvas����  ��Ϣ��ֱ��͸��
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
	// ��Ϣ����
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
	UILayoutGrid �ṩ����grid�Ĳ��ּ���

	row1: ctrl11 Interval1 ctrl12  Interval2 ctrl13 Interval3 ......
	Interval_row
	row2: ctrl21 Interval1 ctrl22  Interval2 ctrl23 Interval3 ......
	Interval_row
	......

*/
class UILayoutGrid
{
public:
	// ��ʼ��
	void InitPoint( POINT& relativePT );

	// ����cell��Ϣ
	void SetRowColumn( UINT row, UINT column, int width=100, int widthInterval=20, int height=30, int heightInterval=20 );
	void SetCell( UINT bRow, UINT bColumn, UINT eRow, UINT eColumn, UIWinBase* pWin );
	void SetCell( UINT row, UINT column, UIWinBase* pWin );

	// ���ó������
	void SetColumnWidthInterval( UINT column, int width=100, int interval=20 );
	void SetRowHeightInterval( UINT row, int heigth=30, int interval=20 );

	// ����λ����Ϣ
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