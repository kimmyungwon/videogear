#include "StdAfx.h"
#include "VGFilters.h"

//////////////////////////////////////////////////////////////////////////

#include "../mpc-hc/src/filters/transform/mpcvideodec/MPCVideoDecFilter.h"
#include "../mpc-hc/src/filters/transform/mpcvideodec/MPCAudioDecFilter.h"

namespace MPCVideoDec
{
	const AMOVIESETUP_PIN sudpPinsVideoDec[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesInCount,  CMPCVideoDecFilter::sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesOutCount, CMPCVideoDecFilter::sudPinTypesOut}
	};


	const AMOVIESETUP_PIN sudpPinsAudioDec[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCAudioDecFilter::sudPinTypesInCount,  CMPCAudioDecFilter::sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCAudioDecFilter::sudPinTypesOutCount, CMPCAudioDecFilter::sudPinTypesOut}
	};


	const AMOVIESETUP_FILTER sudFilters[] =
	{
		{&__uuidof(CMPCVideoDecFilter), L"MPC - Video decoder", /*MERIT_DO_NOT_USE*/0x40000001, countof(sudpPinsVideoDec), sudpPinsVideoDec}
		//	{&__uuidof(CMPCAudioDecFilter), L"MPC - Audio decoder", /*MERIT_DO_NOT_USE*/0x40000001, countof(sudpPinsAudioDec), sudpPinsAudioDec},
	};


	CFactoryTemplate g_Templates[] =
	{
		{sudFilters[0].strName, &__uuidof(CMPCVideoDecFilter), CreateInstance<CMPCVideoDecFilter>, NULL, &sudFilters[0]},
		{L"CMPCVideoDecPropertyPage", &__uuidof(CMPCVideoDecSettingsWnd), CreateInstance<CInternalPropertyPageTempl<CMPCVideoDecSettingsWnd> >},
		{L"CMPCVideoDecPropertyPage2", &__uuidof(CMPCVideoDecCodecWnd), CreateInstance<CInternalPropertyPageTempl<CMPCVideoDecCodecWnd> >},

		//    {sudFilters[1].strName, &__uuidof(CMPCAudioDecFilter), CreateInstance<CMPCAudioDecFilter>, NULL, &sudFilters[1]},
		//	{L"CMPCAudioDecPropertyPage", &__uuidof(CMPCAudioDecSettingsWnd), CreateInstance<CInternalPropertyPageTempl<CMPCAudioDecSettingsWnd> >},
	};

	int g_cTemplates = countof(g_Templates);
}

//////////////////////////////////////////////////////////////////////////

VGFilters g_internalFilters;	// 全局唯一对象

VGFilters::VGFilters(void)
{		
	RegisterTransformFilters(MPCVideoDec::g_Templates,MPCVideoDec:: g_cTemplates);
}

VGFilters::~VGFilters(void)
{
}

HRESULT VGFilters::FindMatchingFilters( const AM_MEDIA_TYPE* _InType, VGMatchingFilters& _Results )
{
	HRESULT hr;
	VGMediaType mt;
	pair<pinTypes_t::const_iterator, pinTypes_t::const_iterator> pii;
	UINT nTotal = 0;
	CUnknown* pUnk = NULL; 
	CComPtr<IBaseFilter> pFilter;
	
	if (_InType == NULL)
		return E_POINTER;
	mt = _InType;
	pii = m_pinTypes.equal_range(mt);
	for (pinTypes_t::const_iterator it = pii.first; it != pii.second; ++it)
	{
		ASSERT(it->second != NULL && it->second->m_lpfnNew != NULL);
		pUnk = it->second->m_lpfnNew(NULL, &hr);
		if (hr != S_OK || pUnk == NULL)
			continue;
		if (pUnk->NonDelegatingQueryInterface(IID_IBaseFilter, (void**)&pFilter) != S_OK)
		{
			delete pUnk;
			continue;
		}
		ASSERT(it->second->m_pAMovieSetup_Filter != NULL);
		_Results.push_back(VGFilter(it->second->m_pAMovieSetup_Filter->strName, pFilter));
		nTotal++;
	}
	return nTotal > 0 ? S_OK : E_FAIL;
}

void VGFilters::RegisterTransformFilters( CFactoryTemplate* _Templates, int _Count )
{
	CFactoryTemplate* pTempl = NULL;
	const AMOVIESETUP_FILTER* pFilter = NULL; 
	const AMOVIESETUP_PIN* pPin = NULL;
	
	for (int iFlt = 0; iFlt < _Count; ++iFlt)
	{
		pTempl = &_Templates[iFlt];
		if (pTempl->m_pAMovieSetup_Filter == NULL)
			continue;
		pFilter = pTempl->m_pAMovieSetup_Filter;
		for (UINT iPin = 0; iPin < pFilter->nPins; ++iPin)
		{
			pPin = &pFilter->lpPin[iPin];
			if (!pPin->bOutput)
				continue;
			for (UINT iMT = 0; iMT < pPin->nMediaTypes; ++iMT)
			{
				m_pinTypes.insert(make_pair(VGMediaType(*pPin->lpMediaType[iMT].clsMajorType, 
					*pPin->lpMediaType[iMT].clsMinorType), pTempl));
			}
		}
	}	
}

