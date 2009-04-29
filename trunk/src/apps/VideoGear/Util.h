#pragma once

#include "OpenArray.h"

/* ������ */
bool operator<(const GUID& _Left, const GUID& _Right);
/* �ַ��� */
void ExtractStrings(TCHAR sep, const CString& str, std::list<CString>& tokens);
void HexToBin(const CString& str, CStringA& bytes);