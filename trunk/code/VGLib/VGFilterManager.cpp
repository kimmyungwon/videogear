#include "StdAfx.h"
#include "VGFilterManager.h"

VG_IMPLEMENT_ADDREF(CVGFilterManager)
VG_IMPLEMENT_RELEASE(CVGFilterManager)
VG_IMPLEMENT_QI(CVGFilterManager)

VG_IMPLEMENT_CONSTRUCTOR_BEGIN(CVGFilterManager)	
VG_IMPLEMENT_CONSTRUCTOR_END(CVGFilterManager)

VG_IMPLEMENT_DESTRUCTOR_BEGIN(CVGFilterManager)
VG_IMPLEMENT_DESTRUCTOR_END(CVGFilterManager)

HRESULT STDMETHODCALLTYPE CVGFilterManager::Initialize( void )
{
	HRESULT hr;
	
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGB);
	if (FAILED(hr))	
		return hr;

	return S_OK;
}