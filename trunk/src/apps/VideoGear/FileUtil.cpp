#include "stdafx.h"
#include "FileUtil.h"

CStringW ExpandFileName(LPCWSTR pszFile)
{
	if (PathIsRelativeW(pszFile))
	{
		WCHAR szNewPath[MAX_PATH], szRet[MAX_PATH];

		GetCurrentDirectoryW(_countof(szNewPath), szNewPath);
		PathAppend(szNewPath, pszFile);
		PathCanonicalizeW(szRet, szNewPath);
		return CStringW(szRet);
	}
	else
		return CStringW(pszFile);
}