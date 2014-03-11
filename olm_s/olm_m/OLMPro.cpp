
#include "stdafx.h"
#include "OLMPro.h"
using namespace std;


OLMPro* OLMPro::_pSingleton	= NULL;


PFunc_View_Type0 PCB_View_ShowMsg;
PFunc_View_Type1 PCB_View_ShowProduct;
PFunc_View_Type2 PCB_View_ShowTestOPM;
PFunc_View_Type3 PCB_View_ShowOSWInfo;
PFunc_View_Type4 PCB_View_ShowOPMPMList;
PFunc_View_Type5 PCB_View_ShowOSWaveLenList;
PFunc_View_Type6 PCB_View_ShowOSOutPutPMList;


__declspec(dllexport) void SetCB_View_ShowMsg( PFunc_View_Type0 func )					{ PCB_View_ShowMsg = func; }
__declspec(dllexport) void SetCB_View_ShowProduct( PFunc_View_Type1 func )				{ PCB_View_ShowProduct = func; }
__declspec(dllexport) void SetCB_View_ShowTestOPM( PFunc_View_Type2 func )				{ PCB_View_ShowTestOPM = func; }
__declspec(dllexport) void SetCB_View_ShowOSWInfo( PFunc_View_Type3 func )				{ PCB_View_ShowOSWInfo = func; }
__declspec(dllexport) void SetCB_View_ShowOPMPMList( PFunc_View_Type4 func )			{ PCB_View_ShowOPMPMList = func; }
__declspec(dllexport) void SetCB_View_ShowOSWaveLenList( PFunc_View_Type5 func )		{ PCB_View_ShowOSWaveLenList = func; }
__declspec(dllexport) void SetCB_View_ShowOSOutPutPMList( PFunc_View_Type6 func )		{ PCB_View_ShowOSOutPutPMList = func; }



OLMPro* OLMPro::GetSingleton()
{
	#define CheckNULL( obj )  if( obj==NULL ) return NULL;

	if( _pSingleton==NULL )  
		_pSingleton = new OLMPro;			// new第一个对象

	return _pSingleton; 
}

void OLMPro::Destroy()
{
	if( _pSingleton!=NULL ) 
		delete _pSingleton;

	_pSingleton = NULL;
}

void OLMPro::StartRecvResponseThread()
{
	if( StartThread( &OLMPro::RecvResponseThread )==false )
		::MessageBoxA( NULL, "设备监听线程创建失败！", "告警", 0 );
}

void OLMPro::RecvResponseThread()
{
	string strMsg;

	while( 1 )
	{
		Sleep( 1 );

		// 接收数据包
		ProResponse response;
		ULONG recvIP = 0;
		if( RecvFrom( (char*)&response, sizeof( response ), recvIP )<=0 )
			continue;
		
		// 解析数据包 获得接收的数据
		USHORT op;
		vector<Var> paraList;
		vector<Var> resultList;
		//
		if( ParaseResponse( response, op, paraList, strMsg, resultList )==false )
			continue;

		// 判断是否在同步队列中进行处理
		_synOPLit.SysOPListLock();
		//
		SynOPData *pSynOPData = _synOPLit.SysOPListSearch( recvIP, op, paraList );
		if( pSynOPData!=NULL )
		{
			pSynOPData->_resultList = resultList;
			pSynOPData->_stopWaitFlag = true;
		}
		//
		_synOPLit.SysOPListUnLock();

		/*
			回调函数处理: 

			1 数据库处理,更新数据库设备信息,暂不实现;
			2 逻辑处理;
			3 gui显示;
		*/
		HandleShowResult( recvIP, op, paraList, strMsg, resultList );
		//if( pCB_HandleResponse!=NULL )
		//	( *pCB_HandleResponse )( recvIP, op, resultList, strMsg );
	}
}

void OLMPro::HandleShowResult( ULONG ip, USHORT& op, std::vector<Var>& paraList, std::string& msg, std::vector<Var>& resultList )
{
	PCB_View_ShowMsg( msg.c_str() );

	switch( op )
	{
		case 0xFF01: // 读产品信息
			if( PCB_View_ShowProduct!=NULL ) PCB_View_ShowProduct( resultList[0] );
			break;
		case 0xEE01: // 测试DS1642-e2prom
			break;
		case 0xEE02: // 测试DS1642-Temp
			break;
		case 0xEE03: // 测试背板e2prom
			break;
		case 0xEE04: // 测试风扇
			break;
		case 0xEE05: // 测试看门狗
			break;
		case 0xEE06: // 测试指示灯
			break;
		case 0xEE08: // 测试opm	
			{
				if( (bool)resultList[0] && PCB_View_ShowTestOPM!=NULL )
				{
					vector<USHORT> vList;
					for( UINT i=1; i<vList.size(); ++i )
						vList.push_back( resultList[i] );

					PCB_View_ShowTestOPM( &vList[1], vList.size()-1 );
					//STRING_FORMAT( str, "0x%02x%02x ", pDataBuf[16*i+2*j], pDataBuf[16*i+2*j+1] );
				}	
			}
			break;
		case 0xEE09: // 测试光开关
			break;
		case 0xEE0A: // 测试I2C0 DS1642-e2prom
			break;
		case 0xEE0B: // 测试I2C0 DS1642-Temper
			break;
		case 0xFF20: // 设置单盘SN信息
			break;
		case 0xFF21: // 设置单盘IP信息
			break;
		case 0xFF22: // 设置单盘MAC信息
			break;
		case 0xFF88: // 主控盘设置机架SN
			break;
		case 0x0110: // 获取光开关状态
			if( PCB_View_ShowOSWInfo!=NULL ) PCB_View_ShowOSWInfo( resultList[0], resultList[1], resultList[2], resultList[3], resultList[4], resultList[5] );
			break;
		case 0x0804: // 设置光开关状态
			break;
		case 0xFF70: // 设置光开关类型
			break;
		case 0x0120: // 获取光功率计采样值
			break;			
		case 0x0122: // 获取功率计功率值
			if( PCB_View_ShowOPMPMList!=NULL ) 
			{
				UCHAR beginPort = resultList[1];
				UCHAR endPort = resultList[2];
				ULONG pmList[32];
				for(UCHAR i=beginPort; i<=endPort; ++i)
					pmList[i-beginPort] = resultList[3+i-beginPort];

				PCB_View_ShowOPMPMList( resultList[0], beginPort, endPort, pmList );
			}
			break;
		case 0x0123: // 设置功率计测试波长
			break;
		case 0x0124:
			break;
		case 0x0125:
			break;
		case 0x126:
			break;
		case 0x0130:
			break;
		case 0x0131:
			break;
		case 0x0132:
			if( PCB_View_ShowOSWaveLenList!=NULL )
			{
				UCHAR rw = resultList[0]; 
				if( rw==0 )
				{
					UCHAR portNum = resultList[1];
					string waveLen[32];
					for(UCHAR i=0; i<portNum; ++i)
						waveLen[i] = (string)resultList[2+i];

					PCB_View_ShowOSWaveLenList( portNum, waveLen );
				}
			}
			break;
		case 0x0133:
			break;
		case 0x0134:
			{
				UCHAR portNum = resultList[0];
				float pmList[16];
				for(UCHAR i=0; i<portNum; ++i)
					pmList[i] = resultList[i+1];

				PCB_View_ShowOSOutPutPMList( portNum, pmList );
			}
			break;
		case 0x0135:
			break;
		case 0xFF71: // 激活IP
			break;
		case 0xFF72: // 激活下载
			break;
		case 0x0121:
			break;
		case 0x0127:
			break;
		case 0x012B:
			break;
		case 0x0136:
		default:
			break;
	}
}

