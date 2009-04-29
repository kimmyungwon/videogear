#pragma once

#include "OpenArray.h"

/* ²Ù×÷·û */
bool operator<(const GUID& _Left, const GUID& _Right);
/* ×Ö·û´® */
void ExtractStrings(TCHAR sep, const CString& str, std::list<CString>& tokens);
void HexToBin(const CString& str, CStringA& bytes);