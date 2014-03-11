#pragma once


extern "C" __declspec(dllexport) char* PlugINInfo();

// ��ʼ�� ����
extern "C" __declspec(dllexport) void PlugINInit( UIDXInit* pPara1, UIAnimateManage* pPara2, UIMessageLoopManage* pPara3, UICamera* pPara4, UIRender* pPara5, UIWinShell* pPara6 );
extern "C" __declspec(dllexport) void PlugINCreate( UIContainer* pPara1, RECT* pPara2 );

// �������
extern "C" __declspec(dllexport) void PlugINSetShow( bool );
extern "C" __declspec(dllexport) void PlugINDraw();

// menu��Ϣ
extern "C" __declspec(dllexport) std::string* PlugINMenu( int& num );

extern "C" __declspec(dllexport) void PlugINMenuNotify( int param );

// tile��Ϣ
extern "C" __declspec(dllexport) char* PlugINTileTitle();
extern "C" __declspec(dllexport) int PlugINTileImageID();