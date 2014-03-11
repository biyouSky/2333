// ESPC: MC-711
/*
	ESPC提供的驱动库是基于MFC 限制极大
*/

#pragma once
#include "..\Utility\GXUtil.h"

#include "Espec.h"
#pragma	comment(lib, "EspecDll.lib")

class TBMC711
{
	static int _obCount;					// 对象计数
	DWORD _macID;							// 当前箱子macID

	static int CALLBACK MessageProc( DWORD msgID, DWORD macID, long *lpParam );

	bool InitSerial( DWORD com );			// 初始化总串口

	bool Stop();							// 停止
	bool Run();								// 运行
	bool IsRunning();

public:
	TBMC711( DWORD macID=0, DWORD com=1 );
	~TBMC711();
	
	bool SetSetTem( float tem );			// 设置目标温度
	bool GetSetTem( float& tem );			// 获取目标温度
	bool GetActTem( float& tem );			// 获取当前温度

	//bool SetWaitTem( float tem, DWORD timeout = 40*60 );			// 	设置温度并等待
};


