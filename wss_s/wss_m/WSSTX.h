#pragma once

#include "WSSDevice.h"

typedef void (*PFunc_View_Type1000)( ULONG wssKey, const char* msg );
extern PFunc_View_Type1000 PCB_View_ShowWSSMsg;	


void ParseOSATrace( std::string& str, VECTOR_FLOAT& vList );

void SaveSLData( std::string& fileName, std::string& trace );

void LoadSLData( std::string& fileName, VECTOR_FLOAT& vList );


void WaitingTempStable_Com( VECTOR_INT comList, VECTOR_FLOAT& stableTempList );
void WaitingTempStable_Net( VECTOR_INT ipList, VECTOR_FLOAT& stableTempList );