bool OLMPro::ParaseResponse( ProResponse& response, USHORT& op, vector<Var>& paraList, string& strMsg, vector<Var>& resultList )
{
	// 检查校验和
	if( response.ChkSUM()==false )
	{
		strMsg = "校验和错误..";
		return false;
	}
	
	// parase
	USHORT comm, tComm;
	response.GetCOMM_TCOMM( comm, tComm );
	UCHAR *pDataBuf = response.GetDataBuf();

	op = comm;
	switch( comm )
	{
		case 0xFF01: // 读产品信息
			{
				static DeviceInfo devInfo;
				devInfo.assignValue( response.GetDataBuf() );

				resultList.push_back( (void*)&devInfo );
			}
			break;
		case 0xEE01: // 测试DS1642-e2prom
			{	
				UCHAR res;
				res = *pDataBuf;
				// 组织返回的数据
				if( res==0x53 )
				{
					strMsg = "测试DS1642-e2prom成功!";
					resultList.push_back( true );
				}
				else if( res==0x46 )
				{
					strMsg = "测试DS1642-e2prom失败!";
					resultList.push_back( false );
				}
			}
			break;
		case 0xEE02: // 测试DS1642-Temp
			{
				USHORT tep;
				tep = ( (USHORT)( *pDataBuf ) )<<8;
				tep += (USHORT)( *( pDataBuf+1 ) );

				float temp;
				temp = (float)( ( ( tep )&0xFF00 )>>8 );
				temp += (float)( ( ( tep )&0x00F0 )>>4 ) / 16;
				resultList.push_back( temp );

				STRING_FORMAT( strMsg, "测试DS1642_Temp: %.2f", temp );
			}
			break;
		case 0xEE03: // 测试背板e2prom
			{				
				UCHAR res;
				res = *pDataBuf;
				
				if( res==0x53 )
				{
					strMsg = "测试背板e2prom成功!";
					resultList.push_back( true );
				}
				else if( res==0x46 )
				{
					strMsg = "测试背板e2prom失败!";
					resultList.push_back( false );
				}
			}
			break;
		case 0xEE04: // 测试风扇
			{				
				UCHAR res;
				res = *pDataBuf;
				
				if( res==0x53 )
					strMsg = "风扇停!";
				else if( res==0x46 )
					strMsg = "风扇转!";
			}
			break;
		case 0xEE05: // 测试看门狗
			{
				UCHAR res;
				res = *pDataBuf;
				
				if( res==0x53 )
				{
					strMsg = "测试看门狗成功!";
					resultList.push_back( true );
				}
				else if( res==0x46 )
				{
					strMsg = "测试看门狗失败!";
					resultList.push_back( false );
				}
			}
			break;
		case 0xEE06: // 测试指示灯
			{	
				UCHAR res;
				res = *pDataBuf;
				// 组织返回的数据
				if( res == 0x00 )
					strMsg = "LED亮!";
				else if( res == 0x01 )
					strMsg = "LED灭!";
			}
			break;
		case 0xEE08:
			{
				USHORT comm, tComm;
				response.GetCOMM_TCOMM( comm, tComm );
				
				bool checkResult = true;
				vector<USHORT> tempList;

				// 解析收到的数据
				int iMax=0;
				if( tComm==0x0010 )
					iMax = 2;
				else // if( tComm==0x0020 )
					iMax = 4;
				for( int i=0; i<iMax; i++ )
				{
					for( int j=0; j<8; ++j )
					{
						if( pDataBuf[16*i+2*j]<0x80 || pDataBuf[16*i+2*j]>0x90 )
						{
							checkResult = false;
							break;
						}
						tempList.push_back( pDataBuf[16*i+2*j] );
						tempList.push_back( pDataBuf[16*i+2*j+1] );
						//STRING_FORMAT( str, "0x%02x%02x ", pDataBuf[16*i+2*j], pDataBuf[16*i+2*j+1] );
					}
				}

				if( checkResult==true )
				{
					strMsg = "测试成功!!";
					resultList.push_back( true );
					for( UINT i=0; i<tempList.size(); ++i )
						resultList.push_back( tempList[i] );
				}
				else	
				{
					strMsg = "测试失败!!";
					resultList.push_back( false );
				}
			}
			break;
		case 0xEE09: 
			{	
				UCHAR res;
				res = *pDataBuf;
				
				if( res==0x53 )
				{
					strMsg = "测试光开关成功!";
					resultList.push_back( true );
				}
				else if( res==0x46 )
				{
					strMsg = "测试光开关失败!";
					resultList.push_back( false );
				}
			}
			break;
		case 0xEE0A: // 测试I2C0 DS1642-e2prom
			{
				UCHAR res;
				res = *pDataBuf;
				// 组织返回的数据
				if( res==0x53 )
					strMsg = "测试I2C0通道 DS1642-e2prom成功!";
				else if( res==0x46 )
					strMsg = "测试I2C0通道 DS1642-e2prom失败!";
			}
			break;
		case 0xEE0B: // 测试I2C0 DS1642-Temp
			{
				USHORT tep;
				tep = ( (USHORT)( *pDataBuf ) )<<8;
				tep += (USHORT)( *( pDataBuf+1 ) );
				// 温度解析
				string tempStr;
				float temp;
				temp = (float)( ( ( tep )&0xFF00 )>>8 );
				temp += (float)( ( ( tep )&0x00F0 )>>4 ) / 16;
			
				resultList.push_back( temp );
				STRING_FORMAT( strMsg, "I2C0通道 DS1642_Temp: %.2f", temp );
			}
			break;
		case 0xFF20: // 设置单盘SN信息
			{		
				UCHAR res;
				res = *pDataBuf;

				strMsg = "设置单盘sn成功!";
			}
			break;
		case 0xFF21: // 设置单盘IP信息
			{				
				UCHAR res;
				res = *pDataBuf;
				
				strMsg =  "设置单盘IP成功!";
			}
			break;
		case 0xFF22: // 设置单盘MAC信息
			{
				UCHAR res;
				res = *pDataBuf;
				
				strMsg =  "设置单盘MAC成功!";
			}
			break;
		case 0xFF88: // 主控盘设置机架SN
			{	
				UCHAR res;
				res = *pDataBuf;
				// 组织返回的数据
				if( res==0x53 )	
					strMsg =  "设置机架SN成功!";
				else if( res==0x45 )	
					strMsg = "失败，指令校验和不正确!";
				else if( res==0x46 )
					strMsg = "失败，主控收到非完整数据!";
			}
			break;
		case 0x0110: // 获取光开关状态
			{		
				pDataBuf += 3;

				UCHAR oswType;
				memcpy( &oswType, pDataBuf, 1 );
				pDataBuf += 1;
				// 
				pDataBuf += 2;

				USHORT temp;
				memcpy( &temp, pDataBuf, 2 );
				temp = ntohs( temp );
				pDataBuf += 2;

				USHORT inportNum;
				memcpy( &inportNum, pDataBuf, 2 );
				inportNum = ntohs( inportNum );			
				pDataBuf += 2;

				USHORT outportNum;
				memcpy( &outportNum, pDataBuf, 2 );
				outportNum = ntohs( outportNum );			
				pDataBuf += 2;

				USHORT inportNo;
				memcpy( &inportNo, pDataBuf, 2 );
				inportNo = ntohs( inportNo );			
				pDataBuf += 2;

				USHORT outportNo;
				memcpy( &outportNo, pDataBuf, 2 );
				outportNo = ntohs( outportNo );			

				// 组织返回的数据		
				resultList.push_back( oswType );		
				resultList.push_back( temp );		
				resultList.push_back( inportNum );	
				resultList.push_back( outportNum );		
				resultList.push_back( inportNo );	
				resultList.push_back( outportNo );
			}
			break;
		case 0x0804: // 设置光开关状态
			{		
				pDataBuf += 4;	
				
				if( (char)pDataBuf[0] < 0 )
					strMsg = "设置入端口状态失败，";
				else
					strMsg = "设置入端口状态成功，";

				if( (char)pDataBuf[1] < 0 )
					strMsg += "设置出端口状态失败.";
				else
					strMsg += "设置出端口状态成功.";
			}
			break;
		case 0xFF70: // 设置光开关类型
			{		
				if( pDataBuf[0] == 0x53 )	
				{
					strMsg = "设置光开关类型成功..";
					resultList.push_back( true );
				}
				else 		
				{
					strMsg = "设置光开关类型失败..";
					resultList.push_back( false );
				}
			}
			break;
		case 0x0120: // 获取光功率计采样值
			{		
				pDataBuf += 4;
				//
				UCHAR opmSamplePort = *pDataBuf;
				USHORT opmSampleValue = *( pDataBuf+1 )<<8;
				opmSampleValue += *( pDataBuf+2 );

				// 组织返回的数据
				paraList.push_back( opmSamplePort );
				//
				resultList.push_back( opmSamplePort );
				resultList.push_back( opmSampleValue );
			}
			break;			
		case 0x0122: // 获取功率计功率值
			{		
				pDataBuf += 4;

				UCHAR pmType = *pDataBuf;

				UCHAR beginPort = *pDataBuf;
				UCHAR endPort = *( pDataBuf+1 );
				pDataBuf += 2;

				ULONG buf[32];
				for( UCHAR i=beginPort; i<=endPort; ++i )
				{
					memcpy( &buf[i-beginPort], pDataBuf, 4 );
					pDataBuf += 4;
				}

				// 组织返回的数据	
				resultList.push_back( pmType );
				resultList.push_back( beginPort );
				resultList.push_back( endPort );
				for( UCHAR i=beginPort; i<=endPort; ++i )
					resultList.push_back( buf[i-beginPort] );
			}
			break;
		case 0x0123: // 设置功率计测试波长
			{				
				if( pDataBuf[0] == 0x53 )
					strMsg = "设置功率计测试波长成功";
				else 	
					strMsg = "设置功率计测试波长失败";
			}
			break;
		case 0x0124:
			{			
				pDataBuf += 3;
				pDataBuf += 1;

				UCHAR opmTestPMPort = *pDataBuf;
				++pDataBuf;

				long pmVaule;
				memcpy( &pmVaule, pDataBuf, 4 );
				pmVaule = ( long )ntohl( pmVaule );
				float opmTestPMValue = (float)pmVaule/100;
				
				paraList.push_back( opmTestPMPort );
				//
				resultList.push_back( opmTestPMPort );
				resultList.push_back( opmTestPMValue );
			}
			break;
		case 0x0125:
			break;
		case 0x126:
			strMsg = "设置OPM端口数成功..";
			break;
		case 0x0130:
			{
				paraList.push_back( pDataBuf[0] );

				if( pDataBuf[0]==0 )
				{
					USHORT sample;
					sample = ( pDataBuf[1]<<8 )+pDataBuf[2];
					
					resultList.push_back( sample );
					STRING_FORMAT( strMsg, "OS采样值：%d", sample );
				}
				else
				{
					USHORT pm;
					memcpy( &pm, pDataBuf+1, 2 );
					pm = ntohs( pm );
					float pm2 = pm/100.f;

					resultList.push_back( pm2 );
					STRING_FORMAT( strMsg, "OS功率值：%0.2f", pm2 );
				}
			}
			break;
		case 0x0131:
			{	
				paraList.push_back( pDataBuf[0] );

				if( pDataBuf[0]==0 )
					strMsg = "关闭光源..";
				else
					strMsg = "打开光源..";
			}
			break;
		case 0x0132:
			{				
				UCHAR rw = pDataBuf[0]; 
				paraList.push_back( rw );
				resultList.push_back( rw );
				if( rw==1 )
				{					
					strMsg = "设置波长成功..";
				}
				else if( rw==0 )
				{
					UCHAR portNum = pDataBuf[1];
					resultList.push_back( portNum );

					string waveLen[16];
					for( UCHAR i=0; i<portNum; ++i )
					{
						if( pDataBuf[2+i]==1 )
							waveLen[i] = "1625";
						else if( pDataBuf[2+i]==2 )
							waveLen[i] = "1550";
						else if( pDataBuf[2+i]==3 )
							waveLen[i] = "1310";
					}
				
					for( UCHAR i=0; i<portNum; ++i )
						resultList.push_back( waveLen[i] );
				}
			}
			break;
		case 0x0133:
			strMsg = "os定标成功..";
			break;
		case 0x0134:
			{				
				UCHAR portNum = *pDataBuf;
				short* pmAddr = (short*)( pDataBuf+1 );
				float pmList[16];
				for( UCHAR i=0; i<portNum; ++i )
				{
					short pm;
					memcpy( &pm, pmAddr+i, 2 );
					pm = ntohs( pm );
					pmList[i] = (float)pm/100;
				}
	
				resultList.push_back( portNum );
				for( UCHAR i=0; i<portNum; ++i )
					resultList.push_back( pmList[i] );
			}
			break;
		case 0x0135:
			{				
				resultList.push_back( pDataBuf[0] );

				SHORT pm;
				memcpy( &pm, pDataBuf+1, 2 );
				pm = ntohs( pm );
				resultList.push_back( pm/100.f );

				memcpy( &pm, pDataBuf+3, 2 );
				pm = ntohs( pm );
				resultList.push_back( pm/100.f );
			}
			break;
		case 0xFF71: // 激活IP
			{			
				string stateStr;
				if( pDataBuf[0] == 0x53 )
					strMsg = "激活IP成功!";
				else 
					strMsg = "激活IP失败..";
			}
			break;
		case 0xFF72: // 激活下载
			{
				string stateStr;
				if( pDataBuf[0] == 0x53 )
					strMsg = "激活下载成功!";
				else 
					strMsg = "激活下载失败..";
			}
			break;
		case 0x0121:
			{
				USHORT len = response.GetLen();
				len -= 8;
				for( int i=0; i<len; ++i )
					resultList.push_back( (char)pDataBuf[i] );
			}
			break;
		case 0x0127:
			{
				// 组织返回的数据
				UCHAR portNum = pDataBuf[1]-pDataBuf[0]+1;
				//
				paraList.push_back( pDataBuf[0] );
				paraList.push_back( pDataBuf[1] );
				//
				int offset = 2+3*portNum;
				short pm = ( pDataBuf[offset]<<8 )+pDataBuf[offset+1];
				for( UCHAR i=0; i<portNum; ++i )
					resultList.push_back( (float)( pm/100.f ) );
			}
			break;
		case 0x012B:
			{
				// 组织返回的数据
				UCHAR portNum = pDataBuf[1]-pDataBuf[0];
				//
				paraList.push_back( pDataBuf[0] );
				paraList.push_back( pDataBuf[1] );
				paraList.push_back( pDataBuf[3] );
				//
				// 组织返回的数据
				if( pDataBuf[2] == 0x53 )
					resultList.push_back( true );
				else if( pDataBuf[2] == 0x46 )
					resultList.push_back( false );
			}
			break;
		case 0x0136:
			{
				USHORT IBMON;
				memcpy( &IBMON, pDataBuf, 2 );
				IBMON = ntohs( IBMON );
				resultList.push_back( IBMON );
				//
				USHORT IMPDMON;
				memcpy( &IMPDMON, pDataBuf+4, 2 );
				IMPDMON = ntohs( IMPDMON );
				resultList.push_back( IMPDMON );
				//
				USHORT resistanceLSB;
				memcpy( &resistanceLSB, pDataBuf+6, 2 );
				resistanceLSB = ntohs( resistanceLSB );
				resultList.push_back( resistanceLSB );
			}
			break;
		default:
			break;
	}

	return true;
}

