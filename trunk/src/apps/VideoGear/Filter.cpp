#include "StdAfx.h"
#include "Filter.h"

CFilter::CFilter( const CLSID& clsID, LPCWSTR pszName )
: m_clsID(clsID), m_strName(pszName)
{
}

CFilter::~CFilter(void)
{
}

//////////////////////////////////////////////////////////////////////////

CInternalFilter::CInternalFilter( const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, 
								 UINT uiPinCount, const AMOVIESETUP_PIN* pPins )
: CFilter(clsID, pszName), m_pfnCreateInstance(pfnCreateInstance)
{
	for (UINT i = 0; i < uiPinCount; i++)
	{
		const AMOVIESETUP_PIN& pin = pPins[i];
		if (pin.bOutput)
			continue;
		for (UINT j = 0; j < pin.nMediaTypes; j++)
		{
			m_majorTypes[*pin.lpMediaType[j].clsMajorType][*pin.lpMediaType[j].clsMinorType] = 0;
		}
	}
}

HRESULT CInternalFilter::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	return m_pfnCreateInstance(pUnk, ppv);
}

HRESULT CInternalFilter::CheckInputType( const CMediaType& mt )
{
	if (mt.majortype == GUID_NULL)
		return E_INVALIDARG;

	MajorTypes::CPair *pairKey = m_majorTypes.Lookup(mt.majortype);
	if (pairKey != NULL)
	{
		MinorTypes::CPair *pair = pairKey->m_value.Lookup(mt.subtype);
		if (pair != NULL)
			return S_OK;
		if (mt.subtype != GUID_NULL)
		{		
			pair = pairKey->m_value.Lookup(GUID_NULL);
			return (pair != NULL) ? S_OK : S_FALSE;
		}
	}
	return S_FALSE;	
}