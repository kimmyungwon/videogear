#include "stdafx.h"
#include "DSUtil.h"

void MyTrace(LPCWSTR pszFormat, ...)
{
	CStringW strLog;
	va_list args;

	va_start(args, pszFormat);
	strLog.FormatV(pszFormat, args);
	va_end(args);
	OutputDebugStringW(strLog);
}

bool IsPinConnected(IPin *pPin)
{
	ASSERT(pPin != NULL);

	CComPtr<IPin> ppinTo;

	return SUCCEEDED(pPin->ConnectedTo(&ppinTo)) && ppinTo != NULL;
}

bool IsPinDir(IPin *pPin, PIN_DIRECTION dir)
{
	ASSERT(pPin != NULL);

	PIN_DIRECTION pinDir;

	return SUCCEEDED(pPin->QueryDirection(&pinDir)) && pinDir == dir;
}