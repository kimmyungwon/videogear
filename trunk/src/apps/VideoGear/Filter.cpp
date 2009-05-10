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

CInternalFilter::CInternalFilter( const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, UINT uiPinCount, const AMOVIESETUP_PIN* pPins )
: CFilter(clsID, pszName), m_pfnCreateInstance(pfnCreateInstance)
{
	for (UINT i = 0; i < uiPinCount; i++)
	{
		const AMOVIESETUP_PIN& pin = pPins[i];
		if (pin.bOutput)
			continue;
		for (UINT j = 0; j < pin.nMediaTypes; j++)
		{
			m_mtsIn.push_back(MediaType(*pin.lpMediaType[j].clsMajorType, *pin.lpMediaType[j].clsMinorType));
		}
	}
}

HRESULT CInternalFilter::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	return m_pfnCreateInstance(pUnk, ppv);
}

HRESULT CInternalFilter::CheckInputType( const CMediaType& mt )
{
	if (CheckInputType(MediaType(mt.majortype, mt.subtype)))
		return S_OK;
	if (mt.subtype != GUID_NULL && CheckInputType(MediaType(mt.majortype, GUID_NULL)))
		return S_OK;
	if (mt.majortype != GUID_NULL && CheckInputType(MediaType(GUID_NULL, mt.subtype)))
		return S_OK;
	return S_FALSE;
}

bool CInternalFilter::CheckInputType( const MediaType& mt )
{
	for (std::list<MediaType>::const_iterator it = m_mtsIn.begin(); it != m_mtsIn.end(); it++)
	{
		if ((it->major == mt.major || it->major == GUID_NULL || mt.major == GUID_NULL)
			&& (it->minor == mt.minor || it->minor == GUID_NULL || mt.minor == GUID_NULL))
			return true;
	}
	return false;
}