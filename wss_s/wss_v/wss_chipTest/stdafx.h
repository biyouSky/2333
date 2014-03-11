// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include "..\..\..\GX_LIB\GXUI\UIWinShell.h"
#include "..\..\..\GX_LIB\GXUI\UIPlugIN.h"
#include "..\..\..\GX_LIB\GXUI\UI3D.h"

#include "..\..\..\GX_LIB\Communication_IO\GXSerial.h"
#include "..\..\..\GX_LIB\Communication_IO\GXUDP.h"

#include "..\..\..\GX_LIB\FILE_IO\GXTXT.h"
#include "..\..\..\GX_LIB\Temporary\ThreadLogic.h"

#include "..\..\..\GX_LIB\MATH_ANALYZE\GXWaveMath.h"

#include "..\..\..\GX_LIB\DEVICE_IO\GXLaser8164B.h"
#include "..\..\..\GX_LIB\DEVICE_IO\GXTempBoxMC711.h"

#include "..\..\wss_c\WSSProBaseControl.h"
#include "..\..\wss_c\WSSConfigControl.h"


using namespace std;