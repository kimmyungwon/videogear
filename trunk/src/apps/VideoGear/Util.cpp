#include "stdafx.h"
#include "Util.h"

void Trace(LPCTSTR msg, ...)
{
	va_list args;
	CString str;

	va_start(args, msg);
	str.FormatV(msg, args);
	va_end(args);
	OutputDebugString(str);
}

bool operator<(const GUID& _Left, const GUID& _Right)
{
	return (memcmp(&_Left, &_Right, sizeof(GUID)) < 0);
}

void ExtractStrings(TCHAR sep, const CString& str, std::list<CString>& tokens)
{
	int startpos = 0, pos = 0;
	
	while (startpos < str.GetLength())
	{
		pos = str.Find(sep, startpos);
		if (pos >= 0)
		{
			tokens.push_back(str.Mid(startpos, pos - startpos));
			startpos = pos + 1;
		}
		else
		{
			tokens.push_back(str.Mid(startpos));
			break;
		}
	}	
}

void HexToBin(const CString& str, std::string& bytes)
{
	int pos = 0;

	while (str.GetLength() - pos >= 2)
	{
		bytes.push_back((char)_tcstol(str.Mid(pos, 2), NULL, 16));
		pos += 2;
	}
}

bool IsPinConnected(IPin* pin)
{
	ASSERT(pin != NULL);

	IPin* pinTo;

	return pin->ConnectedTo(&pinTo) == S_OK;
}

PIN_DIRECTION GetPinDir(IPin* pin)
{
	ASSERT(pin != NULL);
	
	PIN_DIRECTION pinDir;

	pin->QueryDirection(&pinDir);
	return pinDir;
}