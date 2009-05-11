#include "stdafx.h"
#include "DbgUtil.h"

void MyTrace( LPCWSTR pszFormat, ... )
{
	va_list args;
	CStringW strLog;

	va_start(args, pszFormat);
	strLog.FormatV(pszFormat, args);
	va_end(args);
	OutputDebugStringW(strLog);
}