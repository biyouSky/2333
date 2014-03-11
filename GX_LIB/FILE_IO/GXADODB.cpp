#include "stdafx.h"
#include "GXADODB.h"
using namespace std;

ADODB::ADODB()
{
	::CoInitialize( NULL );

	_pConnection.CreateInstance( "ADODB.Connection" );
	_pRecordset.CreateInstance( "ADODB.Recordset" );
}

ADODB::~ADODB()
{
	_pConnection.Release();
	_pRecordset.Release();

	//::CoUninitialize();
}

inline bool ADODB::IsConnect()
{
	return _pConnection->State!=0 ? true:false;
}

bool ADODB::Connect( ULONG addr, string dbName, string id, string pwd )
{
	if( IsConnect()==true )
		DisConnect();

	// ��ȡ��������ַ( ��ʱΪ������ )
	in_addr ipAddr;
	ipAddr.S_un.S_addr = htonl( addr );	

	// �������
	string strSRC;
	STRING_FORMAT( strSRC, "Driver=SQL Server;Server=%s;Database=%s;UID=%s;PWD=%s", inet_ntoa( ipAddr ), dbName.c_str(), id.c_str(), pwd.c_str() );

	// ���Ӳ���
	try
	{
		_pConnection->Open( (LPCSTR)strSRC.c_str(), "", "", adModeUnknown );
	}
	catch( _com_error& e )
	{
		_traceMsg = "�������ݿ�ʧ�� ������Ϣ:";
		_traceMsg += (LPCSTR)e.Description();
		return false;
	}

	// ������Ϣ
	_dbName = dbName;
	_id = id;
	_pwd = pwd;

	STRING_FORMAT( _traceMsg, "�������ݿ�%s�ɹ�..", (LPCSTR)_pConnection->GetDefaultDatabase() );
	return true;
}

inline bool ADODB::DisConnect()
{
	if( _pConnection->State )    
		_pConnection->Close();

	_traceMsg = "�Ͽ����ݿ�ɹ�..";
	return true;
}

inline bool ADODB::OPCheck()
{
	if( IsConnect()==false )
	{
		_traceMsg = "���ݿ�δ����..";
		return false;
	}
	return true;
}

bool ADODB::ExecuteSqlCmd( string sqlCmd )
{
	// sql�����
	if( sqlCmd=="" )
	{
		_traceMsg = "sql���Ϊ��..";
		return false;
	}

	// ���Ӽ��
	if( OPCheck()==false )
		return false;

	try 
	{
		// ��¼��ֻ�Ǹ����� ����������Ա����κ�Ӱ��
		_variant_t recordsAffected;
		_pRecordset = _pConnection->Execute( (LPCSTR)sqlCmd.c_str(), &recordsAffected, adCmdText );
	}
	catch( _com_error& e )
	{
		_traceMsg = "ִ��sqlʧ�ܣ�ԭ��";
		_traceMsg += (LPCSTR)e.Description();
		return false;
	}
	
	_traceMsg = "ִ��sql���ɹ�..";
	return true;
}

bool ADODB::CreateDB( string dbName )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "create database %s", dbName.c_str() );

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::DropDB( string dbName )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "drop database %s", dbName.c_str() );

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::DropTab( string tabName )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "Drop tab %s", tabName.c_str() );

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::Delete( string tabName, string whereStr )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "Delete From %s", tabName.c_str() );
	if( whereStr!="" )
	{
		sqlCmd += " Where ";
		sqlCmd += whereStr;
	}

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::Insert( string tabName, string fieldStr, string valueStr )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "Insert Into %s(%s) Values(%s)", tabName.c_str(), fieldStr.c_str(), valueStr.c_str() );

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::Insert( string tabName, vector<string> fieldStrV, vector<string> valueStrV )
{
	assert( fieldStrV.size()==valueStrV.size() );

	string fieldStr, valueStr;
	fieldStr = fieldStrV[0];
	valueStr = valueStrV[0];
	for( UINT i=1; i<fieldStrV.size(); ++i )
	{
		fieldStr += ",";
		fieldStr += fieldStrV[i];
		// 
		valueStr += ",";
		valueStr += valueStrV[i];
	}

	return Insert( tabName, fieldStr, valueStr );
}

bool ADODB::Update( string tabName, string fieldStr, string newValueStr, string whereStr )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "Update %s Set %s = %s", tabName.c_str(), fieldStr.c_str(), newValueStr.c_str() );
	if( whereStr!="" )
	{
		sqlCmd += " where ";
		sqlCmd += whereStr;
	}

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::Update( string tabName, vector<string> fieldStrV, vector<string> newValueStrV, string whereStr )
{
	if( fieldStrV.size()!=newValueStrV.size() )
		return false;

	string sqlCmd;
	STRING_FORMAT( sqlCmd, "Update %s Set %s = %s", tabName.c_str(), fieldStrV[0].c_str(), newValueStrV[0].c_str() );
	for( UINT i=1; i<fieldStrV.size(); ++i )
	{
		string str;
		STRING_FORMAT( str, ", %s=%s", fieldStrV[i].c_str(), newValueStrV[i].c_str() );
		sqlCmd += str;
	}

	if( whereStr!="" )
	{
		sqlCmd += " where ";
		sqlCmd += whereStr;
	}

	return ExecuteSqlCmd( sqlCmd );
}

bool ADODB::Select( string tabName, string fieldStr, string whereStr, vector<vector<_variant_t> >& record )
{
	string sqlCmd;
	STRING_FORMAT( sqlCmd, "Select %s From %s", fieldStr.c_str(), tabName.c_str() );
	if( whereStr!="" )
	{
		sqlCmd += " Where ";
		sqlCmd += whereStr;
	}

	if( ExecuteSqlCmd( sqlCmd )==false )
		return false;

	int index = 0;
	while( !_pRecordset->adoEOF )
	{
		record.resize( record.size()+1 );
		int columnNum = _pRecordset->GetFields()->Count;
		record[index].resize( columnNum );

		for( long i=0; i<columnNum; ++i )
			record[index][i] = _pRecordset->GetFields()->GetItem( i )->Value;

		++index;
		_pRecordset->MoveNext();
	}

	return true;
}

bool ADODB::Select( string tabName, vector<string> fieldStrV, string whereStr, vector<vector<_variant_t> >& record )
{
	string fieldStr;
	fieldStr = fieldStrV[0];
	for( UINT i=1; i<fieldStrV.size(); ++i )
	{
		fieldStr += ",";
		fieldStr += fieldStrV[i];
	}

	return Select( tabName, fieldStr, whereStr, record );
}

