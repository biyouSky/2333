

#include "stdafx.h"
#include "ExcelControl.h"


PFunc_XLS_Type001 XLSOpenFile;
PFunc_XLS_Type002 XLSOpenSheet;
PFunc_XLS_Type003 XLSSetCellString;
PFunc_XLS_Type004 XLSClose;
PFunc_XLS_Type005 XLSGetAllCellsString;



struct XLSAutoLoad
{
	HMODULE _handle;

	XLSAutoLoad()
	{
		_handle = 0;
		_handle = ::LoadLibraryA( "Excel_IO.dll" );
		//assert( _handle!=0 );

		if( _handle!=0 )
		{
			XLSOpenFile				= ( PFunc_XLS_Type001 )GetProcAddress( _handle, "XLSOpenFile" );
			XLSOpenSheet			= ( PFunc_XLS_Type002 )GetProcAddress( _handle, "XLSOpenSheet" );
			XLSSetCellString		= ( PFunc_XLS_Type003 )GetProcAddress( _handle, "XLSSetCellString" );
			XLSClose				= ( PFunc_XLS_Type004 )GetProcAddress( _handle, "XLSClose" );
			XLSGetAllCellsString	= ( PFunc_XLS_Type005 )GetProcAddress( _handle, "XLSGetAllCellsString" );
		}
		else
		{
			::MessageBoxA( 0, "Excel_IO.dll‘ÿ»Î ß∞‹", "Warning", 0 );
		}
	}

	~XLSAutoLoad()
	{
		if( _handle!=0 ) 
			::FreeLibrary( _handle );
	}
};
XLSAutoLoad gXLSAutoLoad;
