#include "StdAfx.h"
#include "VGFilterManager.h"

CEnumFilter::CEnumFilter( void )
{
	m_iter = m_items.end();
}

void CEnumFilter::Add( const CVGFilter& flt )
{
	m_items.insert(flt);
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
		rgelt[nDone] = m_iter->clsID;
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
		RegisterFilter(g_pTemplates[i]);
	// 枚举系统滤镜
	LPOLESTR lpszFilterCategory;
	LONG lRet;
	CRegKey regRoot, regItem;
	DWORD nIdx = 0, nKeyNameLen, nFilterNameLen, nFilterDataSize;
	CLSID clsID;
	WCHAR szKeyName[MAX_PATH], szFilterName[MAX_FILTER_NAME];
	REGFILTER2 *pFilterData;

	StringFromCLSID(CLSID_LegacyAmFilterCategory, &lpszFilterCategory);
	if (regRoot.Open(HKEY_CLASSES_ROOT, L"CLSID\\" + CStringW(lpszFilterCategory) + L"\\Instance") == ERROR_SUCCESS)
	{
		while (regRoot.EnumKey(nIdx++, szKeyName, &(nKeyNameLen = MAX_PATH), NULL) == S_OK)
		{
			if (regItem.Open(regRoot.m_hKey, szKeyName) != ERROR_SUCCESS)
				continue;
			if (regItem.QueryGUIDValue(L"CLSID", clsID) != ERROR_SUCCESS)
				continue;
			if (regItem.QueryStringValue(L"FriendlyName", szFilterName, &(nFilterNameLen = MAX_FILTER_NAME)) != ERROR_SUCCESS)
				continue;
			nFilterDataSize = 10240;
			pFilterData = (REGFILTER2*)malloc(nFilterDataSize);
			while((lRet = regItem.QueryBinaryValue(L"FilterData", pFilterData, &nFilterDataSize)) != ERROR_SUCCESS)
			{
				if (lRet != ERROR_MORE_DATA)
					break;
				pFilterData = (REGFILTER2*)realloc(pFilterData, nFilterDataSize * 2);
			}
			if (lRet == ERROR_SUCCESS)
				switch (pFilterData->dwVersion)
				{
				case 1:
					RegisterFilter(clsID, szFilterName, pFilterData->dwMerit, pFilterData->cPins, pFilterData->rgPins);
					break;
				case 2:
					RegisterFilter(clsID, szFilterName, pFilterData->dwMerit, pFilterData->cPins2, pFilterData->rgPins2);
				}			
			free(pFilterData);
		}
	}
	CoTaskMemFree(lpszFilterCategory);
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
					if (it->second.dwMerit >= dwMerit)
						pEnum->Add(it->second);
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
				if (MatchGUID(itSub->first, clsInSub) && itSub->second.dwMerit >= dwMerit)
					pEnum->Add(itSub->second);
			}
		}
	}

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

HRESULT CVGFilterManager::RegisterFilter( const CFactoryTemplate& templ )
{	
	return RegisterFilter(*templ.m_ClsID, templ.m_Name, templ.m_pAMovieSetup_Filter->dwMerit, 
		templ.m_pAMovieSetup_Filter->nPins, templ.m_pAMovieSetup_Filter->lpPin);
}

HRESULT CVGFilterManager::RegisterFilter( REFCLSID clsID, LPCWSTR lpszName, DWORD dwMerit, ULONG nPins, 
										 const REGFILTERPINS* lpPins, LPFNNewCOMObject lpfnNew )
{
	CheckPointer(lpPins, E_POINTER);
	if (m_lookupFlt.find(clsID) != m_lookupFlt.end())
		return ERROR_FILE_EXISTS;

	CVGFilter flt(clsID, lpszName, dwMerit, lpfnNew);
	const AMOVIESETUP_MEDIATYPE *pMSMT = NULL;

	m_lookupFlt[clsID] = flt;
	for (UINT i=0; i<nPins; i++)
	{
		if (lpPins[i].bOutput)
			continue;
		for (UINT j=0; j<lpPins[i].nMediaTypes; j++)
		{
			pMSMT = &lpPins[i].lpMediaType[j];
			(m_lookupMT[*pMSMT->clsMajorType]).insert(make_pair(*pMSMT->clsMinorType, flt));
		}
	}	
	return S_OK;	
}

HRESULT CVGFilterManager::RegisterFilter( REFCLSID clsID, LPCWSTR lpszName, DWORD dwMerit, ULONG nPins2, 
										 const REGFILTERPINS2* lpPins2, LPFNNewCOMObject lpfnNew )
{
	CheckPointer(lpPins2, E_POINTER);
	if (m_lookupFlt.find(clsID) != m_lookupFlt.end())
		return ERROR_FILE_EXISTS;

	CVGFilter flt(clsID, lpszName, dwMerit, lpfnNew);
	const AMOVIESETUP_MEDIATYPE *pMSMT = NULL;

	m_lookupFlt[clsID] = flt;
	for (UINT i=0; i<nPins2; i++)
	{
		if (lpPins2[i].dwFlags & REG_PINFLAG_B_OUTPUT)
			continue;
		for (UINT j=0; j<lpPins2[i].nMediaTypes; j++)
		{
			pMSMT = &lpPins2[i].lpMediaType[j];
			(m_lookupMT[*pMSMT->clsMajorType]).insert(make_pair(*pMSMT->clsMinorType, flt));
		}
	}	
	return S_OK;		
}