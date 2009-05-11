#include "stdafx.h"
#include "DbgUtil.h"

void MyTrace(LPCWSTR pszFormat, ...)
{
	CStringW strLog;
	va_list args;

	va_start(args, pszFormat);
	strLog.FormatV(pszFormat, args);
	va_end(args);
	OutputDebugStringW(strLog);
}