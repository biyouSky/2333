/*
	Add:	2011-5-27
	Update:	2011-6-3

	1. 只针对小型文本（内存能一次放入）进行相关操作
	2. 只针对非UNICODE文本
*/
#pragma once
#include "..\Utility\GXUtil.h"
#include   <Commdlg.h>

class TxtOBJ
{
public:
	// 构造析构函数
	TxtOBJ();
	~TxtOBJ();

	// 获得文件大小
	_LONGLONG GetFileSize();

	// 关闭文件
	void CloseFile();

	// 打开文件接口
	bool OpenFile( const std::string pathfileName );		// 只打开已存在文件
	bool CreateFile( const std::string pathfileName );		// 创建文件 存在则覆盖

	// 写数据到文件末尾
	void WriteToEnd( const std::string str );
	void WriteToEnd( const char* str, UINT size );

	// 读取文件
	void ReadAllLineWords( std::vector<VECTOR_STRING >& allLineWordsList );
	void ReadAllLineWords( std::vector<VECTOR_STRING >& allLineWordsList, const std::vector<char>& splitList );

	//
	int ReadBuf( char* pBuf, DWORD bufLen );

protected:
	// 打开文件
	bool Open();

	// 解析文件名 路径
	bool ParseFilePathAndName(	const std::string pathfileName, std::string& fullPathFileName, std::string& curDir, std::string& fileName );

	// 路径 文件名
	std::string _fullPathFileName;
	std::string _curDir;
	std::string _fileName;

	// 文件对象 大小 操作模式
	std::fstream _file;
	_LONGLONG _fileSize;
	std::ios_base::openmode _mode;
};

// 检查字符是否是分隔字符
bool CheckSplit( const char& c , const std::vector<char>& splitList );

// 解析word
void SplitWords( const std::string line, VECTOR_STRING& wordList );
void SplitWords( const std::string line, VECTOR_STRING& wordList, const VECTOR_CHAR& splitList );

// 获得当前exe所在的目录
bool GetExeDirectory( std::string& path );
bool GetUpDirectory( std::string& path );
bool GetFileName( std::string path, std::string& fileName );

// 读取一个目录下的所有文件  后缀名为查询条件  eg: suffixLine = "pdf|rar"
// 调用文件夹选择框选择单个文件
bool OpenFolderFile( std::string& fileName );
bool SaveFolderFile( std::string& fileName );
// 调用文件夹选择框获得目录路径 并查询该目录下所有文件
bool ReadFolderAllFiles1( std::string suffixLine, std::string& folderPath, VECTOR_STRING& fileList );
// 通过传入的目录路径 查询该目录下所有文件
bool ReadFolderAllFiles2( std::string suffixLine, std::string folderPath, VECTOR_STRING& fileList );

// 文件拷贝功能
bool CopyFileX( std::string srcFilePath, std::string dstFilePath );