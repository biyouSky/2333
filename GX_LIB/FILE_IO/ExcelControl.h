

#pragma once

typedef bool (*PFunc_XLS_Type001)( const char* path );
typedef bool (*PFunc_XLS_Type002)( const char* sheet );
typedef void (*PFunc_XLS_Type003)( int row, int column, const char* str );
typedef void (*PFunc_XLS_Type004)();
typedef void (*PFunc_XLS_Type005)( void* pData );

extern PFunc_XLS_Type001 XLSOpenFile;
extern PFunc_XLS_Type002 XLSOpenSheet;
extern PFunc_XLS_Type003 XLSSetCellString;
extern PFunc_XLS_Type004 XLSClose;
extern PFunc_XLS_Type005 XLSGetAllCellsString;


void XLSDLLInit();