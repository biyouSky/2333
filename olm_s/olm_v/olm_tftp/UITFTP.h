#pragma once


class TFTPLogic
{
public:
	TFTPLogic():_udp(69)	{}

	virtual void ShowMsg( std::string str )=0;

	bool StartTFTPWrite( std::string fileFullPath, ULONG tftpSerAddr )
	{
		std::string fileName(fileFullPath.begin()+fileFullPath.find_last_of('\\')+1, fileFullPath.end());
		VECTOR_STRING wordList;
		VECTOR_CHAR splitList;
		splitList.push_back( '_' );
		SplitWords( fileName, wordList, splitList );
		if( wordList.size()>1 )
		{
			string& word = wordList[2];
			if( word.substr( 0, 3 )=="OPM" )
				fileName = "app_opm.bin";
			else if( word.substr( 0, 3 )=="OSW" )
				fileName = "app_osw.bin";
			else if( word.substr( 0, 2 )=="OS" )
				fileName = "app_os.bin";
			else if( word.substr( 0, 6 )=="ROUTER" )
				fileName = "app_router.bin";
		}

		// 打开文件
		TxtOBJ binOBJ;
		if( !binOBJ.OpenFile( fileFullPath ) )
		{
			::MessageBoxA( NULL, "文件打开失败", "告警", 0 );
			return false;
		}
		ULONG fileLen = (ULONG)binOBJ.GetFileSize();

		// 分配缓存  读取文件到缓存中
		char* pCacheBuf = new char[fileLen];
		memset( pCacheBuf, 0, fileLen );
		//
		if( binOBJ.ReadBuf( pCacheBuf, fileLen )!=fileLen )
		{
			::MessageBoxA( NULL, "文件读取失败", "告警", 0 );
			return false;
		}

		// 计算传送数据包数量
		_LONGLONG count = fileLen/512 + 1;

		// 开始FTFP写过程
		int offset = 0;
		int rt = -1;

		// 发送写请求
		_showMsg = "tftp发送写文件请求..";
		memset( _sendBuf, 0, 1024 );
		memset( _recvBuf, 0, 1024 );
		// 
		_sendBuf[1] = 2;
		offset = 2;
		// 文件名
		memcpy( _sendBuf+offset, fileName.c_str(), fileName.size() );
		offset += fileName.size();
		_sendBuf[offset] = 0;
		++offset;
		// 模式
		memcpy( _sendBuf+offset, "octet", 5 );
		offset += 5;
		_sendBuf[offset] = 0;
		++offset;
		//
		int i;
		for( i=0; i<3; ++i )
		{
			_udp.SendTo(_sendBuf, offset, tftpSerAddr);
			rt = _udp.RecvFrom(_recvBuf, 1024, 3000);
			if( rt==4 && _recvBuf[3]==0 )
				break;
		}
		if( i==3 )
		{
			_showMsg = "tftp等待写文件请求回应超时失败..";
			goto FAILE_RETURN;
		}

		// 发送数据包
		for( ULONG i=0; i<count; ++i )
		{
			STRING_FORMAT( _showMsg, "tftp发送第%d/%d个包..", i+1, count );
			int j;
			for( j=0; j<3; ++j )
			{
				memset( _sendBuf, 0, 1024 );
				memset( _recvBuf, 0, 1024 );
				// 
				_sendBuf[1] = 3;		// data
				_sendBuf[3] = (char)i+1;		// 块编号
				offset = 4;

				if( i!=count-1 )
				{
					memcpy( _sendBuf+4, pCacheBuf+i*512, 512 );
					offset += 512;
				}
				else
				{
					memcpy( _sendBuf+4, pCacheBuf+i*512, fileLen-i*512 );
					offset += fileLen-i*512;
				}
				
				_udp.SendTo( _sendBuf, offset, tftpSerAddr );
				rt = _udp.RecvFrom( _recvBuf, 1024, 1500 );
				if( rt==4 && _recvBuf[3]==i+1 )
					break;
			}
			if(j==3)
			{
				_showMsg = "tftp等待写数据包回应超时失败..";
				goto FAILE_RETURN;
			}
		}
		// 
		delete []pCacheBuf;
		_showMsg = "tftp写文件完成..";
		ShowMsg( _showMsg );
		return true;

FAILE_RETURN:
		delete []pCacheBuf;
		ShowMsg( _showMsg );
		return false;
	}

	std::string _showMsg;

private:
	UdpOBJ _udp;
	char _sendBuf[1024];
	char _recvBuf[1024];
};


class UITFTP : public UIWindow<UITFTP>, public _QueueThreadManage<UITFTP>, public TFTPLogic
{
	friend UIWindow;

public:
	void ShowMsg( std::string str );

private:
	void OnCreate();
	void OnNotify( int id, int param );
	//
	void OnActivateDownload();
	void OnGetBinPath();
	void OnStartTFTP();

	void StartTFTPThread();

	UIButton _but1;
	UIButton _but2;
	UIButton _but3;

	UILable _lable1;
	UILable _lable2;
	UILable _lable3;
	
	UIEdit _edit1;
	UIComboBox _combo1;

	UILayoutGrid _layoutGrid1;
};
extern UITFTP gUITFTP;




