#include "StdAfx.h"
#include "VideoGear.h"
#include "FilterManager.h"
#include "..\..\filters\parser\avisplitter\AviSplitter.h"
#include "..\..\filters\parser\matroskasplitter\MatroskaSplitter.h"
#include "..\..\filters\parser\realmediasplitter\RealMediaSplitter.h"
#include "..\..\filters\transform\mpcvideodec\MPCVideoDecFilter.h"
#include "..\..\filters\transform\mpadecfilter\MpaDecFilter.h"
#include "..\..\filters\switcher\audioswitcher\AudioSwitcher.h"
#include "DbgUtil.h"
#include "DSUtil.h"

template<typename ClassT>
HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv)
{
	HRESULT hr;
	ClassT* pObj = new ClassT(pUnk, &hr);
	if (SUCCEEDED(hr))
	{
		pObj->AddRef();
		*ppv = (IBaseFilter*)pObj;
	}
	else
	{
		delete pObj;
		*ppv = NULL;
	}
	return hr;
}

namespace AVI
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_Avi},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const FilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CAviSplitterFilter), L"MPC - Avi Splitter", CreateInstance<CAviSplitterFilter>, _countof(sudpPins), sudpPins}
	};
}

namespace MKV
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{	
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_Matroska},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL}
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const FilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMatroskaSplitterFilter), L"MPC - Matroska Splitter", CreateInstance<CMatroskaSplitterFilter>, _countof(sudpPins), sudpPins},
	};
}

namespace RMDec
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesIn2[] =
	{
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV20},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV30},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV40},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV41},
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut2[] =
	{
		{&MEDIATYPE_Video, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins2[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn2), sudPinTypesIn2},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesOut2), sudPinTypesOut2}
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesIn3[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_14_4},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_28_8},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_ATRC},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_COOK},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_DNET},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_SIPR},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_AAC},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_RAAC},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_RACP},
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut3[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_PCM},
	};

	const AMOVIESETUP_PIN sudpPins3[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn3), sudPinTypesIn3},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesOut3), sudPinTypesOut3}
	};

	const FilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CRealMediaSplitterFilter), L"MPC - RealMedia Splitter", CreateInstance<CRealMediaSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CRealVideoDecoder), L"MPC - RealVideo Decoder", CreateInstance<CRealVideoDecoder>, _countof(sudpPins2), sudpPins2},
		{&__uuidof(CRealAudioDecoder), L"MPC - RealAudio Decoder", CreateInstance<CRealAudioDecoder>, _countof(sudpPins3), sudpPins3}
	};
}

namespace VideoDec
{
	const AMOVIESETUP_PIN sudpPinsVideoDec[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesInCount,  CMPCVideoDecFilter::sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesOutCount, CMPCVideoDecFilter::sudPinTypesOut}
	};

	const FilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMPCVideoDecFilter), L"MPC - Video decoder", CreateInstance<CMPCVideoDecFilter>, _countof(sudpPinsVideoDec), sudpPinsVideoDec}
	};
}

namespace AudioDec
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_MP3},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_MPEG1AudioPayload},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_MPEG1Payload},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_MPEG1Packet},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_MPEG2_AUDIO},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_MPEG2_AUDIO},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_MPEG2_AUDIO},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_MPEG2_AUDIO},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_DOLBY_AC3},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_DOLBY_AC3},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_DOLBY_AC3},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_DOLBY_AC3},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_DOLBY_DDPLUS},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_DOLBY_DDPLUS},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_DOLBY_DDPLUS},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_DOLBY_DDPLUS},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_DOLBY_TRUEHD},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_DOLBY_TRUEHD},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_DOLBY_TRUEHD},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_DOLBY_TRUEHD},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_WAVE_DOLBY_AC3},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_DTS},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_DTS},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_DTS},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_DTS},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_WAVE_DTS},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_DVD_LPCM_AUDIO},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_DVD_LPCM_AUDIO},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_DVD_LPCM_AUDIO},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_DVD_LPCM_AUDIO},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_HDMV_LPCM_AUDIO},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_AAC},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_AAC},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_AAC},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_AAC},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_MP4A},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_MP4A},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_MP4A},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_MP4A},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_mp4a},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_mp4a},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_mp4a},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_mp4a},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_PS2_PCM},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_PS2_PCM},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_PS2_PCM},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_PS2_PCM},
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_PS2_ADPCM},
		{&MEDIATYPE_MPEG2_PACK,			&MEDIASUBTYPE_PS2_ADPCM},
		{&MEDIATYPE_MPEG2_PES,			&MEDIASUBTYPE_PS2_ADPCM},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_PS2_ADPCM},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_Vorbis2},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_FLAC_FRAMED},
		{&MEDIATYPE_Audio,				&MEDIASUBTYPE_NELLYMOSER},
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_PCM},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesOut), sudPinTypesOut}
	};

	const FilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMpaDecFilter), L"MPC - MPA Decoder Filter", CreateInstance<CMpaDecFilter>, _countof(sudpPins), sudpPins},
	};
}

