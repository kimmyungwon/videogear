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
	ASSERT(pFilter != NULL);

	BeginEnumPins(pFilter, pEnumPins, pPin)
	{
		if (IsPinDir(pPin, PINDIR_INPUT))
			return false;
	}
	EndEnumPins;
	return true;
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