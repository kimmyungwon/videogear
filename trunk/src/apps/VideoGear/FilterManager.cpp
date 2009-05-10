#include "StdAfx.h"
#include "FilterManager.h"
#include "..\..\filters\parser\avisplitter\AviSplitter.h"
#include "..\..\filters\parser\matroskasplitter\MatroskaSplitter.h"
#include "..\..\filters\transform\mpcvideodec\MPCVideoDecFilter.h"

template<typename ClassT>
HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv)
{
	HRESULT hr;
	ClassT* pObj = new ClassT(pUnk, &hr);
	if (SUCCEEDED(hr))
	{
		pObj->AddRef();
		*ppv = (IBaseFilter*)pObj;
	}
	else
	{
		delete pObj;
		*ppv = NULL;
	}
	return hr;
}

namespace Avi
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_Avi},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};
}

namespace VideoDec
{
	const AMOVIESETUP_PIN sudpPinsVideoDec[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesInCount,  CMPCVideoDecFilter::sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesOutCount, CMPCVideoDecFilter::sudPinTypesOut}
	};
}

//////////////////////////////////////////////////////////////////////////

CFilterManager::CFilterManager(void)
{
	RegisterFilter(L"MPC - Avi Splitter", CreateInstance<CAviSplitterFilter>, Avi::sudpPins, _countof(Avi::sudpPins));
	RegisterFilter(L"MPC - Video decoder", CreateInstance<CMPCVideoDecFilter>, VideoDec::sudpPinsVideoDec, _countof(VideoDec::sudpPinsVideoDec));
}

CFilterManager::~CFilterManager(void)
{
}

HRESULT CFilterManager::RegisterFilter( LPCWSTR pszName, PFNCreateInstanceProc pfnCreateInst, const AMOVIESETUP_PIN* pPins, size_t nPinCount )
{
	return E_NOTIMPL;
}