#pragma once

#include "..\Utility\GXUtil.h"
#include "visa.h"
#pragma comment( lib, "visa32.lib" )


class Laser816XBase
{
public:
	Laser816XBase();
	~Laser816XBase();

	std::string _addrStr;

protected:
	// 发送命令
	inline bool SendCmd( std::string cmd );

	// gpib操作
	bool IsOpenGPIB();
	bool OpenGPIB();
	void CloseGPIB();	

	ViSession _defaultRM;
	ViSession _vi; 
};

class Laser8164B : public Laser816XBase
{
public:
	// 打开关闭激光器
	bool Open();
	void Close();
	
	// 功能
	bool SetWavelength( std::string waveLen );			// 设置波长
	bool SetPowerDB( float db );
	bool SetTrigOutpStf();								// 设置脉冲触发模式为step finished
	// sweep相关
	bool SetSweMode( int flag=0 );						// 0 cont | 1 step | 2 man
	bool SetSweCyc( int cyc=1 );						// 设置循环圈数
	bool SetSweStarStop( float star, float stop );		// 设置sweep的波长起始范围
	bool SetSweSpeedStep( float spe, float step );		// 设置sweep的速度 步长
	bool SetSweStart();
	bool SetSweStop();
	bool SetSwePause();
	bool SetSweContinue();
	// 日记操作
	bool OpenLog();
	bool CloseLog();
	// 读取数据
	int ReadLogDataPointNum();							// -1代表失败
	bool ReadLogData( std::vector<double>& channelList );

	// sweep综合功能 con模式
	bool SweContModeTest( float start, float stop, float speed, float step, std::vector<double>& channelList );

protected:
	// 激光器锁定解锁
	bool Lock();
	bool UnLock();

	// 激光器出光
	bool PowStat( bool stat=true );
};


// [1520, 1630]
class Laser8163B : public Laser816XBase
{
public:
	// 打开关闭激光器
	bool Open();
	void Close();
	
	// 功能
	bool SetWavelength( std::string waveLen );			// 设置波长
	bool SetPowerDB( float db );
};