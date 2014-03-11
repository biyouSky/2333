
#pragma once

#include "WinShell.h"



typedef void (*PFunc_PlugIN_Init)( UIDXInit* pPara1, UIAnimateManage* pPara2, UIMessageLoopManage* pPara3, UICamera* pPara4, UIRender* pPara5, UIWinShell* pPara6 );
typedef void (*PFunc_PlugIN_Create)( UIContainer* pPara1, RECT* pPara2 );

