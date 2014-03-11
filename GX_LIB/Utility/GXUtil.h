/*
	Add:	2011-5-26
	Update:	2011-9-23

*/


/*-------------------------------------------------- ��ܻ��� --------------------------------------------------*/
#pragma once

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

typedef std::vector<bool>			VECTOR_BOOL;
typedef std::vector<char>			VECTOR_CHAR;
typedef std::vector<UCHAR>			VECTOR_UCHAR;
typedef std::vector<double>			VECTOR_DOUBLE;
typedef std::vector<float>			VECTOR_FLOAT;
typedef std::vector<int>			VECTOR_INT;
typedef std::vector<unsigned int>	VECTOR_UINT;
typedef std::vector<unsigned long>	VECTOR_ULONG;
typedef std::vector<std::string>	VECTOR_STRING;

typedef std::deque<char>			DEQUE_CHAR;
typedef std::deque<double>			DEQUE_DOUBLE;
typedef std::deque<float>			DEQUE_FLOAT;
typedef std::deque<int>				DEQUE_INT;
typedef std::deque<std::string>		DEQUE_STRING;


/*-------------------------------------------------- ���ߺ��� --------------------------------------------------*/
// ʱ��ȴ�����
#define BEGIN_WAIT()						DWORD beginTick = ::GetTickCount(); 
#define END_WAIT( iWate )				{	DWORD endTick = ::GetTickCount(); \
											DWORD dxTick = abs( (int)( endTick-beginTick ) ); \
											if( dxTick<iWate ) \
												Sleep( iWate-dxTick ); \
										}
#define END_WAIT_FLAG( iWate, flag )	{	while( 1 ) \
											{ \
												if( flag==true ) \
													break; \
												DWORD endTick = ::GetTickCount(); \
												DWORD dxTick = abs( ( int )( endTick-beginTick ) ); \
												if( dxTick<iWate ) \
													Sleep( 10 ); \
												else \
													break; \
											} \
										}
#define BREAK_WAIT( iWate, flag )		{	DWORD endTick = ::GetTickCount(); \
											DWORD dxTick = abs( (int)( endTick-beginTick ) ); \
											flag = dxTick<iWate ? false:true; \
										}

#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
			TypeName(const TypeName&);		\
			void operator=(const TypeName&) 

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

// ip4��ַ���ַ������໥ת��
struct IP4_StrToULONG
{
	unsigned long operator()( const char* ipStr );
};

struct IP4_ULONGToStr
{
	char* operator()( const unsigned long ip );
};

// ʱ���ȡ��
class TimeFunc
{
public:
	TimeFunc();

	std::string GetString();
	void GetCurTime();
	void SetDate( int year, int month, int monthday );

	void GetTimeSpan( VECTOR_STRING& timeSpan );

	inline int GetYear()		{ return _tm.tm_year+1900;	}
	inline int GetMonth()		{ return _tm.tm_mon+1;		}
	inline int GetMonthDay()	{ return _tm.tm_mday;			}
	inline int GetHour()		{ return _tm.tm_hour;			}
	inline int GetMinute()		{ return _tm.tm_min;			}
	inline int GetSecond()		{ return _tm.tm_sec;			}
	inline int GetYearDay()		{ return _tm.tm_yday+1;		}

protected:
	struct tm _tm;

//private:
//	DISALLOW_COPY_AND_ASSIGN( TimeFunc );
};

/*-------------------------------------------------- ����֧�� --------------------------------------------------*/
// ��ȡϵͳ������Ϣ����
struct GetSysErrorMsg
{
#ifdef _WIN32_
	std::string operator()( int errCode );
#endif
};









// ���·��ն�̬���ԣ��ǳ�Σ��


// ��̬���ͱ���
/*
 eg:
	Var var(3.5f);
	float v = var;		// right
	int vi = var;		// wrong, throw 0

	һ������ͨ�����ͱ����Ĵ������, ��Э��Ľ��մ�����
	����ʹ��������ǰԤ֪ʹ�õ���ʲô���Ͳ�������ȷ��ת��
	�����ʹ�þ��кܴ�Σ���ԣ���
*/
struct Var
{
	// ������Ϣ
	std::string _type;

	// ��������ֵ
	union
	{
		char cVal;
		UCHAR ucVal;
		USHORT usVal;
		ULONG ulVal;
		int iVal;
		float fVal;
		double dbVal;
		bool blVal;
		// 
		void* _pVoidVal;
	} value;
	// �ַ���ֵ
	std::string strVal;

