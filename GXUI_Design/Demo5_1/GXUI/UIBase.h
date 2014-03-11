
#pragma once

#include "UIUtil.h"


extern cnCriticalSection gDXCS;

/*-------------------------------------------------------DirectUI ��Ϣѭ������-------------------------------------------------------*/
// ��Ϣ����ʽ
class UIWinBase;
struct UIMessage
{
	UIWinBase* _pWin;
	UINT _msg;
	WPARAM _wParam;
	LPARAM _lParam;

	UIMessage();
	UIMessage( UIWinBase* pWin, UINT message, WPARAM wParam, LPARAM lParam );
};



// DirectUI ��Ϣѭ���������  �����¼�ȫ��������Ϣѭ���� �����л��ƴ���  ����ʵ���˲�������
class UIMessageLoopManage : public _MultiThreadManage<UIMessageLoopManage>
{
public:
	static UIMessageLoopManage* GetSingleton();
	static void Destroy();
	
	void RunMessageLoop();
	friend void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );

private:
	UIMessageLoopManage();
	~UIMessageLoopManage();

	void RunMessageLoopThread( void* pData );

	void PushMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );
	bool PopMessage( UIMessage& msg ); 

	bool HandleMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );

	static UIMessageLoopManage* _pSingleton;

	std::deque<UIMessage> _uiMsgList;		// UI��Ϣ����
	cnCriticalSection _uiMsgListCS;
};
void UIPostMessage( UIWinBase* winPoint, UINT message, WPARAM wParam, LPARAM lParam );
void UIRefresh( bool immediately=true );
bool UIMessageFilter1( UINT message );		// ����UI��Ϣѭ������
