#include "stdafx.h"
#include "GXTXT.h"
using namespace std;

TxtOBJ::TxtOBJ()
{
	_fileSize = 0;
}

TxtOBJ::~TxtOBJ()
{
	CloseFile();
}

void TxtOBJ::CloseFile()
{
	_fileSize = 0;
	_fullPathFileName.clear();
	_curDir.clear();
	_fileName.clear();

	if( _file.is_open() )
		_file.close();
}

_LONGLONG TxtOBJ::GetFileSize()
{
	if( !_file.is_open() )
		return 0;

	_file.seekg( 0, ios::end ); 
	_fileSize = _file.tellg();
	_file.seekg( 0, ios::beg ); 
	return _fileSize;
}

bool TxtOBJ::ParseFilePathAndName(	const string pathfileName, string& fullPathFileName, string& curDir, string& fileName )
{
	// 判定是不是完整路径
	bool fullPathFlag = false;
	if( pathfileName.size()>3 )
	{
		if( pathfileName[1]==':' || pathfileName[1]=='\\' )
			fullPathFlag = true;
	}

	// 解析路径 文件名
	if( fullPathFlag )
	{
		int index = pathfileName.rfind( '\\' );
		if( index==-1 )
			return false;

		fullPathFileName = pathfileName;
		curDir = pathfileName.substr( 0, index );
		fileName = pathfileName.substr( index+1, pathfileName.size()-1 );
	}
	else
	{
		GetExeDirectory( curDir );
		fileName = pathfileName;
		fullPathFileName = curDir + "\\" + fileName;
	}
	return true;
}

bool TxtOBJ::Open()
{
#if _0X_
	_file.open( _fullPathFileName.c_str(), _mode );
#else
	wstring wStr;
	MByteToWChar()( _fullPathFileName, wStr );
	_file.open( wStr.c_str(), _mode );
#endif

	return !_file ? false:true;
}

bool TxtOBJ::OpenFile( const string pathfileName )
{
	string fullPathFileName, curDir, fileName;
	ParseFilePathAndName( pathfileName, fullPathFileName, curDir, fileName );

	// 已经打开该文件 则返回
	if( _file.is_open() && fullPathFileName==_fullPathFileName )
	{
		return true;
	}
	else
	{	// 关闭该文件并打开指定文件
		CloseFile();
		_fullPathFileName = fullPathFileName;
		_curDir = curDir;
		_fileName = fileName;
	}

	_mode = ios::in | ios::out | ios::_Nocreate | ios::binary;
	return Open();
}

bool TxtOBJ::CreateFile( const string pathfileName )
{
	CloseFile();
	ParseFilePathAndName( pathfileName, _fullPathFileName, _curDir, _fileName );

	_mode = ios::in | ios::out | ios::trunc | ios::binary;
	return Open();
}

void TxtOBJ::WriteToEnd( const string str )
{
	// 设置游标到初始位置
	_file.seekg( 0, ios::end );
	_file.seekp( 0, ios::end );

	_file.write( str.c_str(), str.size() );
}

void TxtOBJ::WriteToEnd( const char* str, UINT size )
{
	// 设置游标到初始位置
	_file.seekg( 0, ios::end );
	_file.seekp( 0, ios::end );

	_file.write( str, size );
}

int TxtOBJ::ReadBuf( char* pBuf, DWORD bufLen )
{
	// 检查文件是否打开
	if( !_file.is_open() )
		return -1;

	memset( pBuf, 0, bufLen );
	
	// 读取整个文本数据
	_file.read( pBuf, bufLen );
	return (int)_file.gcount();
}

void TxtOBJ::ReadAllLineWords( vector<VECTOR_STRING >& allLineWordsList )
{
	vector<char> splitList;
	splitList.push_back( ' ' );
	splitList.push_back( '	' );
	splitList.push_back( 0x0D );
	splitList.push_back( ',' );
	ReadAllLineWords( allLineWordsList, splitList );
}

