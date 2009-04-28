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

bool IsConnected(IPin* pin)
{
	ASSERT(pin != NULL);

	IPin* pinTo;

	return (SUCCEEDED(pin->ConnectedTo(&pinTo)) && pinTo != NULL);
}