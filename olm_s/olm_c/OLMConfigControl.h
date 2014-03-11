
#pragma once

#define SELDEV_IP		GetSelDevIP()
#define SELDEV_TYPE		GetSelDevType()


typedef ULONG& (*PFunc_Model_Type101)();	
typedef std::string& (*PFunc_Model_Type102)();														


extern PFunc_Model_Type101 GetSelDevIP;
extern PFunc_Model_Type102 GetSelDevType;





bool LoadOLMConfigDLL();