	// �Զ���������ת��
	operator char() const			{ if( _type!="char" ) throw 0; return value.cVal;		}
	operator UCHAR() const			{ if( _type!="UCHAR" ) throw 0; return value.ucVal;		}
	operator USHORT() const			{ if( _type!="USHORT" ) throw 0; return value.usVal;	}
	operator ULONG() const			{ if( _type!="ULONG" ) throw 0; return value.ulVal;		}
	operator int() const			{ if( _type!="int" ) throw 0; return value.iVal;		}
	operator float() const			{ if( _type!="float" ) throw 0; return value.fVal;		}
	operator double() const			{ if( _type!="double" ) throw 0; return value.dbVal;	}
	operator bool() const			{ if( _type!="bool" ) throw 0; return value.blVal;		}
	operator std::string() const	{ if( _type!="string" ) throw 0; return strVal;			}
	operator void*() const			{ if( _type!="void*" ) throw 0; return value._pVoidVal;	}
#if _MFC_
	operator CString() const 
	{
		if( _type!="string" ) throw 0;
	#ifdef _UNICODE
		wstring wStr;
		MByteToWChar()( strVal, wStr );
		return wStr.c_str();
	#else
		return strVal.c_str();  
	#endif	
	}
#endif

	// ������ʼ��
	Var( char t )			{ value.cVal = t; _type="char";		}
	Var( UCHAR t )			{ value.ucVal = t; _type="UCHAR";	}
	Var( USHORT t )			{ value.usVal = t; _type="USHORT";	}
	Var( ULONG t )			{ value.ulVal = t; _type="ULONG";	}
	Var( int t )			{ value.iVal = t; _type="int";		}
	Var( float t )			{ value.fVal = t; _type="float";	}
	Var( double t )			{ value.dbVal = t; _type="double";	}
	Var( bool t )			{ value.blVal = t; _type="bool";	}
	Var( std::string t )	{ strVal = t; _type="string";		}
	Var( char* t )			{ strVal = t; _type="string";		}
#if _MFC_
	Var( CString& t ) 
	{ 
	#ifdef _UNICODE
		WCharToMByte()( (LPCWSTR)t, strVal );
	#else
		strVal = t; 
	#endif
		_type="string"; 
	}
#endif
	Var( void* t )			{ value._pVoidVal = t; _type="void*"; }
};

inline bool operator==( Var& v1, Var& v2 )
{
	if( v1._type!=v2._type )
		return false;

	bool rt = false;
	if( v1._type=="UCHAR" )
		rt = ( UCHAR )v1==( UCHAR )v2;
	else if( v1._type=="USHORT" )
		rt = (USHORT)v1==(USHORT)v2;
	else if( v1._type=="int" )
		rt = ( int )v1==( int )v2;
	else if( v1._type=="float" )
		rt = (float)v1==(float)v2;
	else if( v1._type=="double" )
		rt = ( double )v1==( double )v2;
	else if( v1._type=="string" )
		rt = ( std::string )v1==( std::string )v2;

	return rt;
}
inline bool operator!=( Var& v1, Var& v2 )
{
	return !( operator==( v1, v2 ) );
}

// �Զ��ж�ֵ�Ƿ���ڵı���
/*
 eg:
	VarJ<float> varJ( 3.5f );
	if( varJ )
		printf( "%f", varJ );

	ע�⣺Tһ��Ϊint float double, һ������Ϊbool����
*/
template<class T>
struct VarJ
{
	T _v;
	bool _flag;

	VarJ()
	{
		_v = 0;
		_flag = false;
	}
	VarJ( T t )
	{
		_v = t;
		_flag = true;
	}

	void operator =( T t )
	{
		_v = t;
		_flag = true;
	}

	// ��ʾת���ж�
	operator bool() { return _flag; }

	// ��ʾת��ֵ
	operator T() 
	{ 
		if( _flag==false ) throw;
		return _v;
	}	
};

// �Զ��ж�����List
/*
	һ������ָ�����ݴ��� ĳЩָ�겻���ڵ����
*/
template<class T>
struct VarJList
{
	// ����list
	std::vector<VarJ<T> > _varJList;

public:
	VarJList() {}

	// ��list�ӿ�
	void Push_Back( T t )				{ _varJList.push_back( VarJ<T>( t ) );  }
	void Push_NoValue()					{ _varJList.push_back( VarJ<T>() );		}
	UINT Size()							{ return _varJList.size();				}
	void Clear()						{ _varJList.clear();					}
	//
	VarJ<T>& operator[]( UINT index )	{ return _varJList[index];				}


	// ������� ��С ƽ��ֵ
	bool CalcMaxMinAve( T& min, T& max, T& ave ) 
	{
		vector<T> vList;
		for( UINT i=0; i<_varJList.size(); ++i )
		{
			if( _varJList[i] )
				vList.push_back( _varJList[i] );
		}

		if( vList.size()==0 )
		{
			return false;
		}
		{
			min = *min_element( vList.begin(), vList.end() );
			max = *max_element( vList.begin(), vList.end() );
			ave = static_cast<T>( accumulate( vList.begin(), vList.end(), 0.0 )/vList.size() );
			return true;
		}
	}
};