void TxtOBJ::ReadAllLineWords( vector<VECTOR_STRING >& allLineWordsList, const vector<char>& splitList )
{
	// 检查文件是否打开
	if( !_file.is_open() )
		return;

	// 获得文件大小
	GetFileSize();
	// 检查是否超UINT范围
	if( _fileSize<=0 )
		return;

	// 设置游标到初始位置
	_file.seekg( 0, ios::beg );
	_file.seekp( 0, ios::beg );

	// 分配缓存 
	string pBuf;
	pBuf.resize( (UINT)_fileSize+1 );
	
	// 读取整个文本数据
	_file.read( &pBuf[0], _fileSize );
	UINT readBufLen = (UINT)_file.gcount();
	if( readBufLen==0 )
		return;

	// 补充回车换行
	if( pBuf[readBufLen-1]!=0x0A )
	{
		++readBufLen;
		pBuf[readBufLen-1] = 0x0A;
	}

	// 寻找回车换行
	UINT byteOffset = 0;
	VECTOR_STRING wordList;
	UINT wordBeginOffset = 0;
	bool wordBeginFlag = true;
	while( byteOffset<readBufLen )
	{
		if( pBuf[byteOffset]==0x0A ) // 找到回车换行
		{
			// 保存最后个word
			if( wordBeginFlag==false )
			{	// 记录单词
				string word;
				for( UINT i=wordBeginOffset; i<byteOffset; ++i )
					word += pBuf[i];
				wordList.push_back( word );
			}
			// 保存当前行的word list
			allLineWordsList.push_back( wordList );
			wordList.clear();
			wordBeginFlag = true;
		}
		else if( CheckSplit( pBuf[byteOffset], splitList )==true )  // 找到分隔字符
		{
			if( wordBeginFlag==true ) // 略过分隔字符 寻找word的起始
			{
				++wordBeginOffset;
			}
			else  // 一个word的结束
			{
				// 记录单词
				string word;
				for( UINT i=wordBeginOffset; i<byteOffset; ++i )
					word += pBuf[i];
				wordList.push_back( word );

				// 
				wordBeginOffset = byteOffset+1;
				wordBeginFlag = true;
			}
		}
		else // 找到字符
		{
			if( wordBeginFlag==true ) // 略过分隔字符 寻找word的起始
			{
				wordBeginOffset = byteOffset;
				wordBeginFlag = false;
			}
		}

		// 判断下一字符
		++byteOffset;
	}
}






bool CheckSplit( const char& c , const vector<char>& splitList )
{
	for( UINT i=0; i<splitList.size(); ++i )
	{
		if( c==splitList[i] )
			return true;
	}
	return false;
}

void SplitWords( const string line, VECTOR_STRING& wordList )
{
	vector<char> splitList;
	splitList.push_back( ' ' );
	splitList.push_back( '	' );
	splitList.push_back( 0x0D );
	//
	SplitWords( line, wordList, splitList );
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

bool GetFileName( std::string path, std::string& fileName )
{
	int index = path.rfind( '\\' );
	if( index==-1 ) return false;
	fileName = path.substr( index+1, path.size()+1 );
	return true;
}

bool OpenFolderFile( std::string& fileName )
{
	// 选取文件
	char szFile[512];				// buffer for file name
	OPENFILENAMEA ofn;				// common dialog box structure
	//
	ZeroMemory( &ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All Files(*.*)\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 
	if( GetOpenFileNameA( &ofn )==FALSE )
		return false;

	fileName = szFile;
	return true;
}

bool SaveFolderFile( std::string& fileName )
{
	// 选取文件
	char szFile[512];				// buffer for file name
	OPENFILENAMEA ofn;				// common dialog box structure
	//
	ZeroMemory( &ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All Files(*.*)\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 
	if( GetSaveFileNameA( &ofn )==FALSE )
		return false;

	fileName = szFile;
	return true;
}

bool ReadFolderAllFiles1( string suffixLine, std::string& folderPath, VECTOR_STRING& fileList )
{
#ifndef _WIN32_
	return false;
#endif

	// 选取文件
	char szFile[512];				// buffer for file name
	OPENFILENAMEA ofn;				// common dialog box structure
	//
	ZeroMemory( &ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All Files(*.*)\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 
	if( GetOpenFileNameA( &ofn )==FALSE )
		return false;

	// 获得文件夹路径
	string fileName = szFile;
	int index = fileName.rfind( '\\' );
	if( index==-1 )
		return false;
	folderPath = fileName.substr( 0, index );
	
	return ReadFolderAllFiles2( suffixLine, folderPath, fileList );
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



bool CopyFileX( std::string srcFilePath, std::string dstFilePath )
{
	ofstream outfile;//创建文件 
    outfile.open( dstFilePath, ios::out|ios::binary );     
	if( !outfile.is_open() )
		return false;

    ifstream infile;//读入待复制文件 
    infile.open( srcFilePath,ios::in|ios::binary ); 
	if( !infile.is_open() )
		return false;

	char buf[1024];
	while( !infile.eof( ))
	{
		infile.read( buf, 1024 );
		outfile.write( buf, infile.gcount() );
	}
    
	infile.close(); 
    outfile.close(); 
	return true;
}