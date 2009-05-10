#include "StdAfx.h"
#include "VideoGear.h"
#include "FGManager.h"
#include "DSUtil.h"

CFGManager::CFGManager(void)
: CUnknown(L"CFGManager", NULL)
{
	m_pGraph.CoCreateInstance(CLSID_FilterGraph);
}

CFGManager::~CFGManager(void)
{
	m_pGraph.Release();
}

HRESULT STDMETHODCALLTYPE CFGManager::NukeDownstream( IPin *ppinOut )
{
	if (ppinOut == NULL)
		return E_POINTER;

	CComPtr<IPin> ppinTo;
	PIN_INFO piTo;
	CComPtr<IEnumPins> pEnumPins;

	RIF(ppinOut->ConnectedTo(&ppinTo));
	RIF(ppinTo->QueryPinInfo(&piTo));
	RIF(piTo.pFilter->EnumPins(&pEnumPins));
	for (CComPtr<IPin> pPin; pEnumPins->Next(1, &pPin, NULL) == S_OK; pPin.Release())
	{
		RIF(Disconnect(pPin));
	}
	RIF(RemoveFilter(piTo.pFilter));
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderFilter( IBaseFilter *pFilter )
{
	if (pFilter == NULL)
		return E_POINTER;
	
	CComPtr<IEnumPins> pEnumPins;
	int nTotal = 0, nRendered = 0;

	RIF(pFilter->EnumPins(&pEnumPins));
	for (CComPtr<IPin> pPin; pEnumPins->Next(1, &pPin, NULL) == S_OK; pPin.Release())
	{
		if (!IsPinDir(pPin, PINDIR_OUTPUT) || IsPinConnected(pPin))
			continue;
		nTotal++;
		if (SUCCEEDED(Render(pPin)))
			nRendered++;
	}
	if (nTotal > 0)
		if (nRendered > 0)
			return nRendered == nTotal ? S_OK : S_FALSE;
		else
			return VFW_E_CANNOT_RENDER;
	else
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CFGManager::ConnectDirectEx( IPin *ppinOut, IBaseFilter *pFilter, const AM_MEDIA_TYPE *pmt )
{
	if (ppinOut == NULL || pFilter == NULL)
		return E_POINTER;
	if (IsPinConnected(ppinOut))
		return VFW_E_ALREADY_CONNECTED;

	CComPtr<IEnumPins> pEnumPins;

	RIF(pFilter->EnumPins(&pEnumPins));
	for (CComPtr<IPin> ppinIn; pEnumPins->Next(1, &ppinIn, NULL) == S_OK; ppinIn.Release())
	{
		if (!IsPinDir(ppinIn, PINDIR_INPUT) || IsPinConnected(ppinIn))
			continue;
		HRESULT hr = ConnectDirect(ppinOut, ppinIn, pmt);
		if (SUCCEEDED(hr))
			return hr;
	}
	return VFW_E_CANNOT_CONNECT;
}

STDMETHODIMP CFGManager::NonDelegatingQueryInterface( REFIID riid, __deref_out void **ppv )
{
	return	QI(IFilterGraph2)
			QI(IGraphBuilder)
			QI(IFilterGraph)
			m_pGraph->QueryInterface(riid, ppv);
}

HRESULT STDMETHODCALLTYPE CFGManager::AddFilter( IBaseFilter *pFilter, LPCWSTR pName )
{
	return m_pGraph->AddFilter(pFilter, pName);
}

HRESULT STDMETHODCALLTYPE CFGManager::RemoveFilter( IBaseFilter *pFilter )
{
	return m_pGraph->RemoveFilter(pFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::EnumFilters( IEnumFilters **ppEnum )
{
	return m_pGraph->EnumFilters(ppEnum);
}

HRESULT STDMETHODCALLTYPE CFGManager::FindFilterByName( LPCWSTR pName, IBaseFilter **ppFilter )
{
	return m_pGraph->FindFilterByName(pName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt)
{
	return m_pGraph->ConnectDirect(ppinOut, ppinIn, pmt);
}

HRESULT STDMETHODCALLTYPE CFGManager::Reconnect(IPin *ppin)
{
	return m_pGraph->Reconnect(ppin); 
}

HRESULT STDMETHODCALLTYPE CFGManager::Disconnect(IPin *ppin)
{
	return m_pGraph->Disconnect(ppin);
}

HRESULT STDMETHODCALLTYPE CFGManager::SetDefaultSyncSource(void)
{
	return m_pGraph->SetDefaultSyncSource();
}

HRESULT STDMETHODCALLTYPE CFGManager::Connect(IPin *ppinOut, IPin *ppinIn)
{
	return m_pGraph->Connect(ppinOut, ppinIn);
}

HRESULT STDMETHODCALLTYPE CFGManager::Render(IPin *ppinOut)
{
	if (ppinOut == NULL)
		return E_POINTER;
	if (IsPinConnected(ppinOut))
		return VFW_E_ALREADY_CONNECTED;

	IBaseFilter* pFilterOut;
	CComPtr<IEnumFilters> pEnumFilters;
	CAtlList<CMediaType> mtsOut;
	std::list<CFilter*> filters;

	/* ƥ�������˾� */

	pFilterOut = GetFilterFromPin(ppinOut);
	if (SUCCEEDED(EnumFilters(&pEnumFilters)))
	{
		for (CComPtr<IBaseFilter> pFilter; pEnumFilters->Next(1, &pFilter, NULL) == S_OK; pFilter.Release())
		{
			if (pFilterOut == pFilter)
				continue;
			if (SUCCEEDED(ConnectDirectEx(ppinOut, pFilter, NULL)))
				return S_OK;
		}
	}
	
	/* ƥ���ڲ��˾� */

	ExtractMediaTypes(ppinOut, mtsOut);
	if(theApp.m_pFilterManager->EnumMatchingFilters(mtsOut, filters) == S_OK)
	{
		for (std::list<CFilter*>::iterator it = filters.begin(); it != filters.end(); it++)
		{
			CComPtr<IBaseFilter> pFilter;

			if (FAILED((*it)->CreateInstance(NULL, &pFilter))
				|| FAILED(AddFilter(pFilter, (*it)->GetName())))
				continue;
			if (SUCCEEDED(ConnectDirectEx(ppinOut, pFilter, NULL))
				&& SUCCEEDED(RenderFilter(pFilter)))
			{
				return S_OK;
			}
			NukeDownstream(ppinOut);
		}
	}

	return m_pGraph->Render(ppinOut);
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList)
{
	CComPtr<IBaseFilter> pSource;
	HRESULT hr;
	
	RIF(AddSourceFilter(lpcwstrFile, NULL, &pSource));
	if (SUCCEEDED(hr = RenderFilter(pSource)))
	{	
		return hr;
	}
	else
	{
		return VFW_E_CANNOT_RENDER;
	}
}

HRESULT STDMETHODCALLTYPE CFGManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	return m_pGraph->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::SetLogFile(DWORD_PTR hFile)
{
	return m_pGraph->SetLogFile(hFile);
}

HRESULT STDMETHODCALLTYPE CFGManager::Abort(void)
{
	return m_pGraph->Abort();
}

HRESULT STDMETHODCALLTYPE CFGManager::ShouldOperationContinue(void)
{
	return m_pGraph->ShouldOperationContinue();
}

HRESULT STDMETHODCALLTYPE CFGManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	return m_pGraph->AddSourceFilterForMoniker(pMoniker, pCtx, lpcwstrFilterName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt)
{
	return m_pGraph->ReconnectEx(ppin, pmt);
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext)
{
	return m_pGraph->RenderEx(pPinOut, dwFlags, pvContext);
}