namespace AudioSw
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_NULL}
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_NULL}
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesOut), sudPinTypesOut}
	};

	const FilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CAudioSwitcherFilter), L"MPC - AudioSwitcher", CreateInstance<CAudioSwitcherFilter>, _countof(sudpPins), sudpPins}
	};
}

//////////////////////////////////////////////////////////////////////////

CFilterManager g_FilterMgr;

CFilterManager::CFilterManager(void)
{
	RegisterFilter(_countof(AVI::sudFilters), AVI::sudFilters);
	RegisterFilter(_countof(MKV::sudFilters), MKV::sudFilters);
	RegisterFilter(_countof(RMDec::sudFilters), RMDec::sudFilters);
	RegisterFilter(_countof(VideoDec::sudFilters), VideoDec::sudFilters);
	RegisterFilter(_countof(AudioDec::sudFilters), AudioDec::sudFilters);
	RegisterFilter(_countof(AudioSw::sudFilters), AudioSw::sudFilters, true);
}

CFilterManager::~CFilterManager(void)
{
	m_filters.RemoveAll();
}

HRESULT CFilterManager::EnumMatchingFilters( const CAtlList<CMediaType>& mts, CAtlList<CFilter*>& filters )
{
	UINT nInitCount = filters.GetCount();

	POSITION posMT = mts.GetHeadPosition();
	while (posMT != NULL)
	{
		const CMediaType &mt = mts.GetNext(posMT);
		MajorTypes::CPair *pair = m_majorTypes.Lookup(mt.majortype);
		if (pair == NULL)
			continue;
		/* 精确匹配 */
		POSITION pos = pair->m_value.FindFirstWithKey(mt.subtype);
		while (pos != NULL)
		{
			CFilter *pFilter = pair->m_value.GetNextValueWithKey(pos, mt.subtype);
			filters.AddTail(pFilter);
		}
		/* 模糊匹配 */
		if (mt.subtype != GUID_NULL)
		{
			POSITION pos = pair->m_value.FindFirstWithKey(GUID_NULL);
			while (pos != NULL)
			{
				CFilter *pFilter = pair->m_value.GetNextValueWithKey(pos, mt.subtype);
				filters.AddTail(pFilter);
			}
		}
	}

	return filters.GetCount() - nInitCount > 0 ? S_OK : S_FALSE;
}

HRESULT CFilterManager::AddAudioSwitcherToGraph( IFilterGraph *pGraph, IBaseFilter **ppvObj )
{
	if (pGraph == NULL || ppvObj == NULL)
		return E_POINTER;
	FilterList::CPair *pair = m_filters.Lookup(__uuidof(CAudioSwitcherFilter));
	if (pair == NULL)
		return E_FAIL;
	RIF(pair->m_value->CreateInstance(NULL, ppvObj));
	return pGraph->AddFilter(*ppvObj, pair->m_value->GetName());
}

HRESULT CFilterManager::RegisterFilter( UINT nFilterCount, const FilterSetupInfo* pSetupInfo, bool bFilterOnly )
{
	for (UINT i = 0; i < nFilterCount; i++)
	{
		const FilterSetupInfo& setupInfo = pSetupInfo[i];
		CFilter* pFilter = new CFilterInternal(*setupInfo.pClsID, setupInfo.pszName, setupInfo.pfnCreateInstance, 
			setupInfo.nPinCount, setupInfo.pPins);
		m_filters[*setupInfo.pClsID] = CAutoPtr<CFilter>(pFilter);
		if (!bFilterOnly)
		{
			/* 注册输入类型 */
			for (UINT j = 0; j < setupInfo.nPinCount; j++)
			{
				const AMOVIESETUP_PIN &pinInfo = setupInfo.pPins[j];
				if (pinInfo.bOutput)
					continue;
				for (UINT k = 0; k < pinInfo.nMediaTypes; k++)
				{
					const REGPINTYPES &pinTypes = pinInfo.lpMediaType[k];
					m_majorTypes[*pinTypes.clsMajorType].Insert(*pinTypes.clsMinorType, pFilter);
				}
			}
		}		
	}

	return S_OK;
}

HRESULT CFilterManager::RegisterFilter( CLSID clsID )
{
	CFilter* pFilter = new CFilterRegister(clsID);
	m_filters[clsID] = CAutoPtr<CFilter>(pFilter);
	return S_OK;
}

