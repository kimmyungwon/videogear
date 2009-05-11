#pragma once

#include "..\..\dsutil\DSUtil.h"

#ifdef _DEBUG
#define XTRACE MyTrace
#else
#define XTRACE __noop
#endif // _DEBUG

void MyTrace(LPCWSTR pszFormat, ...);
bool IsPinConnected(IPin *pPin);
bool IsPinDir(IPin *pPin, PIN_DIRECTION dir);