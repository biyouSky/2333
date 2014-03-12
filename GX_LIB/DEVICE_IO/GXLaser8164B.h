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
	// ��������
	inline bool SendCmd( std::string cmd );

	// gpib����
	bool IsOpenGPIB();
	bool OpenGPIB();
	void CloseGPIB();	

	ViSession _defaultRM;
	ViSession _vi; 
};

class Laser8164B : public Laser816XBase
{
public:
	// �򿪹رռ�����
	bool Open();
	void Close();
	
	// ����
	bool SetWavelength( std::string waveLen );			// ���ò���
	bool SetPowerDB( float db );
	bool SetTrigOutpStf();								// �������崥��ģʽΪstep finished
	// sweep���
	bool SetSweMode( int flag=0 );						// 0 cont | 1 step | 2 man
	bool SetSweCyc( int cyc=1 );						// ����ѭ��Ȧ��
	bool SetSweStarStop( float star, float stop );		// ����sweep�Ĳ�����ʼ��Χ
	bool SetSweSpeedStep( float spe, float step );		// ����sweep���ٶ� ����
	bool SetSweStart();
	bool SetSweStop();
	bool SetSwePause();
	bool SetSweContinue();
	// �ռǲ���
	bool OpenLog();
	bool CloseLog();
	// ��ȡ����
	int ReadLogDataPointNum();							// -1����ʧ��
	bool ReadLogData( std::vector<double>& channelList );

	// sweep�ۺϹ��� conģʽ
	bool SweContModeTest( float start, float stop, float speed, float step, std::vector<double>& channelList );

protected:
	// ��������������
	bool Lock();
	bool UnLock();

	// ����������
	bool PowStat( bool stat=true );
};


// [1520, 1630]
class Laser8163B : public Laser816XBase
{
public:
	// �򿪹رռ�����
	bool Open();
	void Close();
	
	// ����
	bool SetWavelength( std::string waveLen );			// ���ò���
	bool SetPowerDB( float db );
};