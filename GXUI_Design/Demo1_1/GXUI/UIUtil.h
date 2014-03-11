
#pragma once

/*-------------------------------------------------- ��ܻ��� --------------------------------------------------*/
#define _0X_		1			// ����
#define _WIN32_		1			// ϵͳ
#define _MFC_		0			// ������
#define _CONSOLE_	1			// ����ģʽ


/*-------------------------------------------------- ����ͷ�ļ� --------------------------------------------------*/
// C��
#include <ctime>				// 
#include <cmath>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>				// only win����

// STL
#include <string>				// �ַ�������
#include <vector>				// ��������
#include <deque>
#include <list>
#include <algorithm>			// �㷨
#include <numeric>
#include <functional>			// �º���			
// �������
#include <iostream>
#include <fstream>

// win32
#ifdef _WIN32_
//
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

#endif


/*-------------------------------------------------- ���Ͷ��� --------------------------------------------------*/
#ifdef _UNICODE
	typedef std::wstring			tstring;
#else
	typedef std::string				tstring;
#endif


/*-------------------------------------------------- ���ߺ��� --------------------------------------------------*/

// �ַ���ת��
struct WCharToMByte			// UNICODE16 -> ���ֽ�
{   
	void operator()( std::wstring wStr, std::string &str );
};

struct MByteToWChar			// ���ֽ� -> UNICODE16
{
	void operator()( std::string str, std::wstring &wStr );
};

// ��ʽ��string ����CString fromat
struct StringFormat
{
	void operator()( std::string& str, const char* format, ... );
};
#define STRING_FORMAT	StringFormat()
