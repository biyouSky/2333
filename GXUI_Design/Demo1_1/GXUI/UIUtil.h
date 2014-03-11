
#pragma once

/*-------------------------------------------------- 框架环境 --------------------------------------------------*/
#define _0X_		1			// 语言
#define _WIN32_		1			// 系统
#define _MFC_		0			// 软件框架
#define _CONSOLE_	1			// 运行模式


/*-------------------------------------------------- 包含头文件 --------------------------------------------------*/
// C库
#include <ctime>				// 
#include <cmath>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>				// only win？？

// STL
#include <string>				// 字符串处理
#include <vector>				// 数据容器
#include <deque>
#include <list>
#include <algorithm>			// 算法
#include <numeric>
#include <functional>			// 仿函数			
// 输入输出
#include <iostream>
#include <fstream>

// win32
#ifdef _WIN32_
//
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )

#endif


/*-------------------------------------------------- 类型定义 --------------------------------------------------*/
#ifdef _UNICODE
	typedef std::wstring			tstring;
#else
	typedef std::string				tstring;
#endif


/*-------------------------------------------------- 工具函数 --------------------------------------------------*/

// 字符串转换
struct WCharToMByte			// UNICODE16 -> 多字节
{   
	void operator()( std::wstring wStr, std::string &str );
};

struct MByteToWChar			// 多字节 -> UNICODE16
{
	void operator()( std::string str, std::wstring &wStr );
};

// 格式化string 类似CString fromat
struct StringFormat
{
	void operator()( std::string& str, const char* format, ... );
};
#define STRING_FORMAT	StringFormat()
