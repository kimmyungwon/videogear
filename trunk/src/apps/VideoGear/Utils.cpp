#include "stdafx.h"
#include "Utils.h"

void HexToBin(LPCTSTR str, std::string& bytes)
{
	LPCTSTR ptr;
	TCHAR byte[3];
	
	ptr = str;
	while (_tcslen(ptr) >= 2)
	{
		ZeroMemory(byte, sizeof(byte));
		_tcsncpy(byte, ptr, 2);
		ptr += 2;
		bytes.push_back((char)_tcstoul(byte, NULL, 16));
	}
}

bool IsPinConnected(IPin* pin)
{
	ASSERT(pin != NULL);

	IPin* pinTo;

	return (SUCCEEDED(pin->ConnectedTo(&pinTo)) && pinTo != NULL);
}

PIN_DIRECTION GetPinDir(IPin* pin)
{
	ASSERT(pin != NULL);

	PIN_DIRECTION pinDir;

	pin->QueryDirection(&pinDir);
	return pinDir;
}