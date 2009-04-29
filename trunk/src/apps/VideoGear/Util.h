#pragma once

#include "OpenArray.h"

#ifdef _DEBUG
#define XTRACE Trace
#else
#define XTRACE __noop
#endif // _DEBUG

/* µ÷ÊÔ */
void Trace(LPCTSTR msg, ...);
/* ²Ù×÷·û */
bool operator<(const GUID& _Left, const GUID& _Right);
/* ×Ö·û´® */
//void ExtractStrings(TCHAR sep, const CString& str, std::list<CString>& tokens);
//void HexToBin(const CString& str, std::string& bytes);
/* Pin */
bool IsPinConnected(IPin* pin);
PIN_DIRECTION GetPinDir(IPin* pin);