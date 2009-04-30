#include "stdafx.h"
#include "StrUtils.h"

bool StartsTextW(const TCHAR* substr, const TCHAR* str)
{
	return (wcslen(str) >= wcslen(substr) && _wcsnicmp(str, substr, wcslen(substr)) == 0);
}