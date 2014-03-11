#include "stdafx.h"

#include "UIUtil.h"
using namespace std;


RECT GenRect::operator()( POINT point, SIZE size )
{
	RECT rc;
	rc.right = ( rc.left=point.x ) + size.cx;
	rc.bottom = ( rc.top=point.y ) + size.cy;
	return rc;
}

RECT GenRect::operator()( LONG left, LONG top, LONG right, LONG bottom )
{
	RECT rc;
	rc.left = left;
	rc.top = top;
	rc.right = right;
	rc.bottom = bottom;
	return rc;
}

POINT GenPoint::operator()( LONG x, LONG y )
{
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}

SIZE GenSize::operator()( LONG x, LONG y )
{
	SIZE s;
	s.cx = x;
	s.cy = y;
	return s;
}

int GetWidth::operator()( RECT& rc ) 
{ 
	return rc.right - rc.left; 
}

int GetHeight::operator()( RECT& rc ) 
{ 
	return rc.bottom - rc.top; 
}

bool CompareRect::operator()( RECT& r1, RECT& r2 )
{
	return ( r1.left==r2.left )&&( r1.right==r2.right )&&( r1.top==r2.top )&&( r1.bottom==r2.bottom );
}

bool CrossRect::operator()( RECT& rc1, RECT& rc2, RECT& crossRC )
{
	if( ( rc1.right<=rc2.left )||( rc2.right<=rc1.left )||( rc1.bottom<=rc2.top )||( rc2.bottom<=rc1.top ) )
		return false;

	crossRC.left = rc1.left>rc2.left ? rc1.left:rc2.left;
	crossRC.right = rc1.right<rc2.right ? rc1.right:rc2.right;
	crossRC.top = rc1.top>rc2.top ? rc1.top:rc2.top;
	crossRC.bottom = rc1.bottom<rc2.bottom ? rc1.bottom:rc2.bottom;
	return true;
}

bool ComparePoint::operator()( POINT& p1, POINT& p2 )
{
	return ( ( p1.x==p2.x ) && ( p1.y==p2.y ) );
}

bool IsPointInRect::operator()( const POINT& point, RECT& rect ) 
{ 
	return !( point.x<rect.left || point.x>rect.right || point.y<rect.top || point.y>rect.bottom ); 
}

bool IsKeyDown::operator()( int key ) 
{ 
	return GetKeyState( key )&0x80 ? true:false; 
}

UILayoutCalc::UILayoutCalc( int flag ) 
{ 
	_zoomModeflag=flag; 
}

void UILayoutCalc::SetLayoutMode( int flag ) 
{  
	_zoomModeflag=flag;  
}

void UILayoutCalc::InitLayout( RECT& parentRect, RECT& rect )
{
	_parentRect = parentRect;
	_rect = rect;
}

// 计算新状态
RECT UILayoutCalc::CalcLayout( LONG cx, LONG cy )
{
	RECT newRect = _rect;

	if( _zoomModeflag==0 )
		return newRect;

	if( _zoomModeflag&1 )
	{
		newRect.left = cx-( _parentRect.right-_rect.left );
		newRect.right = newRect.left+GetWidth()( _rect );
	}
	
	if( _zoomModeflag&2 )
	{
		newRect.top = cy-( _parentRect.bottom-_rect.top );
		newRect.bottom = newRect.top+GetHeight()( _rect );
	}

	if( _zoomModeflag&4 )
		newRect.right = cx-( _parentRect.right-_rect.right );

	if( _zoomModeflag&8 )
		newRect.bottom = cy-( _parentRect.bottom-_rect.bottom );

	if( _zoomModeflag&16 )
	{
		newRect.left = (LONG)( ( (float)_rect.left/GetWidth()( _parentRect ) )*cx );
		newRect.right = newRect.left + (LONG)( ( (float)GetWidth()( _rect )/GetWidth()( _parentRect ) )*cx );
	}

	if( _zoomModeflag&32 )
	{
		newRect.top = (LONG)( ( (float)_rect.top/GetHeight()( _parentRect ) )*cy );
		newRect.bottom = newRect.top + (LONG)( ( (float)GetHeight()( _rect )/GetHeight()( _parentRect ) )*cy );
	}

	// 防止最小化引起的负值
	if( newRect.left<0 )
		newRect.left = 0;
	if( newRect.right<0 )
		newRect.right = 0;
	if( newRect.top<0 )
		newRect.top = 0;
	if( newRect.bottom<0 )
		newRect.bottom = 0;	

	// 防止最小化引起的异常
	if( newRect.left>newRect.right )
		newRect.left = newRect.right;
	if( newRect.top>newRect.bottom )
		newRect.top = newRect.bottom;

	return newRect;
}

void StringPasteFromClipboard::operator()( std::wstring &strBuf, HWND dstHwnd )
{
	#ifndef _WIN32_
		throw 0;
	#endif

	// 打开剪贴板
	if ( OpenClipboard( dstHwnd )==FALSE ) return;
		
	HANDLE hData = GetClipboardData( CF_UNICODETEXT );

	wchar_t* clipBuf = (wchar_t*)GlobalLock( hData );
	strBuf = clipBuf;

	GlobalUnlock( hData );
	CloseClipboard();
}

