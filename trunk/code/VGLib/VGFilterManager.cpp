#include "StdAfx.h"
#include "VGFilterManager.h"

CEnumGUID::CEnumGUID( void )
	:m_nIter(0)
{
}

void CEnumGUID::Add( const GUID &guid )
{
	m_items.push_back(guid);
}

HRESULT STDMETHODCALLTYPE CEnumGUID::Next( ULONG celt, GUID *rgelt, ULONG *pceltFetched )
{
	CheckPointer(rgelt, E_POINTER);
	ValidateReadWritePtr(rgelt, celt * sizeof(GUID));

	ULONG nToDo = m_items.size() - m_nIter;
	if (nToDo == 0)
		return E_FAIL;
	if (nToDo > m_nIter)
		nToDo = m_nIter;
	for (ULONG i=0; i<nToDo; i++)
		*rgelt = m_items[m_nIter++];
	return nToDo == celt ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CEnumGUID::Skip( ULONG celt )
{
	ULONG nToDo = m_items.size() - m_nIter;
	if (nToDo == 0)
		return E_FAIL;
	if (nToDo > m_nIter)
		nToDo = m_nIter;
	m_nIter += nToDo;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEnumGUID::Reset( void )
{
	m_nIter = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEnumGUID::Clone( IEnumGUID **ppenum )
{
	CEnumGUID *pEnum = new CEnumGUID;
	pEnum->m_items = m_items;
	pEnum->m_nIter = m_nIter;
	pEnum->AddRef();
	(*ppenum) = pEnum;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

extern CFactoryTemplate *g_Templates;
extern int g_cTemplates;

CVGFilterManager::CVGFilterManager(void)
{
	const AMOVIESETUP_FILTER *pMSF = NULL;
	const AMOVIESETUP_PIN *pMSP= NULL;
	
	for (int i=0; i<g_cTemplates; i++)
	{
		m_lookupFlt[*g_Templates[i].m_ClsID] = &g_Templates[i];
		pMSF = g_Templates[i].m_pAMovieSetup_Filter;
		for (UINT j=0; j<pMSF->nPins; j++)
		{
			pMSP = &pMSF->lpPin[j];
			if (pMSP->bOutput)
				continue;
			for (UINT k=0; k<pMSP->nMediaTypes; k++)
				m_lookupMT.insert(make_pair(pMSP->lpMediaType[k], &g_Templates[i]));
		}
	}
}

CVGFilterManager::~CVGFilterManager(void)
{
}

HRESULT CVGFilterManager::EnumMatchingFilters( IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, CLSID clsInMaj, CLSID clsInSub )
{
	CEnumGUID *pEnum = new CEnumGUID;
	pair<mt_lookup_iter_t, mt_lookup_iter_t> p;
	CVGMediaType mt(clsInMaj, clsInSub, false);

	// 枚举完全匹配的
	p = m_lookupMT.equal_range(mt);
	if (p.first != m_lookupMT.end() && p.second != m_lookupMT.end())
	{
		for (mt_lookup_iter_t it=p.first; it!=p.second; it++)
			pEnum->Add(*it->second->m_ClsID);
	}

	return S_OK;
}