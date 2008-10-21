#include "StdAfx.h"
#include "VGFilters.h"

//////////////////////////////////////////////////////////////////////////

#include "../mpc-hc/src/filters/transform/mpcvideodec/MPCVideoDecFilter.h"
#include "../mpc-hc/src/filters/transform/mpcvideodec/MPCAudioDecFilter.h"
#include "../mpc-hc/src/dsutil/DSUtil.h"

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

VGFilters g_filters;	// 全局唯一对象

VGFilters::VGFilters(void)
{		
	RegisterFilters(MPCVideoDec::g_Templates,MPCVideoDec:: g_cTemplates);
}

VGFilters::~VGFilters(void)
{
}

void VGFilters::RegisterFilters( CFactoryTemplate* _Templates, int _Count )
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
			for (UINT iMT = 0; iMT < pPin->nMediaTypes; ++iMT)
			{
				m_pinTypes.insert(make_pair(VGMediaType(*pPin->lpMediaType[iMT].clsMajorType, 
					*pPin->lpMediaType[iMT].clsMinorType), pTempl));
			}
		}
	}	
}