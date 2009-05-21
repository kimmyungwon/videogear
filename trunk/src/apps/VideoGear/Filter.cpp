#include "StdAfx.h"
#include "VideoGear.h"
#include "Filter.h"
#include "DSUtil.h"

CFilter::CFilter( const CLSID& clsID, LPCWSTR pszName )
: m_clsID(clsID), m_strName(pszName)
{
}

CFilter::~CFilter(void)
{
}

//////////////////////////////////////////////////////////////////////////

CFilterInternal::CFilterInternal( const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, 
								 UINT uiPinCount, const AMOVIESETUP_PIN* pPins )
: CFilter(clsID, pszName), m_pfnCreateInstance(pfnCreateInstance)
{
}

HRESULT CFilterInternal::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	return m_pfnCreateInstance(pUnk, ppv);
}

//////////////////////////////////////////////////////////////////////////

CFilterRegister::CFilterRegister( const CLSID& clsID, LPCWSTR pszName )
: CFilter(clsID, pszName)
{
}

HRESULT CFilterRegister::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	return CoCreateInstance(m_clsID, pUnk, CLSCTX_ALL, IID_IBaseFilter, (LPVOID*)ppv);
}

//////////////////////////////////////////////////////////////////////////

CFilterDMO::CFilterDMO( const CLSID &clsID, LPCWSTR pszName, const CLSID &clsidCategory )
: CFilter(clsID, pszName), m_clsidCategory(clsidCategory)
{

}

HRESULT CFilterDMO::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	CComPtr<IDMOWrapperFilter> pWrapper;
	
	RIF(CoCreateInstance(CLSID_DMOWrapperFilter, pUnk, CLSCTX_ALL, IID_IBaseFilter, (LPVOID*)ppv));
	if (SUCCEEDED((*ppv)->QueryInterface(IID_IDMOWrapperFilter, (LPVOID*)&pWrapper)))
	{
		if (SUCCEEDED(pWrapper->Init(m_clsID, m_clsidCategory)))
			return S_OK;
	}
	(*ppv)->Release();
	*ppv = NULL;
	return E_FAIL;
}