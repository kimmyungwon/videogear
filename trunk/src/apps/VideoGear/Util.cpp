#include "stdafx.h"
#include "Util.h"

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

void HexToBin(const CString& str, CStringA& bytes)
{
	int pos = 0;

	while (str.GetLength() - pos >= 2)
	{
		bytes.AppendChar((_TCHAR)_tcstol(str.Mid(pos, 2), NULL, 16));
		pos += 2;
	}
}