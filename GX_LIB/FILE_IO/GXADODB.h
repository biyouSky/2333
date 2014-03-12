/*
	Add:	2011-6-23
	Update:	2011-6-23

	ADO接口连接sql2000
*/

#pragma once
#include "..\Utility\GXUtil.h"

#ifdef _WIN32_
#import "..\DOC\msado15.dll"  no_namespace rename( "EOF", "adoEOF" )
#endif

class ADODB
{
public:
	ADODB();
	~ADODB();

	// 连接
	inline bool IsConnect();
	bool Connect( ULONG addr, std::string dbName, std::string id, std::string pwd );
	inline bool DisConnect();

	// 执行sql语句
	bool ExecuteSqlCmd( std::string sqlStr );

	// 数据库创建操作
	bool CreateDB( std::string dbName );
	bool DropDB( std::string dbName );

	// 表操作
	bool DropTab( std::string tabName );

	// 表内容操作
	bool Delete( std::string tabName, std::string whereStr );
	// 
	bool Insert( std::string tabName, std::string fieldStr, std::string valueStr );
	bool Insert( std::string tabName, std::vector<std::string> fieldStrV, std::vector<std::string> valueStrV );
	// 
	bool Update( std::string tabName, std::string fieldStr, std::string newValueStr, std::string whereStr="" );
	bool Update( std::string tabName, std::vector<std::string> fieldStrV, std::vector<std::string> newValueStrV, std::string whereStr="" );
	// 
	bool Select( std::string tabName, std::string fieldStr/*="*"*/, std::string whereStr/*=""*/, std::vector<std::vector<_variant_t> >& record );
	bool Select( std::string tabName, std::vector<std::string> fieldStrV, std::string whereStr/*=""*/, std::vector<std::vector<_variant_t> >& record );


	_RecordsetPtr _pRecordset;				// 记录集
	std::string _traceMsg;					// 回馈信息

protected:
	inline bool OPCheck();

	_ConnectionPtr _pConnection;			// 连接

	// 保持的db名、用户、密码
	std::string _dbName;
	std::string _id;
	std::string _pwd;
};

