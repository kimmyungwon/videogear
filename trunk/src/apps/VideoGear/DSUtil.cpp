#include "stdafx.h"
#include "VideoGear.h"
#include "DSUtil.h"

HRESULT AddToROT(IFilterGraph *pFG)
{
	if (pFG == NULL)
		return E_POINTER;
	
	CComPtr<IRunningObjectTable> pROT;
	CStringW strName;
	CComPtr<IMoniker> pMoniker;
	DWORD dwRegister;

	RIF(GetRunningObjectTable(0, &pROT));
	strName.Format(L"FilterGraph %08x pid %08x", (DWORD_PTR)pFG, GetCurrentProcessId());
	RIF(CreateItemMoniker(L"!", strName, &pMoniker));
	return pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pFG, pMoniker, &dwRegister);
}

bool IsSourceFilter(IBaseFilter *pFilter)
{
	int nInPins, nOutPins, nInCPins, nOutCPins;
	
	CountPins(pFilter, nInPins, nOutPins, nInCPins, nOutCPins);
	return nInPins == 0 && nOutPins == 1;
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

void CStringToBin(const CStringW &str, CStringA &bytes)
{
	ASSERT(str.GetLength() % 2 == 0);

	LPCWSTR pszSrc = str;
	wchar_t szNum[3];
	char cVal;

	bytes.Empty();
	while (*pszSrc != 0)
	{
		wcsncpy_s(szNum, _countof(szNum), pszSrc, 2);
		cVal = (char)wcstol(szNum, NULL, 16);
		bytes += cVal;
		pszSrc += 2;
	}
}