// 广播|单播 读取产品信息
void OLMPro::ReadProductInfo( ULONG scanIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF01 );
	cmd.CalSUM();

	SendToBroadCast( (char*)&cmd, cmd.GetCmdLen(), scanIP );
}

bool OLMPro::ReadProductInfoSyn( ULONG devIP, DeviceInfo& devInfo )
{
	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	//
	_synOPLit.SysOPListPushMute( devIP, 0xFF01, opParaList, pSynOPdata );

	// 发送请求
	ReadProductInfo( devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )

	// 处理回复结果
	bool rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		DeviceInfo* pDev = ( DeviceInfo* )(void*)pSynOPdata->_resultList[0];
		devInfo = *pDev;
		delete pDev;
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::SetSN( UCHAR slot, ULONG sn, ULONG devIP )
{
	ProCommand cmd( 0, 7 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF20 );
	// 设data域
	UCHAR dataBuf[7];
	memset( dataBuf, 0 ,7 );
	dataBuf[0] = slot;
	dataBuf[1] = 0x01;
	dataBuf[2] = (UCHAR)( sn>>24 );
	dataBuf[3] = (UCHAR)( sn>>16 );
	dataBuf[4] = (UCHAR)( sn>>8 );
	dataBuf[5] = (UCHAR)sn;
	dataBuf[6] = 0x01;
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

void OLMPro::SetIP( UCHAR slot, ULONG ipAddr, ULONG gateIP, ULONG netMask, ULONG devIP )
{
	ProCommand cmd( 0, 14 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF21 );
	// 设data域
	UCHAR dataBuf[14];
	memset( dataBuf, 0 ,14 );
	dataBuf[0] = slot;
	dataBuf[1] = 0x07;
	// 
	ipAddr = htonl( ipAddr );
	memcpy( dataBuf+2, &ipAddr, 4 );
	// 
	netMask = htonl( netMask );
	memcpy( dataBuf+6, &netMask, 4 );
	// 
	gateIP = htonl( gateIP );
	memcpy( dataBuf+10, &gateIP, 4 );
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和
 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

void OLMPro::SetMAC( UCHAR slot, UCHAR* pMAC, ULONG devIP )
{
	ProCommand cmd( 0, 8 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF22 );
	// 设data域
	UCHAR dataBuf[8];
	memset( dataBuf, 0 ,8 );
	dataBuf[0] = slot;
	dataBuf[1] = 0x01;
	memcpy( dataBuf+2, pMAC, 6 );
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

void OLMPro::SetFrameSN( ULONG RackSN, ULONG devIP )
{
	ProCommand cmd( 0, 4 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF88 );
	// 设data域
	UCHAR dataBuf[4];
	memset( dataBuf, 0 ,4 );
	RackSN = htonl( RackSN );
	memcpy( dataBuf, (void*)&RackSN, 4 );	
	// 
	cmd.SetDataBuf( dataBuf );	
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

void OLMPro::ActiveIP( bool activeIP, ULONG devIP )
{
	ProCommand cmd( 0, 2 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF71 );
	// 设data域
	UCHAR dataBuf[2];
	memset( dataBuf, 0 , 2 );	
	if( activeIP == true )
		dataBuf[0] = 0x53;
	else
		dataBuf[0] = 0xFF;
	// 
	cmd.SetDataBuf( dataBuf );

	// 计算校验和
	cmd.CalSUM();

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::ActiveDownload( bool activeDown, ULONG devIP )
{
	ProCommand cmd( 0, 2 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF72 );
	// 设data域
	UCHAR dataBuf[2];
	memset( dataBuf, 0 , 2 );	
	if( activeDown == true )
		dataBuf[0] = 0xFF;
	else
		dataBuf[0] = 0x00;
	// 
	cmd.SetDataBuf( dataBuf );

	// 计算校验和
	cmd.CalSUM();

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestDS1642_E2prom( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE01 );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestDS1642_Temp( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE02 );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestI2C0_E2prom( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE0A );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestI2C0_Temp( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE0B );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

void OLMPro::TestBackBoardE2prom( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE03 );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestFan( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE04 );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestWatchDog( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE05 );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestLed( ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE06 );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestOPM( USHORT para, ULONG devIP )
{
	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE08, para );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::TestOSW( int sel, int outPort, ULONG devIP )
{
	USHORT para;
	para = (USHORT)outPort;
	para += (USHORT)( sel<<8 );

	ProCommand cmd;
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xEE09, para );
	cmd.CalSUM();
	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::GetOSWStatus( UCHAR mainType, USHORT subType, ULONG devIP )
{
	ProCommand cmd( 0, 3 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0110 );
	// 设data域
	UCHAR dataBuf[3];
	memset( dataBuf, 0 ,3 );
	dataBuf[0] = mainType;
	dataBuf[1] = (UCHAR)( subType>>8 );
	dataBuf[2] = (UCHAR)subType;
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::SetOSWStatus( UCHAR oswType, UCHAR mainType, USHORT subType, USHORT inPortNO, USHORT outPortNO, ULONG devIP )
{
	ProCommand cmd( 0, 8 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0804 );
	// 设data域
	UCHAR dataBuf[8];
	memset( dataBuf, 0 ,8 );
	// 
	dataBuf[0] = mainType;
	dataBuf[1] = (UCHAR)( subType>>8 );
	dataBuf[2] = (UCHAR)subType;
	// 
	dataBuf[3] = oswType;
	dataBuf[4] = (UCHAR)( inPortNO>>8 );
	dataBuf[5] = (UCHAR)( inPortNO );
	dataBuf[6] = (UCHAR)( outPortNO>>8 );
	dataBuf[7] = (UCHAR)( outPortNO );
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::SetOSWType( UCHAR oswType, ULONG devIP )
{
	ProCommand cmd( 0, 2 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0xFF70 );
	// 设data域
	UCHAR dataBuf[2];
	memset( dataBuf, 0 ,2 );
	dataBuf[0] = oswType;
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::GetOPMSample( UCHAR mainType, USHORT subType, UCHAR channel, ULONG devIP )
{
	ProCommand cmd( 0, 4 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0120 );
	// 设data域
	UCHAR dataBuf[4];
	memset( dataBuf, 0 ,4 );
	dataBuf[0] = mainType;
	dataBuf[1] = (UCHAR)( subType>>8 );
	dataBuf[2] = (UCHAR)subType;
	dataBuf[3] = channel;
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::GetOPMSampleSyn( UCHAR mainType, USHORT subType, UCHAR channel, USHORT &sampleValue, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( channel );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0120, opParaList, pSynOPdata );

	// 发送请求
	GetOPMSample( mainType, subType, channel, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )

	// 处理回复
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		sampleValue = pSynOPdata->_resultList[1];
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::GetOPMPowerMeter( UCHAR mainType, USHORT subType, ULONG devIP )
{
	ProCommand cmd( 0, 3 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0122 );
	// 设data域
	UCHAR dataBuf[3];
	memset( dataBuf, 0 ,3 );
	dataBuf[0] = mainType;
	dataBuf[1] = (UCHAR)( subType>>8 );
	dataBuf[2] = (UCHAR)subType;
	// 
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::SetOPMWaveLen( UCHAR waveFlag[32], ULONG devIP )
{
	ProCommand cmd( 0, 32 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0123 );
	// 设data域
	cmd.SetDataBuf( waveFlag );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::GetOPMCalbTestPM( UCHAR mainType, USHORT subType, UCHAR port, ULONG devIP )
{
	ProCommand cmd( 0, 4 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0124 );
	// 设data域
	UCHAR dataBuf[4];
	dataBuf[0] = mainType;
	dataBuf[1] = (UCHAR)( subType>>8 );
	dataBuf[2] = (UCHAR)subType;
	dataBuf[3] = port;
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::GetOPMCalbTestPMSyn( UCHAR mainType, USHORT subType, UCHAR port, float& calbTestPM, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( port );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0124, opParaList, pSynOPdata );

	// 发送请求
	GetOPMCalbTestPM( mainType, subType, port, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	
	// 处理回复
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
		calbTestPM = pSynOPdata->_resultList[1];

	// 从同步队列中删除
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::SendOPMCalData( UCHAR channel, int packetNO, char* pBuf, ULONG devIP )
{
	ProCommand cmd( 0, 802 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0125 );
	// 设data域
	UCHAR dataBuf[802];
	dataBuf[0] = (UCHAR)channel;
	dataBuf[1] = (UCHAR)packetNO;
	memcpy( dataBuf+2, pBuf, 800 );
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();			// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::SendOPMCalDataSyn( UCHAR channel, int packetNO, char* pBuf, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0125, opParaList, pSynOPdata );

	// 发送请求
	SendOPMCalData( channel, packetNO, pBuf, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	
	// 处理回复
	rt = pSynOPdata->_stopWaitFlag;
	
	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::SetOPMPortNum( UCHAR portNum, ULONG devIP )
{
	ProCommand cmd( 0, 1 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0126 );
	// 设data域
	cmd.SetDataBuf( &portNum );
	cmd.CalSUM();		// 计算校验和

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

void OLMPro::SetOPM2Clear( ULONG devIP )
{
	ProCommand cmd( 0, 0 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0121 );
	// 设data域
	cmd.CalSUM();

	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

bool OLMPro::SetOPM2ClearSyn( char v[32], ULONG devIP )
{
	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0121, opParaList, pSynOPdata );

	// 发送请求
	SetOPM2Clear( devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	
	// 处理回复
	bool rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		for( UINT i=0; i<pSynOPdata->_resultList.size(); ++i )
			v[i] = pSynOPdata->_resultList[i];
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::GetOPM2ADPM( UCHAR startC, UCHAR endC, ULONG devIP )
{
	ProCommand cmd( 0, 2 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0127 );
	// 设data域
	UCHAR dataBuf[2];
	dataBuf[0] = startC;
	dataBuf[1] = endC;
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();

	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );	
}

bool OLMPro::GetOPM2ADPMSyn( UCHAR startC, UCHAR endC, float pm[32], ULONG devIP )
{
	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( startC );
	opParaList.push_back( endC );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0127, opParaList, pSynOPdata );

	// 发送请求
	GetOPM2ADPM( startC, endC, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	bool rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		UCHAR portNum = endC-startC+1;
		for( int i=0; i<portNum; ++i )
			pm[i] = pSynOPdata->_resultList[i];
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::SetOPM2Dx( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP )
{
	int channelNum = endC-startC+1;
	ProCommand cmd( 0, 3+channelNum*2 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x012B );
	// 设data域
	UCHAR dataBuf[256];
	memset( dataBuf, 0 , 256 );
	dataBuf[0] = startC;
	dataBuf[1] = endC;
	dataBuf[2] = gear;
	for( int i=0; i<channelNum; ++i )
	{
		USHORT v = (USHORT)( dx[i]*100 );
		v = htons( v );
		memcpy( &dataBuf[3+i*2], &v, 2 );
	}
	//
	cmd.SetDataBuf( dataBuf );
	cmd.CalSUM();

	// 
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::SetOPM2DxSyn( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP )
{
	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( startC );
	opParaList.push_back( endC );
	opParaList.push_back( gear );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x012B, opParaList, pSynOPdata );

	// 发送请求
	SetOPM2Dx( startC, endC, gear, dx, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	bool rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
		rt = pSynOPdata->_resultList[0];

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::GetOSSampleOrPM( bool flag, ULONG devIP )
{
	ProCommand cmd( 0, 1 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0130 );
	// 设data域
	UCHAR data = flag ? 1:0;
	cmd.SetDataBuf( &data );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::GetOSPMSyn( float& pm, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( (UCHAR)1 );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0130, opParaList, pSynOPdata );

	// 发送请求
	GetOSSampleOrPM( 1, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
		pm = pSynOPdata->_resultList[1];

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

bool OLMPro::GetOSSampleSyn( USHORT& sample, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( (UCHAR)0 );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0130, opParaList, pSynOPdata );

	// 发送请求
	GetOSSampleOrPM( 0, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
		sample = pSynOPdata->_resultList[1];

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::SwitchOS( bool openFlag, ULONG devIP )
{
	ProCommand cmd( 0, 1 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0131 );
	// 设data域
	UCHAR data = openFlag ? 1:0;
	cmd.SetDataBuf( &data );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::SetOSCalData( float k, float b, int n, float* pData, ULONG devIP )
{
	if( n==0 ) return;

	int num = ( 2+n )*4;
	ProCommand cmd( 0, num );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0133 );
	// 设data域
	ULONG data[1024];
	memcpy( data, &k, 4 );
	// data[0] = htonl( data[0] );
	// 
	memcpy( data+1, &b, 4 );
	// data[1] = htonl( data[1] );
	// 
	for( int i=0; i<n; ++i )
	{
		memcpy( data+2+i, pData+i, 4 );
		// data[2+i] = htonl( data[2+i] );
	}
	cmd.SetDataBuf( (UCHAR*)&data );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::GetOSOutPutPM( ULONG devIP )
{
	ProCommand cmd( 0, 0 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0134 );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::GetOSOutPutPMSyn( UCHAR& portNum, float pmList[16], ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0134, opParaList, pSynOPdata );

	// 发送请求
	GetOSOutPutPM( devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		portNum = pSynOPdata->_resultList[0];
		for( UCHAR i=0; i<portNum; ++i )
			pmList[i] = pSynOPdata->_resultList[1+i];
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

bool OLMPro::GetOSOutPutPortPMSyn( UCHAR portIndex, float& pm, ULONG devIP )
{
	UCHAR portNum;
	float pmList[16];
	if( !GetOSOutPutPMSyn( portNum, pmList, devIP ) )
		return false;

	if( portIndex>portNum )
		return false;

	pm = pmList[portIndex-1];
	return true;
}

void OLMPro::GetOSWaveLen( ULONG devIP )
{
	ProCommand cmd( 0, 1 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0132 );

	UCHAR rw = 0;
	cmd.SetDataBuf( &rw );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::GetOSPortNumSyn( UCHAR& portNum, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	opParaList.push_back( (UCHAR)0 );
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0132, opParaList, pSynOPdata );

	// 发送请求
	GetOSWaveLen( devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
		portNum = pSynOPdata->_resultList[1];

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

void OLMPro::SetOSWaveLen( UINT portNum, string waveLen[16], ULONG devIP )
{
	int dataLen = 2+portNum;

	ProCommand cmd( 0, dataLen );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0132 );

	UCHAR dataBuf[18];
	dataBuf[0] = 1;
	dataBuf[1] = portNum;

	for( UINT i=0; i<portNum; i++ )
	{
		if( waveLen[i]=="1625" )
			dataBuf[2+i] = 1;
		else if( waveLen[i]=="1550" )
			dataBuf[2+i] = 2;
		else if( waveLen[i]=="1310" )
			dataBuf[2+i] = 3;
	}

	cmd.SetDataBuf( dataBuf );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

void OLMPro::GetOSMaxMinPM( UCHAR monitorFlag, ULONG devIP )
{
	ProCommand cmd( 0, 1 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0135 );

	cmd.SetDataBuf( &monitorFlag );

	// 计算校验和
	cmd.CalSUM();
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::GetOSMaxMinPMSyn( UCHAR monitorFlag1, UCHAR& monitorFlag2, float& max, float& min, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0135, opParaList, pSynOPdata );

	// 发送请求
	GetOSMaxMinPM( monitorFlag1, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		monitorFlag2 = pSynOPdata->_resultList[0];
		max = pSynOPdata->_resultList[1];
		min = pSynOPdata->_resultList[2];
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}

/*	
cmd:
	01 电阻增加1LSB
	02 电阻减小1LSB
	10 电阻值x2
	20 电阻值/2
	44 保存当前电阻值
	55 使用已保存的电阻值
	Other 无操作，用于获得返回值
*/

void OLMPro::SetOSResistance( UCHAR flag, ULONG devIP )
{
	ProCommand cmd( 0, 1 );
	cmd.SetVer( 0x11 );
	cmd.SetCOMM_TCOMM( 0x0136 );
	// 设data域
	cmd.SetDataBuf( &flag );
	cmd.CalSUM();

	//
	SendTo( (char*)&cmd, cmd.GetCmdLen(), devIP );
}

bool OLMPro::SetOSResistanceSyn( UCHAR flag, USHORT& IBMON, USHORT& IMPDMON, USHORT& resistanceLSB, ULONG devIP )
{
	bool rt = true;

	// 插入同步队列
	SynOPData *pSynOPdata = NULL;
	vector<Var> opParaList;
	//
	_synOPLit.SysOPListPushMute( devIP, 0x0136, opParaList, pSynOPdata );

	// 发送请求
	SetOSResistance( flag, devIP );

	// 等待回复
	BEGIN_WAIT()
	END_WAIT_FLAG( 200, pSynOPdata->_stopWaitFlag )
	//
	rt = pSynOPdata->_stopWaitFlag;
	if( rt==true )
	{
		IBMON = pSynOPdata->_resultList[0];
		IMPDMON = pSynOPdata->_resultList[1];
		resistanceLSB = pSynOPdata->_resultList[2];
	}

	// 同步队列中删除同步对象
	_synOPLit.SysOPListPopMute( pSynOPdata );
	return rt;
}













__declspec(dllexport) void ReadProductInfo( ULONG scanIP )
{
	OLMPro::GetSingleton()->ReadProductInfo( scanIP );
}

__declspec(dllexport) void SetSN( UCHAR slot, ULONG sn, ULONG devIP )
{
	OLMPro::GetSingleton()->SetSN( slot, sn, devIP );
}

__declspec(dllexport) void SetIP( UCHAR slot, ULONG ipAddr, ULONG gateIP, ULONG netMask, ULONG devIP )
{
	OLMPro::GetSingleton()->SetIP( slot, ipAddr, gateIP, netMask, devIP );
}

__declspec(dllexport) void SetMAC( UCHAR slot, UCHAR* pMAC, ULONG devIP )
{
	OLMPro::GetSingleton()->SetMAC( slot, pMAC, devIP );
}

__declspec(dllexport) void SetFrameSN( ULONG RackSN, ULONG devIP )
{
	OLMPro::GetSingleton()->SetFrameSN( RackSN, devIP );
}

__declspec(dllexport) bool ReadProductInfoSyn( ULONG devIP, DeviceInfo& devInfo )
{
	return OLMPro::GetSingleton()->ReadProductInfoSyn( devIP, devInfo );
}

__declspec(dllexport) void TestDS1642_E2prom( ULONG devIP )
{
	OLMPro::GetSingleton()->TestDS1642_E2prom( devIP );
}

__declspec(dllexport) void TestDS1642_Temp( ULONG devIP )
{
	OLMPro::GetSingleton()->TestDS1642_Temp( devIP );
}

__declspec(dllexport) void TestBackBoardE2prom( ULONG devIP )
{
	OLMPro::GetSingleton()->TestBackBoardE2prom( devIP );
}

__declspec(dllexport) void TestFan( ULONG devIP )
{
	OLMPro::GetSingleton()->TestFan( devIP );
}

__declspec(dllexport) void TestWatchDog( ULONG devIP )
{
	OLMPro::GetSingleton()->TestWatchDog( devIP );
}

__declspec(dllexport) void TestLed( ULONG devIP )
{
	OLMPro::GetSingleton()->TestLed( devIP );
}

__declspec(dllexport) void TestOPM( USHORT para, ULONG devIP )
{
	OLMPro::GetSingleton()->TestOPM( para, devIP );
}

__declspec(dllexport) void TestOSW( int sel, int outPort, ULONG devIP )
{
	OLMPro::GetSingleton()->TestOSW( sel, outPort, devIP );
}

__declspec(dllexport) void TestI2C0_E2prom( ULONG devIP )
{
	OLMPro::GetSingleton()->TestI2C0_E2prom( devIP );
}

__declspec(dllexport) void TestI2C0_Temp( ULONG devIP )
{
	OLMPro::GetSingleton()->TestI2C0_Temp( devIP );
}

__declspec(dllexport) void ActiveIP( bool activeIP, ULONG devIP )
{
	OLMPro::GetSingleton()->ActiveIP( activeIP, devIP );
}

__declspec(dllexport) void ActiveDownload( bool activeDown, ULONG devIP )
{
	OLMPro::GetSingleton()->ActiveDownload( activeDown, devIP );
}

__declspec(dllexport) void GetOSWStatus( UCHAR mainType, USHORT subType, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOSWStatus( mainType, subType, devIP );
}

__declspec(dllexport) void SetOSWStatus( UCHAR oswType, UCHAR mainType, USHORT subType, USHORT inPortNO, USHORT outPortNO, ULONG devIP )
{
	OLMPro::GetSingleton()->SetOSWStatus( oswType, mainType, subType, inPortNO, outPortNO, devIP );
}

__declspec(dllexport) void SetOSWType( UCHAR oswType, ULONG devIP )
{
	OLMPro::GetSingleton()->SetOSWType( oswType, devIP );
}

__declspec(dllexport) void GetOPMSample( UCHAR mainType, USHORT subType, UCHAR channel, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOPMSample( mainType, subType, channel, devIP );
}

__declspec(dllexport) void GetOPMPowerMeter( UCHAR mainType, USHORT subType, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOPMPowerMeter( mainType, subType, devIP );
}

__declspec(dllexport) void SetOPMWaveLen( UCHAR waveFlag[32], ULONG devIP )
{
	OLMPro::GetSingleton()->SetOPMWaveLen( waveFlag, devIP );
}

__declspec(dllexport) void GetOPMCalbTestPM( UCHAR mainType, USHORT subType, UCHAR port, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOPMCalbTestPM( mainType, subType, port, devIP );
}

__declspec(dllexport) void SendOPMCalData( UCHAR channel, int packetNO, char* pBuf, ULONG devIP )
{
	OLMPro::GetSingleton()->SendOPMCalData( channel, packetNO, pBuf, devIP );
}

__declspec(dllexport) void SetOPMPortNum( UCHAR portNum, ULONG devIP )
{
	OLMPro::GetSingleton()->SetOPMPortNum( portNum, devIP );
}

__declspec(dllexport) bool GetOPMSampleSyn( UCHAR mainType, USHORT subType, UCHAR channel, USHORT &sampleValue, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOPMSampleSyn( mainType, subType, channel, sampleValue, devIP );
}

__declspec(dllexport) bool GetOPMCalbTestPMSyn( UCHAR mainType, USHORT subType, UCHAR port, float& calbTestPM, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOPMCalbTestPMSyn( mainType, subType, port, calbTestPM, devIP );
}

__declspec(dllexport) bool SendOPMCalDataSyn( UCHAR channel, int packetNO, char* pBuf, ULONG devIP )
{
	return OLMPro::GetSingleton()->SendOPMCalDataSyn( channel, packetNO, pBuf, devIP );
}

__declspec(dllexport) void SetOPM2Clear( ULONG devIP )
{
	OLMPro::GetSingleton()->SetOPM2Clear( devIP );
}

__declspec(dllexport) void GetOPM2ADPM( UCHAR startC, UCHAR endC, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOPM2ADPM( startC, endC, devIP );
}

__declspec(dllexport) void SetOPM2Dx( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP )
{
	OLMPro::GetSingleton()->SetOPM2Dx( startC, endC, gear, dx, devIP );
}

	// Synchronous
__declspec(dllexport) bool GetOPM2ADPMSyn( UCHAR startC, UCHAR endC, float pm[32], ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOPM2ADPMSyn( startC, endC, pm, devIP );
}

__declspec(dllexport) bool SetOPM2DxSyn( UCHAR startC, UCHAR endC, UCHAR gear, float dx[32], ULONG devIP )
{
	return OLMPro::GetSingleton()->SetOPM2DxSyn( startC, endC, gear, dx, devIP );
}

__declspec(dllexport) bool SetOPM2ClearSyn( char v[32], ULONG devIP )
{
	return OLMPro::GetSingleton()->SetOPM2ClearSyn( v, devIP );
}

__declspec(dllexport) void GetOSSampleOrPM( bool flag, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOSSampleOrPM( flag, devIP );
}

__declspec(dllexport) void SwitchOS( bool openFlag, ULONG devIP )
{
	OLMPro::GetSingleton()->SwitchOS( openFlag, devIP );
}

__declspec(dllexport) void SetOSCalData( float k, float b, int n, float* pData, ULONG devIP )
{
	OLMPro::GetSingleton()->SetOSCalData( k, b, n, pData, devIP );
}

__declspec(dllexport) void GetOSOutPutPM( ULONG devIP )
{
	OLMPro::GetSingleton()->GetOSOutPutPM( devIP );
}

__declspec(dllexport) void GetOSWaveLen( ULONG devIP )
{
	OLMPro::GetSingleton()->GetOSWaveLen( devIP );
}

__declspec(dllexport) void SetOSWaveLen( UINT portNum, std::string waveLen[16], ULONG devIP )
{
	OLMPro::GetSingleton()->SetOSWaveLen( portNum, waveLen, devIP );
}

__declspec(dllexport) void GetOSMaxMinPM( UCHAR monitorFlag, ULONG devIP )
{
	OLMPro::GetSingleton()->GetOSMaxMinPM( monitorFlag, devIP );
}

__declspec(dllexport) void SetOSResistance( UCHAR flag, ULONG devIP )
{
	OLMPro::GetSingleton()->SetOSResistance( flag, devIP );
} 

__declspec(dllexport) bool GetOSPMSyn( float& pm, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOSPMSyn( pm, devIP );
}

__declspec(dllexport) bool GetOSMaxMinPMSyn( UCHAR monitorFlag1, UCHAR& monitorFlag2, float& max, float& min, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOSMaxMinPMSyn( monitorFlag1, monitorFlag2, max, min, devIP );
}

__declspec(dllexport) bool GetOSSampleSyn( USHORT& sample, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOSSampleSyn( sample, devIP );
}

__declspec(dllexport) bool GetOSPortNumSyn( UCHAR& portNum, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOSPortNumSyn( portNum, devIP );
}

__declspec(dllexport) bool SetOSResistanceSyn( UCHAR flag, USHORT& IBMON, USHORT& IMPDMON, USHORT& resistanceLSB, ULONG devIP )
{
	return OLMPro::GetSingleton()->SetOSResistanceSyn( flag, IBMON, IMPDMON, resistanceLSB, devIP );
} 

__declspec(dllexport) bool GetOSOutPutPMSyn(  UCHAR& portNum, float pmList[16], ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOSOutPutPMSyn( portNum, pmList, devIP );
}

__declspec(dllexport) bool GetOSOutPutPortPMSyn(  UCHAR portIndex, float& pm, ULONG devIP )
{
	return OLMPro::GetSingleton()->GetOSOutPutPortPMSyn( portIndex, pm, devIP );
}

__declspec(dllexport) void StartRecvResponseThread()
{
	OLMPro::GetSingleton()->StartRecvResponseThread();
}