void StringCopyToClipboard::operator()( std::wstring &strBuf, HWND srcHwnd )
{
	#ifndef _WIN32_
		throw 0;
	#endif

	if( OpenClipboard( srcHwnd )==FALSE ) return;
	
	HGLOBAL clipHandle;
	wchar_t* clipBuf;
	EmptyClipboard();

	clipHandle = GlobalAlloc( GMEM_DDESHARE, 2*strBuf.size()+2 );
	clipBuf = ( wchar_t* )GlobalLock( clipHandle );
		
	wmemcpy( clipBuf, strBuf.c_str(), strBuf.size() );
	clipBuf[strBuf.size()] = _T( '\0' );

	GlobalUnlock( clipHandle );
	SetClipboardData( CF_UNICODETEXT,clipHandle );

	CloseClipboard();
}














void WCharToMByte::operator()( wstring wStr, string &str )
{
#ifndef _WIN32_
	throw 0;
#endif

	DWORD dwNum = WideCharToMultiByte( CP_OEMCP, NULL, wStr.c_str(), -1, NULL, 0, NULL, FALSE );
	char *psText;
	psText = new char[dwNum];

	WideCharToMultiByte ( CP_OEMCP, NULL, wStr.c_str() ,-1, psText, dwNum, NULL, FALSE );
	str = psText;
	delete []psText;
}

void MByteToWChar::operator()( string str, wstring &wStr )
{
#ifndef _WIN32_
	throw 0;
#endif

	DWORD dwNum = MultiByteToWideChar ( CP_ACP, 0, str.c_str(), -1, NULL, 0 );
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];

	MultiByteToWideChar ( CP_ACP, 0, str.c_str(), -1, pwText, dwNum );
	wStr = pwText;
	delete []pwText;
}


void StringFormat::operator()( string& str, const char* format, ... )
{
	// 最大缓存1024
	enum { MAX_SIZE = 1024 };
	static char buf[MAX_SIZE];
	memset( buf, 0, MAX_SIZE );
	
	// 格式化 
	va_list vptr; 
	va_start( vptr, format ); 
	vsprintf( buf, format, vptr );
	va_end( vptr );

	str = buf;
}



bool CheckSplit( const char& c , const std::vector<char>& splitList )
{
	for( UINT i=0; i<splitList.size(); ++i )
	{
		if( c==splitList[i] )
			return true;
	}
	return false;
}

void SplitWords( const string line, VECTOR_STRING& wordList, const VECTOR_CHAR& splitList )
{
	int strLen = line.size();
	int beginPos = 0;
	int endPos = 0;

	if( strLen == 0 )
		return;

	do
	{
		// 略过分隔字符  找到单词的起点
		while( CheckSplit( line[endPos], splitList ) || line[endPos]==0x0A )
		{
			++endPos;
			if( endPos==strLen )
				return;
		}
		beginPos = endPos;
		
		// 略过单词找到单词的终点 
		while( !CheckSplit( line[endPos], splitList ) && line[endPos]!=0x0A )
		{
			++endPos;
			if( endPos==strLen )
				break;		
		}
		// 记录单词
		string word;
		for( int i=beginPos; i<endPos; ++i )
			word += line[i];
		wordList.push_back( word );
		// wordList.push_back( string( line.begin()+beginPos, line.begin()+endPos ) );  // 效率低
	}
	while( endPos<strLen );
}

bool GetExeDirectory( string& path )			
{
#ifndef _WIN32_
	return false;
#endif

	enum
	{
		MAX_SIZE= 256
	};

	char curdir[MAX_SIZE];
	if( GetModuleFileNameA( 0, curdir, MAX_SIZE )==0 ) return false;

	path = curdir;

	int index = path.rfind( '\\' );
	path = path.substr( 0, index );

	return true;
}

bool GetUpDirectory( std::string& path )
{
	int index = path.rfind( '\\' );
	if( index==-1 ) return false;
	path = path.substr( 0, index );

	return true;
}

bool ReadFolderAllFiles2( std::string suffixLine, std::string folderPath, VECTOR_STRING& fileList )
{
#ifndef _WIN32_
	return false;
#endif

	struct SuffixFilter
	{
		bool operator()( const string& fileName, VECTOR_STRING& suffixWordList )
		{
			VECTOR_STRING::iterator it = find( suffixWordList.begin(), suffixWordList.end(), "*" );
			if( it!=suffixWordList.end() )
				return true;

			for( UINT i=0; i<suffixWordList.size(); ++i )
			{
				UINT subLen = suffixWordList[i].size()+1;
				if( fileName.substr( fileName.size()-subLen, subLen )==string( "."+suffixWordList[i] ) )
					return true;
			}
			
			return false;
		}
	};

	// 后缀处理 获得要过滤得后缀名
	vector<char> splitList;
	splitList.push_back( ' ' );
	splitList.push_back( '|' );
	VECTOR_STRING suffixWordList;
	//
	SplitWords( suffixLine, suffixWordList, splitList );

	// 
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind;
	hFind = FindFirstFileA( string( folderPath+("\\*.*") ).c_str(), &findFileData );
	while( hFind != INVALID_HANDLE_VALUE )
	{
		// 判断不是文件夹
		if( (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
		{
			string fileName = findFileData.cFileName;

			if( SuffixFilter()( fileName, suffixWordList )==true )
				fileList.push_back( fileName );
		}

		// 寻找下一个文件夹
		if( FindNextFileA( hFind, &findFileData )==false )
			break;
	}

	FindClose( hFind );
	return true;
}
