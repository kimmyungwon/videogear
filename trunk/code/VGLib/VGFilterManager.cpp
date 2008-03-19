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

size_t CEnumGUID::GetCount( void )
{
	return m_items.size();
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

extern CFactoryTemplate *g_pTemplates;
extern int g_cTemplates;

CVGFilterManager::CVGFilterManager(void)
{
	const AMOVIESETUP_FILTER *pMSF = NULL;
	const AMOVIESETUP_PIN *pMSP= NULL;
	
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
				//m_lookupMT.insert(make_pair(pMSP->lpMediaType[k], &g_pTemplates[i]));
				m_lookupMT[*pMSP->lpMediaType[k].clsMajorType][*pMSP->lpMediaType[k].clsMinorType] = &g_pTemplates[i];
		}
	}
}

CVGFilterManager::~CVGFilterManager(void)
{
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::EnumMatchingFilters( IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, CLSID clsInMaj, CLSID clsInSub )
{
	CheckPointer(ppEnum, E_POINTER);
	
	CEnumGUID *pEnum = new CEnumGUID;
	
	if (bExactMatch)
	{

	}
	else
	{
		for (maj2subs_t::const_iterator itMaj=m_lookupMT.begin(); itMaj!=m_lookupMT.end(); itMaj++)
		{
			if (!MatchGUID(itMaj->first, clsInMaj))
				continue;

			for (guid2ft_t::const_iterator itSub=itMaj->second.begin(); itSub!=itMaj->second.end(); itSub++)
			{
				if (MatchGUID(itSub->first, clsInSub))
					pEnum->Add(*itSub->second->m_ClsID);
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
		*ppEnum = NULL;
		return E_FAIL;
	}
}