/*
	Add:	2011-5-27
	Update:	2011-6-3

	1. ֻ���С���ı����ڴ���һ�η��룩������ز���
	2. ֻ��Է�UNICODE�ı�
*/
#pragma once
#include "..\Utility\GXUtil.h"
#include   <Commdlg.h>

class TxtOBJ
{
public:
	// ������������
	TxtOBJ();
	~TxtOBJ();

	// ����ļ���С
	_LONGLONG GetFileSize();

	// �ر��ļ�
	void CloseFile();

	// ���ļ��ӿ�
	bool OpenFile( const std::string pathfileName );		// ֻ���Ѵ����ļ�
	bool CreateFile( const std::string pathfileName );		// �����ļ� �����򸲸�

	// д���ݵ��ļ�ĩβ
	void WriteToEnd( const std::string str );
	void WriteToEnd( const char* str, UINT size );

	// ��ȡ�ļ�
	void ReadAllLineWords( std::vector<VECTOR_STRING >& allLineWordsList );
	void ReadAllLineWords( std::vector<VECTOR_STRING >& allLineWordsList, const std::vector<char>& splitList );

	//
	int ReadBuf( char* pBuf, DWORD bufLen );

protected:
	// ���ļ�
	bool Open();

	// �����ļ��� ·��
	bool ParseFilePathAndName(	const std::string pathfileName, std::string& fullPathFileName, std::string& curDir, std::string& fileName );

	// ·�� �ļ���
	std::string _fullPathFileName;
	std::string _curDir;
	std::string _fileName;

	// �ļ����� ��С ����ģʽ
	std::fstream _file;
	_LONGLONG _fileSize;
	std::ios_base::openmode _mode;
};

// ����ַ��Ƿ��Ƿָ��ַ�
bool CheckSplit( const char& c , const std::vector<char>& splitList );

// ����word
void SplitWords( const std::string line, VECTOR_STRING& wordList );
void SplitWords( const std::string line, VECTOR_STRING& wordList, const VECTOR_CHAR& splitList );

// ��õ�ǰexe���ڵ�Ŀ¼
bool GetExeDirectory( std::string& path );
bool GetUpDirectory( std::string& path );
bool GetFileName( std::string path, std::string& fileName );

// ��ȡһ��Ŀ¼�µ������ļ�  ��׺��Ϊ��ѯ����  eg: suffixLine = "pdf|rar"
// �����ļ���ѡ���ѡ�񵥸��ļ�
bool OpenFolderFile( std::string& fileName );
bool SaveFolderFile( std::string& fileName );
// �����ļ���ѡ�����Ŀ¼·�� ����ѯ��Ŀ¼�������ļ�
bool ReadFolderAllFiles1( std::string suffixLine, std::string& folderPath, VECTOR_STRING& fileList );
// ͨ�������Ŀ¼·�� ��ѯ��Ŀ¼�������ļ�
bool ReadFolderAllFiles2( std::string suffixLine, std::string folderPath, VECTOR_STRING& fileList );

// �ļ���������
bool CopyFileX( std::string srcFilePath, std::string dstFilePath );