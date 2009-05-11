#pragma once

#ifdef _DEBUG
#define XTRACE MyTrace
#else
#define XTRACE __noop
#endif // _DEBUG

void MyTrace(LPCWSTR pszFormat, ...);