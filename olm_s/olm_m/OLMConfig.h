
#pragma once



struct OLMConfig
{
	static OLMConfig* GetSingleton();
	static void Destroy();

	OLMConfig();

	ULONG _selDevIP;
	std::string _devType;

private:
	static OLMConfig* _pSingleton;
};




extern "C" __declspec(dllexport) ULONG& GetSelDevIP();	
extern "C" __declspec(dllexport) std::string& GetSelDevType();