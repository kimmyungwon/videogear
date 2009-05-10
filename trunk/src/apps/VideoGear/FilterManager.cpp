#include "StdAfx.h"
#include "FilterManager.h"
#include "..\..\filters\parser\avisplitter\AviSplitter.h"
#include "..\..\filters\parser\matroskasplitter\MatroskaSplitter.h"
#include "..\..\filters\transform\mpcvideodec\MPCVideoDecFilter.h"
#include "..\..\filters\transform\mpadecfilter\MpaDecFilter.h"

bool operator<(const CLSID& left, const CLSID& right)
{
	return memcmp(&left, &right, sizeof(CLSID)) < 0;
}

//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////

CFilterManager::CFilterManager(void)
{
	RegisterFilter(_countof(AVI::sudFilters), AVI::sudFilters);
	RegisterFilter(_countof(MKV::sudFilters), MKV::sudFilters);
	RegisterFilter(_countof(VideoDec::sudFilters), VideoDec::sudFilters);
	RegisterFilter(_countof(AudioDec::sudFilters), AudioDec::sudFilters);
}

CFilterManager::~CFilterManager(void)
{
	while (m_filters.size() > 0)
	{
		CFilter* pFilter = m_filters.begin()->second;
		m_filters.erase(m_filters.begin());
		delete pFilter;
	}
}

HRESULT CFilterManager::EnumMatchingFilters( const CAtlList<CMediaType>& mts, std::list<CFilter*>& filters )
{
	UINT nInitCount = filters.size();
	for (FilterList::const_iterator it = m_filters.begin(); it != m_filters.end(); it++)
	{
		CFilter* pFilter = it->second;
		POSITION pos = mts.GetHeadPosition(); 
		while (pos != NULL)
		{
			const CMediaType& mt = mts.GetNext(pos);
			if (pFilter->CheckInputType(mt) == S_OK)
				filters.push_back(pFilter);
			break;
		}
	}
	return filters.size() - nInitCount > 0 ? S_OK : S_FALSE;
}

HRESULT CFilterManager::RegisterFilter( UINT uiFilterCount, const FilterSetupInfo* pSetupInfo )
{
	for (UINT i = 0; i < uiFilterCount; i++)
	{
		const FilterSetupInfo& setupInfo = pSetupInfo[i];
		CFilter* pFilter = new CInternalFilter(*setupInfo.pClsID, setupInfo.pszName, setupInfo.pfnCreateInstance, 
			setupInfo.uiPinCount, setupInfo.pPins);
		m_filters.insert(std::make_pair(*pSetupInfo->pClsID, pFilter));
	}
	return S_OK;
}