#include "StdAfx.h"
#include "VGFilterManager.h"

CEnumFilter::CEnumFilter( void )
{
	m_iter = m_items.end();
}

void CEnumFilter::Add( const AMOVIESETUP_FILTER* pFilter )
{
	m_items.insert(pFilter);
	Reset();
}

size_t CEnumFilter::GetCount( void )
{
	return m_items.size();
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Next( ULONG celt, GUID *rgelt, ULONG *pceltFetched )
{
	CheckPointer(rgelt, E_POINTER);
	ValidateReadWritePtr(rgelt, celt * sizeof(GUID));
	if (m_iter == m_items.end())
		return E_FAIL;

	ULONG nDone = 0;
	while (m_iter != m_items.end() && nDone < celt)
	{
		rgelt[nDone] = *(*m_iter)->clsID;
		m_iter++;
		nDone++;
	}	
	return nDone == celt ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Skip( ULONG celt )
{
	if (m_iter == m_items.end())
		return E_FAIL;

	ULONG nDone = 0;
	while (m_iter != m_items.end() && nDone < celt)
	{
		m_iter++;
		nDone++;
	}
	return nDone == celt ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Reset( void )
{
	m_iter = m_items.begin();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Clone( IEnumGUID **ppenum )
{
	CEnumFilter *pEnum = new CEnumFilter;
	pEnum->m_items = m_items;
	pEnum->m_iter = m_iter;
	pEnum->AddRef();
	(*ppenum) = pEnum;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

extern CFactoryTemplate *g_pTemplates;
extern int g_cTemplates;

CVGFilterManager::CVGFilterManager(void)
{
	const AMOVIESETUP_FILTER *pMSF = NULL;
	const AMOVIESETUP_PIN *pMSP= NULL;
	const AMOVIESETUP_MEDIATYPE *pMSMT = NULL;
	
	// 注册内部滤镜
	for (int i=0; i<g_cTemplates; i++)
	{
		m_lookupFlt[*g_pTemplates[i].m_ClsID] = &g_pTemplates[i];
		pMSF = g_pTemplates[i].m_pAMovieSetup_Filter;
		for (UINT j=0; j<pMSF->nPins; j++)
		{
			pMSP = &pMSF->lpPin[j];
			if (pMSP->bOutput)
				continue;
			for (UINT k=0; k<pMSP->nMediaTypes; k++)
			{
				pMSMT = &pMSP->lpMediaType[k];
				(m_lookupMT[*pMSMT->clsMajorType]).insert(make_pair(*pMSMT->clsMinorType, &g_pTemplates[i]));
			}
		}
	}
}

CVGFilterManager::~CVGFilterManager(void)
{
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::EnumMatchingFilters( IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, 
																CLSID clsInMaj, CLSID clsInSub )
{
	CheckPointer(ppEnum, E_POINTER);
	
	CEnumFilter *pEnum = new CEnumFilter;
	// 枚举匹配的内部滤镜
	if (bExactMatch)
	{
		 maj2subs_t::const_iterator itMaj = m_lookupMT.find(clsInMaj);
		 if (itMaj != m_lookupMT.end())
		 {
			guid2ft_itpair_t pFT = itMaj->second.equal_range(clsInSub);
			if (pFT.first != itMaj->second.end() && pFT.second != itMaj->second.end())
			{
				for (guid2ft_t::const_iterator it=pFT.first; it!=pFT.second; it++)
					if (it->second->m_pAMovieSetup_Filter->dwMerit >= dwMerit)
						pEnum->Add(it->second->m_pAMovieSetup_Filter);
			}
		 }
	}
	else	// 非精确匹配时GUID_NULL被看作通配符
	{
		for (maj2subs_t::const_iterator itMaj=m_lookupMT.begin(); itMaj!=m_lookupMT.end(); itMaj++)
		{
			if (!MatchGUID(itMaj->first, clsInMaj))
				continue;

			for (guid2ft_t::const_iterator itSub=itMaj->second.begin(); itSub!=itMaj->second.end(); itSub++)
			{
				if (MatchGUID(itSub->first, clsInSub) && itSub->second->m_pAMovieSetup_Filter->dwMerit >= dwMerit)
					pEnum->Add(itSub->second->m_pAMovieSetup_Filter);
			}
		}
	}
	// 枚举匹配的系统滤镜
	/*if (m_pFM2 != NULL)
	{
		CComPtr<IEnumMoniker> pEnumMon;
		GUID inTypes[] = {clsInMaj, clsInSub};
		CComPtr<IMoniker> pMon;
		CComPtr<IBaseFilter> pBF;
		CLSID clsID;

		if (SUCCEEDED(m_pFM2->EnumMatchingFilters(&pEnumMon, 0, bExactMatch, MERIT_NORMAL, TRUE, countof(inTypes) / 2, inTypes,
			NULL, NULL, FALSE, FALSE, 0, NULL, NULL, NULL)))
		{
			while (pEnumMon->Next(1, &pMon, NULL) == S_OK)
			{
				if (SUCCEEDED(pMon->BindToObject(NULL, NULL, IID_IBaseFilter, (LPVOID*)&pBF)))
				{
					pBF->GetClassID(&clsID);
				}
				pBF.Release();
				pMon.Release();
			}
		}
	}*/

	if (pEnum->GetCount() > 0)
	{
		pEnum->AddRef();
		*ppEnum = pEnum;
		return S_OK;
	}
	else
	{
		delete pEnum;
		*ppEnum = NULL;
		return E_FAIL;
	}
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::Initialize( void )
{
	HRESULT hr;
	
	FAILED_RETURN(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGB));
	FAILED_RETURN(CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, (LPVOID*)&m_pFM2));
	return S_OK;
}