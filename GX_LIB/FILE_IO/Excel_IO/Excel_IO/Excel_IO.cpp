// Excel_IO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Excel_IO.h"
#include "ExcelOBJ.h"

#include <vector>
#include <string>
using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	CoInitialize(NULL);

	if( XLSOpenFile( "C:\\¸üÒÂ¹ñ.xlsx" ) )
	{
		if( XLSOpenSheet( "Sheet1" ) )
		{
			vector<vector<string> > allLineWordsList;
			//XLSGetAllCellsString( &allLineWordsList );
		}
	}

	

	XLSClose();

	system("pause");
	CoUninitialize();
	return 0;
}
