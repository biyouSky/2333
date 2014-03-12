
#pragma once

#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorkbook.h"
#include "CWorksheets.h"
#include "CWorksheet.h"
#include "CRange.h"

#include <vector>
#include <string>

class ExcelOBJ
{
public:
	ExcelOBJ();
	~ExcelOBJ();

	bool OpenXLS( CString path );
	bool OpenSheet( CString sheet );

	// 设置相关
	void SetCellString( int row, int column, CString str );
	
	// 读取相关
	void GetRowColumnNum( int& sRow, int& rowNum, int& sColumn, int& columnNum );
	void GetAllCellsString( std::vector<std::vector<std::string> >& allLineWordsList );


	static ExcelOBJ* GetSingleton()
	{
		if ( _pSingleton==NULL ) 
			_pSingleton = new ExcelOBJ();

		return _pSingleton;
	}

	static void Release()
	{
		if ( _pSingleton!=NULL ) 
		{
			delete _pSingleton;
			_pSingleton = NULL;
		}
	}

private:
	static ExcelOBJ* _pSingleton;
	CApplication _excelApp;
    CWorkbooks _books;
    CWorkbook _book;
    CWorksheets _sheets;
    CWorksheet _sheet;
    LPDISPATCH _lpDisp;

	CString _excelVersion;
};


extern "C" __declspec(dllexport) bool XLSOpenFile( const char* path );
extern "C" __declspec(dllexport) bool XLSOpenSheet( const char* sheet );
extern "C" __declspec(dllexport) void XLSSetCellString( int row, int column, const char* str );
extern "C" __declspec(dllexport) void XLSClose();
extern "C" __declspec(dllexport) void XLSGetAllCellsString( void* pData );