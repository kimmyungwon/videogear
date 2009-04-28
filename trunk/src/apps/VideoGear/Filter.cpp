#include "StdAfx.h"
#include "Filter.h"

CFilter::CFilter( LPCTSTR name, const CLSID& clsID )
: m_name(name), m_clsID(clsID)
{

}

//////////////////////////////////////////////////////////////////////////

CInternalFilter::CInternalFilter( LPCTSTR name, const CLSID& clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit /*= NULL*/ )
: CFilter(name, clsID), m_lpfnNew(lpfnNew), m_lpfnInit(lpfnInit)
{
}

HRESULT CInternalFilter::CreateObject( IBaseFilter** ppvObj )
{
	HRESULT hr;
	CBaseFilter* bf;
	
	if (m_lpfnNew == NULL)
		return E_UNEXPECTED;
	if (ppvObj == NULL)
		return E_POINTER;
	bf = static_cast<CBaseFilter*>(m_lpfnNew(NULL, &hr));
	if (SUCCEEDED(hr) && bf != NULL)
	{
		bf->AddRef();
		*ppvObj = (IBaseFilter*)bf;
	}
	return hr;
}