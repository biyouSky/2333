// ESPC: MC-711
/*
	ESPC�ṩ���������ǻ���MFC ���Ƽ���
*/

#pragma once
#include "..\Utility\GXUtil.h"

#include "Espec.h"
#pragma	comment(lib, "EspecDll.lib")

class TBMC711
{
	static int _obCount;					// �������
	DWORD _macID;							// ��ǰ����macID

	static int CALLBACK MessageProc( DWORD msgID, DWORD macID, long *lpParam );

	bool InitSerial( DWORD com );			// ��ʼ���ܴ���

	bool Stop();							// ֹͣ
	bool Run();								// ����
	bool IsRunning();

public:
	TBMC711( DWORD macID=0, DWORD com=1 );
	~TBMC711();
	
	bool SetSetTem( float tem );			// ����Ŀ���¶�
	bool GetSetTem( float& tem );			// ��ȡĿ���¶�
	bool GetActTem( float& tem );			// ��ȡ��ǰ�¶�

	//bool SetWaitTem( float tem, DWORD timeout = 40*60 );			// 	�����¶Ȳ��ȴ�
};


