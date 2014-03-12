
#include "stdafx.h"
#include "ExcelOBJ.h"


using namespace std;


ExcelOBJ* ExcelOBJ::_pSingleton = NULL;

ExcelOBJ::ExcelOBJ()
{
	::CoInitialize( NULL ); 
	//::OleInitialize(NULL);

	_lpDisp = NULL;

	//创建Excel 服务器(启动Excel)
	if( !_excelApp.CreateDispatch( _T("Excel.Application"), NULL ) )
	{
		AfxMessageBox( _T("启动Excel服务器失败!") );
		return;
	}

	_excelApp.put_Visible( FALSE );
    _excelApp.put_UserControl( FALSE );

	_books.AttachDispatch( _excelApp.get_Workbooks() );

	CString version = _excelApp.get_Version();
	if( _T("11") == version )
		_excelVersion = _T("2003");
    else if( _T("12") == version )
		_excelVersion = _T("2007");
	else if( _T("14") == version )
		_excelVersion = _T("2010");
    else
        _excelVersion = _T("其他版本");
}

ExcelOBJ::~ExcelOBJ()
{
	_sheet.ReleaseDispatch();
    _sheets.ReleaseDispatch();
    _book.ReleaseDispatch();
    _books.ReleaseDispatch();
    _excelApp.Quit();
    _excelApp.ReleaseDispatch();

	::CoUninitialize();
}

bool ExcelOBJ::OpenXLS( CString path )
{
	try
    {	 /*打开一个工作簿*/ 
        _lpDisp = _books.Open( path, 
							 vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
							 vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
							 vtMissing, vtMissing, vtMissing, vtMissing );

        _book.AttachDispatch( _lpDisp );
    }
    catch(...)
    {
		// 增加一个新的工作簿
		/* lpDisp = books.Add(vtMissing);
		book.AttachDispatch(lpDisp);*/
		return false;
    }

	_sheets.AttachDispatch( _book.get_Sheets() );
	return true;
}

bool ExcelOBJ::OpenSheet( CString sheet )
{
	try
	{	// 打开一个已有的Sheet
		_lpDisp = _sheets.get_Item( _variant_t( sheet ) );
		_sheet.AttachDispatch( _lpDisp );
	}
	catch(...)
	{
		// 创建一个新的Sheet
		/*lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
		sheet.AttachDispatch(lpDisp);
		sheet.put_Name(strSheetName);*/
		return false;
	}

	return true;
}

void ExcelOBJ::SetCellString( int row, int column, CString str )
{
	char c='A'+column-1;
	CString cellPos;
	cellPos.Format( _T("%c%d"), c, row );

	CRange range;
	_lpDisp = _sheet.get_Range( (_variant_t)(_bstr_t)cellPos, (_variant_t)(_bstr_t)cellPos );
    range.AttachDispatch( _lpDisp );
	range.put_Value2( (_variant_t)(_bstr_t)str );
	range.ReleaseDispatch();

	_book.Save();
}

void ExcelOBJ::GetRowColumnNum( int& sRow, int& rowNum, int& sColumn, int& columnNum )
{
	CRange range;
    range.AttachDispatch( _sheet.get_UsedRange(), true );

	// 起始行列从1开始
	sRow = range.get_Row();
	sColumn = range.get_Column();

	range.AttachDispatch( range.get_Rows(), true );
	rowNum = range.get_Count();

    range.AttachDispatch( range.get_Columns(), true );
    columnNum = range.get_Count();

    range.ReleaseDispatch();
}

void ExcelOBJ::GetAllCellsString( vector<vector<string> >& allLineWordsList )
{
	int sRow, rowNum, sColumn, columnNum;
	GetRowColumnNum( sRow, rowNum, sColumn, columnNum );

	// 获得行列数对应的range
	CString sCellPos, eCellPos;
	char c = 'A'+sColumn-1;
	sCellPos.Format( _T("%c%d"), c, sRow );
	c= 'A'+columnNum+sColumn-2;
	eCellPos.Format( _T("%c%d"), c, rowNum+sRow-1 );

	CRange range;
	_lpDisp = _sheet.get_Range( (_variant_t)(_bstr_t)sCellPos, (_variant_t)(_bstr_t)eCellPos );
    range.AttachDispatch( _lpDisp );
    COleSafeArray olesaRead( ( VARIANT )range.get_Value2() );

	// 获得区域内的数据
    long lFirstLBound = 0;
    long lFirstUBound = 0;
    long lSecondLBound = 0;
    long lSecondUBound = 0;
    olesaRead.GetLBound( 1, &lFirstLBound );
    olesaRead.GetUBound( 1, &lFirstUBound );
    olesaRead.GetLBound( 2, &lSecondLBound );
    olesaRead.GetUBound( 2, &lSecondUBound );
   
	for( long i=lFirstLBound; i<=lFirstUBound; i++ )
    {
		long index[2];
        index[0] = i;

		vector<string> line;
        for ( long j=lSecondLBound; j<=lSecondUBound; j++ )
        {
            index[1] = j;

			VARIANT varItem;
            olesaRead.GetElement( index, &varItem );

			CString strItem;
            switch( varItem.vt )
            {
            case VT_R8:
                {
					strItem.Format( _T("%d"), (int)varItem.dblVal );
                }
				break;
            case VT_BSTR:
                {
                    strItem = varItem.bstrVal;
                }
				break;
            case VT_I4:
                {
                    strItem.Format( _T("%ld"), (int)varItem.lVal );
                }
				break;
            default:
                {
                }
            }

			line.push_back( (const char*)strItem );
        }

		allLineWordsList.push_back( line );
    }

	range.ReleaseDispatch();
}



__declspec(dllexport) bool XLSOpenFile( const char* path )
{
	//return gExcelOBJ.OpenXLS( (_variant_t)(_bstr_t)path );
	return ExcelOBJ::GetSingleton()->OpenXLS( (_variant_t)(_bstr_t)path );
}

__declspec(dllexport) bool XLSOpenSheet( const char* sheet )
{
	//return gExcelOBJ.OpenSheet( (_variant_t)(_bstr_t)sheet );
	return ExcelOBJ::GetSingleton()->OpenSheet( (_variant_t)(_bstr_t)sheet );
}

__declspec(dllexport) void XLSSetCellString( int row, int column, const char* str )
{
	//gExcelOBJ.SetCellString( row, column, (_variant_t)(_bstr_t)str );
	ExcelOBJ::GetSingleton()->SetCellString( row, column, (_variant_t)(_bstr_t)str );
}

__declspec(dllexport) void XLSClose()
{
	ExcelOBJ::Release();
}

__declspec(dllexport) void XLSGetAllCellsString( void* pData )
{
	typedef vector<vector<string> > allLineWordsListType;
	allLineWordsListType* p = ( allLineWordsListType* )pData;
	ExcelOBJ::GetSingleton()->GetAllCellsString( *p );
}
