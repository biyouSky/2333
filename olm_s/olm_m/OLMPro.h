
#pragma once


typedef void (*PFunc_View_Type0)( const char* msg );
typedef void (*PFunc_View_Type1)( void* pDev ); 
typedef void (*PFunc_View_Type2)( USHORT* pList, int num ); 
typedef void (*PFunc_View_Type3)( UCHAR oswType, USHORT temp, USHORT inportNum, USHORT outportNum, USHORT inportNo, USHORT outportNo ); 
typedef void (*PFunc_View_Type4)( UCHAR pmType, UCHAR beginPort, UCHAR endPort, ULONG* pmList );
typedef void (*PFunc_View_Type5)( UCHAR portNum, std::string* waveLenList );
typedef void (*PFunc_View_Type6)( UCHAR portNum, float* pmList );


// view 回调注册接口
extern "C" __declspec(dllexport) void SetCB_View_ShowMsg( PFunc_View_Type0 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowProduct( PFunc_View_Type1 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowTestOPM( PFunc_View_Type2 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowOSWInfo( PFunc_View_Type3 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowOPMPMList( PFunc_View_Type4 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowOSWaveLenList( PFunc_View_Type5 func );
extern "C" __declspec(dllexport) void SetCB_View_ShowOSOutPutPMList( PFunc_View_Type6 func );

/*--------------------------------------------------数据结构--------------------------------------------------*/
/*
	FLAG1	LEN	VER	MFLAG	AMOUNT	NO	RES	  COMM	TCOMM	HSUM	DATA	SUM	
	3		2	1	1		2		2	res	  2		2		1		x		1	
*/
struct ProCommand
{
	UCHAR _cmd[1024];

	ProCommand( UCHAR mFlag=0, int dataLen=0, int res=0 )
	{
		// 初始为0
		memset( this, 0, sizeof( ProCommand ) );
		_dataBufLen = dataLen;

		// 初始化FLAG1
		_cmd[0] = 0xEF;
		_cmd[1] = 0xEF;
		_cmd[2] = 0x00;

		// 初始化LEN
		USHORT len1 = 8;
		if( mFlag==1 )
			len1 += 4;
		len1 += res;
		len1 += dataLen;
		// 
		_cmd[3] = (UCHAR)( len1>>8 );
		_cmd[4] = (UCHAR)len1;
		// 
		_cmdLen = (int)( len1 + 5 );

		// 设置VER
		UCHAR len2 = 0;
		if( mFlag==1 )
			len2 = 4;
		len2 += 1;
		len2 += res;
		_cmd[5] += (UCHAR)len2;
		// 
		_mflagToResLen = (int)len2;

		// 设置MFLAG
		_cmd[6] = mFlag;

		// 设置res
	}
	~ProCommand() {}

	void SetVer( int version ) { _cmd[5] += (UCHAR)( version<<4 ); }
	void SetAMOUNT_NO( USHORT AMOUNT, USHORT NO )
	{
		if( ( _cmd[6]&0x01 )!=1 )	
			return;

		// 设置AMOUNT
		_cmd[7] = (UCHAR)( AMOUNT>>8 );
		_cmd[8] = (UCHAR)AMOUNT;
	
		// 设置NO
		_cmd[9] = (UCHAR)( NO>>8 );
		_cmd[10] = (UCHAR)NO;
	}
	void SetCOMM_TCOMM( USHORT COMM, USHORT TCOMM = 0x0001 )
	{
		UCHAR *p = _cmd;
		p += 6;
		p += _mflagToResLen;

		// 设置COMM
		*p = (UCHAR)( COMM>>8 );
		*( p+1 ) = (UCHAR)COMM;

		// 设置TCOMM
		p += 2;
		*p = (UCHAR)( TCOMM>>8 );
		*( p+1 ) = (UCHAR)TCOMM;
	}
	void CalHSUM()
	{
		int len = 0;
		len += 10;
		len += _mflagToResLen;

		UCHAR sum = 0;
		for( int i=0; i<len; ++i )
			sum += _cmd[i];

		// 
		_cmd[len] = sum;
	}
	void SetDataBuf( UCHAR* pBuf )
	{
		// 
		int len = 0;
		len = 11;
		len += _mflagToResLen;

		memcpy( _cmd+len, pBuf, _dataBufLen );
	}
	void CalSUM()
	{
		// 
		CalHSUM();
	
		USHORT len = _cmdLen - 1;

		UCHAR sum = 0;
		for( USHORT i=0; i<len; ++i )
			sum += _cmd[i];

		// 
		_cmd[len] = sum;	
	}
	inline int GetCmdLen() { return _cmdLen; }

private:
	int _mflagToResLen;									// [MFLAG,RES]的长度
	int _dataBufLen;									// 数据buf的长度
	int _cmdLen;										// 记录整个命令字的长度
};

/*
	FLAG2	LEN		VER		MFLAG	AMOUNT	NO	RES	 COMM	TCOMM	HSUM	DATA	SUM	
	3		2		1		1		2		2	res	 2		2		1		x		1
*/
struct ProResponse
{
	ProResponse() { memset( this, 0, sizeof( ProResponse ) ); }
	~ProResponse() {}
	
	void GetCOMM_TCOMM( USHORT& COMM, USHORT& TCOMM )
	{
		int len = 0;
		len += 6;
		len += ( int )( _res[5]&0x0F );

		COMM = ( (USHORT)_res[len] )<<8;
		COMM += (USHORT)_res[len+1];

		len += 2;
		TCOMM = ( (USHORT)_res[len] )<<8;
		TCOMM += (USHORT)_res[len+1];
	}
	bool ChkHSUM()
	{
		int len = 0;
		len += 6;
		len += ( int )( _res[5]&0x0F );
		len += 4;

		UCHAR sum = 0;
		for( UCHAR i=0; i<len; ++i )
			sum += _res[i];

		return _res[len]==sum;	
	}
	UCHAR* GetDataBuf()
	{
		int len = 0;
		len += 6;
		len += ( int )( _res[5]&0x0F );
		len += 5;

		return ( _res+len );
	}
	bool ChkSUM()
	{
		if( ChkHSUM()!=true )
			return false;

		int len = 0;
		len = ( ( int )_res[3] )<<8;
		len += ( int )_res[4];
		len += 4;

		UCHAR sum = 0;
		for( int i=0; i<len; ++i )
			sum += _res[i];

		return _res[len]==sum;
	}
	USHORT GetDataBufLen()
	{
		USHORT len = ( (USHORT)( _res[3] ) )>>8;
		len += (USHORT)( _res[4] );
		len -= 12;

		return len;
	}
	USHORT GetLen()
	{
		USHORT len = ( (USHORT)( _res[3] ) )>>8;
		len += (USHORT)( _res[4] );

		return len; 
	}

	UCHAR _res[1024];
};

// 设备信息
// 可能出现的自动字节对齐 除了预编译宏外还用重载的=来保证  尽量避免memcpy
#pragma pack( 1 )
struct DeviceInfo
{
	DeviceInfo() { memset( this, 0, sizeof( DeviceInfo ) ); }
	~DeviceInfo() {}

	void assignValue( UCHAR *pBuf )
	{
		int offset = 0;

		_dataVer = *pBuf;
		offset += 1;

		memcpy( &_corNO, pBuf+offset, 2 );
		_corNO = ntohs( _corNO );
		offset += 2;

		memcpy( &_proNO, pBuf+offset, 2 );
		_proNO = ntohs( _proNO );
		offset += 2;
	
		memcpy( &_softVer, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_hardwareVer, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_mainType, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_subType, pBuf+offset, 2 );
		_subType = ntohs( _subType );
		offset += 2;

		memcpy( &_frameSN, pBuf+offset, 4 );
		_frameSN = ntohl( _frameSN );
		offset += 4;

		memcpy( &_slot, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_slotWidth, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_singleDiscSN, pBuf+offset, 4 );
		_singleDiscSN = ntohl( _singleDiscSN );
		offset += 4;

		memcpy( &_mac, pBuf+offset, 6 );
		offset += 6;

		memcpy( &_ipAddr, pBuf+offset, 4 );
		_ipAddr = ntohl( _ipAddr );
		offset += 4;
	
		memcpy( &_mask, pBuf+offset, 4 );
		_mask = ntohl( _mask );
		offset += 4;
	
		memcpy( &_gateAddr, pBuf+offset, 4 );
		_gateAddr = ntohl( _gateAddr );
		offset += 4;
	
		offset += 10;

		memcpy( &_udpMinPollingInterval, pBuf+offset, 1 );
		offset += 1;

		memcpy( &_temprature, pBuf+offset, 2 );
		_temprature = ntohs( _temprature );
		offset += 2;

		memcpy( &_temprature2, pBuf+offset, 2 );
		_temprature2 = ntohs( _temprature2 );
		offset += 2;
	}
	struct CompareSlot //: public binary_function<DeviceInfo, UCHAR, bool>
	{
		bool operator()( DeviceInfo& d, UCHAR curSlot ) { return ( d._slot==curSlot ); }
	};
	struct CompareIP //: public binary_function<DeviceInfo, ULONG, bool>
	{
		bool operator()( DeviceInfo& d, ULONG curIP ) { return ( d._ipAddr==curIP ); }
	};

	UCHAR _dataVer;
	USHORT _corNO;
	USHORT _proNO;
	UCHAR _softVer;
	UCHAR _hardwareVer;
	UCHAR _mainType;
	USHORT _subType;
	ULONG _frameSN;
	UCHAR _slot;
	UCHAR _slotWidth;
	ULONG _singleDiscSN;
	UCHAR _mac[6];
	ULONG _ipAddr;
	ULONG _mask;
	ULONG _gateAddr;
	UCHAR _resver1[10];
	UCHAR _udpMinPollingInterval;
	USHORT _temprature;
	USHORT _temprature2;								// 主控有效
	UCHAR _resver2[19];
	UCHAR _extraBuf[32];								// 自定义存放额外的信息
};



/*--------------------------------------------------UDP同步处理--------------------------------------------------*/
/*
	OLM Protocol		OLM设备通信协议
		||
		\/
	  UDP Layer			UDP网络层

	
	1：负责组织、解析数据包。
	2：收发整体采取异步托管的方式，获得最大的吞吐率。
	3：同时引入同步机制，部分命令可采取同步，但同步效率较低。
*/
class OLMPro : public UdpOBJ, public _SingleThreadManage<OLMPro>
{
	// 同步数据
	struct SynOPData
	{
		SynOPData( ULONG devIP, USHORT op, std::vector<Var>& paraList )
		{
			_devIP = _devIP;
			_op = _op;
			_paraList = _paraList;

			_stopWaitFlag = false;
			_count = 1;
		}

		bool Compare( ULONG& devIP, USHORT& op, std::vector<Var>& paraList )
		{
			if( _devIP!=_devIP || _op!=_op || _paraList.size()!=_paraList.size() )
				return false;

			for( UINT i=0; i<_paraList.size(); ++i )
			{
				if( _paraList[i] != _paraList[i] )
					return false;
			}

			return true;
		}

		/*--------------------------------主键:IP+OP+_paraList--------------------------------*/
		ULONG _devIP;									// 地址
		USHORT _op;										// 操作
		std::vector<Var> _paraList;							// 参数

		std::vector<Var> _resultList;						// 同步操作返回的值
		bool _stopWaitFlag;								// 处理标志
		int _count;										// 引用计数
	};

	class SynOPListManage
	{
	public:
		SynOPData* SysOPListSearch( ULONG ip, USHORT op, std::vector<Var>& paraList )
		{
			for( list<SynOPData>::iterator it=_synOPList.begin(); it!=_synOPList.end(); ++it )
			{
				if( it->Compare( ip, op, paraList )==true )
					return &( *it );
			}
			return NULL;
		}	
		
		void SysOPListPushMute( ULONG ip, USHORT op, std::vector<Var>& opParaList, SynOPData*& pSynOPData )
		{
			cnCriticalSectionAuto csAuto( _cs );

			pSynOPData = SysOPListPush( ip, op, opParaList );
		}
		void SysOPListPopMute( SynOPData*& pSynOPData )
		{
			cnCriticalSectionAuto csAuto( _cs );

			SysOPListPop( pSynOPData );
		}

		void SysOPListLock() { _cs.Lock(); }
		void SysOPListUnLock() { _cs.UnLock(); }

	private:
		SynOPData* SysOPListPush( ULONG ip, USHORT op, std::vector<Var>& paraList )
		{
			SynOPData* pSynOPData = SysOPListSearch( ip, op, paraList );
			// 已经存在则计数器自加
			if( pSynOPData!=NULL )
			{
				++pSynOPData->_count;
				return pSynOPData;
			}

			_synOPList.push_back( SynOPData( ip, op, paraList ) );
			return &_synOPList.back();
		}
		void SysOPListPop( SynOPData* pSynOPData )
		{
			if( pSynOPData==NULL )
				return;

			if( --pSynOPData->_count==0 )
			{
				for( list<SynOPData>::iterator it=_synOPList.begin(); it!=_synOPList.end(); ++it )
				{
					if( &( *it )==pSynOPData )
					{
						_synOPList.erase( it );
						return;
					}
				}
			}
		}

		list<SynOPData> _synOPList;
		cnCriticalSection _cs;
	};

public:
	static OLMPro* GetSingleton();
	static void Destroy();


	// 各设备通用协议
	// Asynchronous
	void ReadProductInfo( ULONG scanIP );																		// 广播|单播 读取产品信息
	void SetSN( UCHAR slot, ULONG sn, ULONG devIP );															// 设置sn
	void SetIP( UCHAR slot, ULONG ipAddr, ULONG gateIP, ULONG netMask, ULONG devIP );							// 设置ip
	void SetMAC( UCHAR slot, UCHAR* pMAC, ULONG devIP );														// 设置mac
	void SetFrameSN( ULONG RackSN, ULONG devIP );																// 设置框架sn
	// Synchronous
	bool ReadProductInfoSyn( ULONG devIP, DeviceInfo& devInfo );

	// 硬件测试协议
	// Asynchronous
	void TestDS1642_E2prom( ULONG devIP );
	void TestDS1642_Temp( ULONG devIP );
	void TestBackBoardE2prom( ULONG devIP );
	void TestFan( ULONG devIP );
	void TestWatchDog( ULONG devIP );
	void TestLed( ULONG devIP );
	void TestOPM( USHORT para, ULONG devIP );
	void TestOSW( int sel, int outPort, ULONG devIP );
	void TestI2C0_E2prom( ULONG devIP );
	void TestI2C0_Temp( ULONG devIP );

	// 固件升级协议
	// Asynchronous
	void ActiveIP( bool activeIP, ULONG devIP );
	void ActiveDownload( bool activeDown, ULONG devIP );

	// OSW
	// Asynchronous
	void GetOSWStatus( UCHAR mainType, USHORT subType, ULONG devIP );
	void SetOSWStatus( UCHAR oswType, UCHAR mainType, USHORT subType, USHORT inPortNO, USHORT outPortNO, ULONG devIP );
	void SetOSWType( UCHAR oswType, ULONG devIP );

	// OPM 旧版设计通信协议 新版尽量兼容
	// Asynchronous
	void GetOPMSample( UCHAR mainType, USHORT subType, UCHAR channel, ULONG devIP );
	void GetOPMPowerMeter( UCHAR mainType, USHORT subType, ULONG devIP );
	void SetOPMWaveLen( UCHAR waveFlag[32], ULONG devIP );
	void GetOPMCalbTestPM( UCHAR mainType, USHORT subType, UCHAR port, ULONG devIP );
	void SendOPMCalData( UCHAR channel, int packetNO, char* pBuf, ULONG devIP );
	void SetOPMPortNum( UCHAR portNum, ULONG devIP );
	// Synchronous
	bool GetOPMSampleSyn( UCHAR mainType, USHORT subType, UCHAR channel, USHORT &sampleValue, ULONG devIP );
	bool GetOPMCalbTestPMSyn( UCHAR mainType, USHORT subType, UCHAR port, float& calbTestPM, ULONG devIP );
	bool SendOPMCalDataSyn( UCHAR channel, int packetNO, char* pBuf, ULONG devIP );

	// OPM 新版命令 用于定标
	void SetOPM2Clear( ULONG devIP );
	void GetOPM2ADPM( UCHAR startC, UCHAR endC, ULONG devIP );
	void SetOPM2Dx( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP );
	// Synchronous
	bool GetOPM2ADPMSyn( UCHAR startC, UCHAR endC, float pm[32], ULONG devIP );
	bool SetOPM2DxSyn( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP );
	bool SetOPM2ClearSyn( char v[32], ULONG devIP );

	// 光源
	// Asynchronous
	void GetOSSampleOrPM( bool flag, ULONG devIP );
	void SwitchOS( bool openFlag, ULONG devIP );
	void SetOSCalData( float k, float b, int n, float* pData, ULONG devIP );
	void GetOSOutPutPM( ULONG devIP );
	void GetOSWaveLen( ULONG devIP );
	void SetOSWaveLen( UINT portNum, std::string waveLen[16], ULONG devIP );
	void GetOSMaxMinPM( UCHAR monitorFlag, ULONG devIP );
	void SetOSResistance( UCHAR flag, ULONG devIP );  
	// Synchronous
	bool GetOSPMSyn( float& pm, ULONG devIP );
	bool GetOSMaxMinPMSyn( UCHAR monitorFlag1, UCHAR& monitorFlag2, float& max, float& min, ULONG devIP );
	bool GetOSSampleSyn( USHORT& sample, ULONG devIP );
	bool GetOSPortNumSyn( UCHAR& portNum, ULONG devIP );
	bool SetOSResistanceSyn( UCHAR flag, USHORT& IBMON, USHORT& IMPDMON, USHORT& resistanceLSB, ULONG devIP );  
	bool GetOSOutPutPMSyn(  UCHAR& portNum, float pmList[16], ULONG devIP );
	bool GetOSOutPutPortPMSyn(  UCHAR portIndex, float& pm, ULONG devIP );

	void StartRecvResponseThread();

private:
	void RecvResponseThread();
	bool ParaseResponse( ProResponse& response, USHORT& op, std::vector<Var>& paraList, std::string& msg, std::vector<Var>& resultList );
	void HandleShowResult( ULONG ip, USHORT& op, std::vector<Var>& paraList, std::string& msg, std::vector<Var>& resultList );

	// 同步队列管理
	SynOPListManage _synOPLit;

	static OLMPro* _pSingleton;
};



extern "C" __declspec(dllexport) void ReadProductInfo( ULONG scanIP );																		// 广播|单播 读取产品信息
extern "C" __declspec(dllexport) void SetSN( UCHAR slot, ULONG sn, ULONG devIP );															// 设置sn
extern "C" __declspec(dllexport) void SetIP( UCHAR slot, ULONG ipAddr, ULONG gateIP, ULONG netMask, ULONG devIP );							// 设置ip
extern "C" __declspec(dllexport) void SetMAC( UCHAR slot, UCHAR* pMAC, ULONG devIP );														// 设置mac
extern "C" __declspec(dllexport) void SetFrameSN( ULONG RackSN, ULONG devIP );																// 设置框架sn
	// Synchronous
extern "C" __declspec(dllexport) bool ReadProductInfoSyn( ULONG devIP, DeviceInfo& devInfo );

	// 硬件测试协议
	// Asynchronous
extern "C" __declspec(dllexport) void TestDS1642_E2prom( ULONG devIP );
extern "C" __declspec(dllexport) void TestDS1642_Temp( ULONG devIP );
extern "C" __declspec(dllexport) void TestBackBoardE2prom( ULONG devIP );
extern "C" __declspec(dllexport) void TestFan( ULONG devIP );
extern "C" __declspec(dllexport) void TestWatchDog( ULONG devIP );
extern "C" __declspec(dllexport) void TestLed( ULONG devIP );
extern "C" __declspec(dllexport) void TestOPM( USHORT para, ULONG devIP );
extern "C" __declspec(dllexport) void TestOSW( int sel, int outPort, ULONG devIP );
extern "C" __declspec(dllexport) void TestI2C0_E2prom( ULONG devIP );
extern "C" __declspec(dllexport) void TestI2C0_Temp( ULONG devIP );

	// 固件升级协议
	// Asynchronous
extern "C" __declspec(dllexport) void ActiveIP( bool activeIP, ULONG devIP );
extern "C" __declspec(dllexport) void ActiveDownload( bool activeDown, ULONG devIP );

	// OSW
	// Asynchronous
extern "C" __declspec(dllexport) void GetOSWStatus( UCHAR mainType, USHORT subType, ULONG devIP );
extern "C" __declspec(dllexport) void SetOSWStatus( UCHAR oswType, UCHAR mainType, USHORT subType, USHORT inPortNO, USHORT outPortNO, ULONG devIP );
extern "C" __declspec(dllexport) void SetOSWType( UCHAR oswType, ULONG devIP );

	// OPM 旧版设计通信协议 新版尽量兼容
	// Asynchronous
extern "C" __declspec(dllexport) void GetOPMSample( UCHAR mainType, USHORT subType, UCHAR channel, ULONG devIP );
extern "C" __declspec(dllexport) void GetOPMPowerMeter( UCHAR mainType, USHORT subType, ULONG devIP );
extern "C" __declspec(dllexport) void SetOPMWaveLen( UCHAR waveFlag[32], ULONG devIP );
extern "C" __declspec(dllexport) void GetOPMCalbTestPM( UCHAR mainType, USHORT subType, UCHAR port, ULONG devIP );
extern "C" __declspec(dllexport) void SendOPMCalData( UCHAR channel, int packetNO, char* pBuf, ULONG devIP );
extern "C" __declspec(dllexport) void SetOPMPortNum( UCHAR portNum, ULONG devIP );
	// Synchronous
extern "C" __declspec(dllexport) bool GetOPMSampleSyn( UCHAR mainType, USHORT subType, UCHAR channel, USHORT &sampleValue, ULONG devIP );
extern "C" __declspec(dllexport) bool GetOPMCalbTestPMSyn( UCHAR mainType, USHORT subType, UCHAR port, float& calbTestPM, ULONG devIP );
extern "C" __declspec(dllexport) bool SendOPMCalDataSyn( UCHAR channel, int packetNO, char* pBuf, ULONG devIP );

	// OPM 新版命令 用于定标
extern "C" __declspec(dllexport) void SetOPM2Clear( ULONG devIP );
extern "C" __declspec(dllexport) void GetOPM2ADPM( UCHAR startC, UCHAR endC, ULONG devIP );
extern "C" __declspec(dllexport) void SetOPM2Dx( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP );
	// Synchronous
extern "C" __declspec(dllexport) bool GetOPM2ADPMSyn( UCHAR startC, UCHAR endC, float pm[32], ULONG devIP );
extern "C" __declspec(dllexport) bool SetOPM2DxSyn( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP );
extern "C" __declspec(dllexport) bool SetOPM2ClearSyn( char v[32], ULONG devIP );

	// 光源
	// Asynchronous
extern "C" __declspec(dllexport) void GetOSSampleOrPM( bool flag, ULONG devIP );
extern "C" __declspec(dllexport) void SwitchOS( bool openFlag, ULONG devIP );
extern "C" __declspec(dllexport) void SetOSCalData( float k, float b, int n, float* pData, ULONG devIP );
extern "C" __declspec(dllexport) void GetOSOutPutPM( ULONG devIP );
extern "C" __declspec(dllexport) void GetOSWaveLen( ULONG devIP );
extern "C" __declspec(dllexport) void SetOSWaveLen( UINT portNum, std::string waveLen[16], ULONG devIP );
extern "C" __declspec(dllexport) void GetOSMaxMinPM( UCHAR monitorFlag, ULONG devIP );
extern "C" __declspec(dllexport) void SetOSResistance( UCHAR flag, ULONG devIP );  
	// Synchronous
extern "C" __declspec(dllexport) bool GetOSPMSyn( float& pm, ULONG devIP );
extern "C" __declspec(dllexport) bool GetOSMaxMinPMSyn( UCHAR monitorFlag1, UCHAR& monitorFlag2, float& max, float& min, ULONG devIP );
extern "C" __declspec(dllexport) bool GetOSSampleSyn( USHORT& sample, ULONG devIP );
extern "C" __declspec(dllexport) bool GetOSPortNumSyn( UCHAR& portNum, ULONG devIP );
extern "C" __declspec(dllexport) bool SetOSResistanceSyn( UCHAR flag, USHORT& IBMON, USHORT& IMPDMON, USHORT& resistanceLSB, ULONG devIP );  
extern "C" __declspec(dllexport) bool GetOSOutPutPMSyn(  UCHAR& portNum, float pmList[16], ULONG devIP );
extern "C" __declspec(dllexport) bool GetOSOutPutPortPMSyn(  UCHAR portIndex, float& pm, ULONG devIP );

extern "C" __declspec(dllexport) void StartRecvResponseThread();