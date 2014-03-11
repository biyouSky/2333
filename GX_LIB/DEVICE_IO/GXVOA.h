
#pragma once

#include "..\Communication_IO\GXSerial.h"

class VOAXX : public SerialOBJ
{
public:
	VOAXX();

	float GetVOA();
	void ClearVOA();
	void SetVOAAbsoluteAtten( float attenuation );
	void SetVOARelativeAtten( float attenuation );
	void SendVOACmd();

	std::string _waveLen;					// 0:1310 	1:1550  

protected:
	bool ParaseRes( std::string& res );

	float _voaValue;
};