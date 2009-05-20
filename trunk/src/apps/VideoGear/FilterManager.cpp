#include "StdAfx.h"
#include "VideoGear.h"
#include "FilterManager.h"
#include "DSUtil.h"
#include "..\..\filters\parser\avisplitter\AviSplitter.h"
#include "..\..\filters\parser\matroskasplitter\MatroskaSplitter.h"
#include "..\..\filters\parser\realmediasplitter\RealMediaSplitter.h"
#include "..\..\filters\parser\flvsplitter\FLVSplitter.h"
#include "..\..\filters\parser\mp4splitter\MP4Splitter.h"
#include "..\..\filters\parser\mpegsplitter\MpegSplitter.h"
#include "..\..\filters\transform\mpcvideodec\MPCVideoDecFilter.h"
#include "..\..\filters\transform\mpeg2decfilter\Mpeg2DecFilter.h"
#include "..\..\filters\transform\mpadecfilter\MpaDecFilter.h"
#include "..\..\filters\switcher\audioswitcher\AudioSwitcher.h"
#include "DbgUtil.h"

namespace std
{
	bool operator<(const GUID &_left, const GUID &_right)
	{
		return (memcmp(&_left, &_right, sizeof(GUID)) < 0);
	}
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

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CAviSplitterFilter), L"MPC - Avi Splitter", CreateInstance<CAviSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CAviSourceFilter), L"MPC - Avi Source", CreateInstance<CAviSourceFilter>, 0, NULL},
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

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMatroskaSplitterFilter), L"MPC - Matroska Splitter", CreateInstance<CMatroskaSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CMatroskaSourceFilter), L"MPC - Matroska Source", CreateInstance<CMatroskaSourceFilter>, 0, NULL}
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

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CRealMediaSplitterFilter), L"MPC - RealMedia Splitter", CreateInstance<CRealMediaSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CRealMediaSourceFilter), L"MPC - RealMedia Source", CreateInstance<CRealMediaSourceFilter>, 0, NULL},
		{&__uuidof(CRealVideoDecoder), L"MPC - RealVideo Decoder", CreateInstance<CRealVideoDecoder>, _countof(sudpPins2), sudpPins2},
		{&__uuidof(CRealAudioDecoder), L"MPC - RealAudio Decoder", CreateInstance<CRealAudioDecoder>, _countof(sudpPins3), sudpPins3}
	};
}

namespace FLV
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_FLV},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut2[] =
	{
		{&MEDIATYPE_Video, &MEDIASUBTYPE_NULL},
	};

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CFLVSplitterFilter), L"MPC - FLV Splitter", CreateInstance<CFLVSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CFLVSourceFilter), L"MPC - FLV Source", CreateInstance<CFLVSourceFilter>, 0, NULL},
	};
}

namespace MP4
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_MP4},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMP4SplitterFilter), L"MPC - MP4 Splitter", CreateInstance<CMP4SplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CMP4SourceFilter), L"MPC - MP4 Source", CreateInstance<CMP4SourceFilter>, 0, NULL},
		{&__uuidof(CMPEG4VideoSplitterFilter), L"MPC - MPEG4 Video Splitter", CreateInstance<CMPEG4VideoSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CMPEG4VideoSourceFilter), L"MPC - MPEG4 Video Source", CreateInstance<CMPEG4VideoSourceFilter>, 0, NULL},	
	};
}

namespace MPEG
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_MPEG1System},
		//	{&MEDIATYPE_Stream, &MEDIASUBTYPE_MPEG1VideoCD}, // cdxa filter should take care of this
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_MPEG2_PROGRAM},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_MPEG2_TRANSPORT},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_MPEG2_PVA},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL},
	};

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMpegSplitterFilter), L"MPC - Mpeg Splitter", CreateInstance<CMpegSplitterFilter>, _countof(sudpPins), sudpPins},
		{&__uuidof(CMpegSourceFilter), L"MPC - Mpeg Source", CreateInstance<CMpegSourceFilter>, 0, NULL},
	};
}

namespace VideoDec
{
	const AMOVIESETUP_PIN sudpPinsVideoDec[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesInCount,  CMPCVideoDecFilter::sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesOutCount, CMPCVideoDecFilter::sudPinTypesOut}
	};

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMPCVideoDecFilter), L"MPC - Video decoder", CreateInstance<CMPCVideoDecFilter>, _countof(sudpPinsVideoDec), sudpPinsVideoDec}
	};
}

namespace MpegDec
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_DVD_ENCRYPTED_PACK, &MEDIASUBTYPE_MPEG2_VIDEO},
		{&MEDIATYPE_MPEG2_PACK, &MEDIASUBTYPE_MPEG2_VIDEO},
		{&MEDIATYPE_MPEG2_PES, &MEDIASUBTYPE_MPEG2_VIDEO},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_MPEG2_VIDEO},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_MPEG1Packet},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_MPEG1Payload},
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut[] =
	{
		{&MEDIATYPE_Video, &MEDIASUBTYPE_IYUV},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesOut), sudPinTypesOut}
	};

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CMpeg2DecFilter), L"MPC - MPEG Video Decoder", CreateInstance<CMpeg2DecFilter>, _countof(sudpPins), sudpPins},
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

	const InternalFilterSetupInfo sudFilters[] =
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

	const InternalFilterSetupInfo sudFilters[] =
	{
		{&__uuidof(CAudioSwitcherFilter), L"MPC - AudioSwitcher", CreateInstance<CAudioSwitcherFilter>, _countof(sudpPins), sudpPins}
	};
}

//////////////////////////////////////////////////////////////////////////

CFilterManager g_FilterMgr;

CFilterManager::CFilterManager(void)
{
}

CFilterManager::~CFilterManager(void)
{
}

void CFilterManager::Initialize( void )
{
	/* �ڲ��˾� */
	RegisterInternalFilter(_countof(AVI::sudFilters), AVI::sudFilters);
	RegisterInternalSource(__uuidof(CAviSourceFilter), 
		L"0,4,,52494646,8,4,,41564920",	// 'RIFF' ... 'AVI '
		L"0,4,,52494646,8,4,,41564958",	// 'RIFF' ... 'AVIX'
		L"0,4,,52494646,8,4,,414D5620", // 'RIFF' ... 'AMV '
		NULL, L".avi", L".divx", L".vp6", L".amv", NULL);
	RegisterInternalFilter(_countof(MKV::sudFilters), MKV::sudFilters);
	RegisterInternalSource(__uuidof(CMatroskaSourceFilter), L"0,4,,1A45DFA3", NULL, L".mkv", L".mka", L".mks", NULL);
	RegisterInternalFilter(_countof(RMDec::sudFilters), RMDec::sudFilters);
	RegisterInternalSource(__uuidof(CRealMediaSourceFilter), L"0,4,,2E524D46", NULL, L".rm", L".rmvb", L".ram", NULL);
	RegisterInternalFilter(_countof(FLV::sudFilters), FLV::sudFilters);
	RegisterInternalSource(__uuidof(CFLVSourceFilter), L"0,4,,464C5601", NULL, NULL);
	RegisterInternalFilter(_countof(MP4::sudFilters), MP4::sudFilters);
	RegisterInternalSource(__uuidof(CMP4SourceFilter),
		L"4,4,,66747970",	// ftyp
		L"4,4,,6d6f6f76",	// moov
		L"4,4,,6d646174",	// mdat
		L"4,4,,736b6970",	// skip
		L"4,12,ffffffff00000000ffffffff,77696465027fe3706d646174",	// wide ? mdat
		L"3,3,,000001",
		NULL, NULL);
	RegisterInternalSource(__uuidof(CMPEG4VideoSourceFilter),
		L"4,4,,66747970",	// ftyp
		L"4,4,,6d6f6f76",	// moov
		L"4,4,,6d646174",	// mdat
		L"4,4,,736b6970",	// skip
		L"4,12,ffffffff00000000ffffffff,77696465027fe3706d646174",	// wide ? mdat
		L"3,3,,000001",
		NULL, NULL);
	RegisterInternalFilter(_countof(MPEG::sudFilters), MPEG::sudFilters);
	RegisterInternalSource(__uuidof(CMpegSourceFilter), 
		L"0,16,FFFFFFFFF100010001800001FFFFFFFF,000001BA2100010001800001000001BB",
		L"0,5,FFFFFFFFC0,000001BA40",
		L"0,8,fffffc00ffe00000,4156000055000000",
		L"0,1,,47,188,1,,47,376,1,,47",
		L"4,1,,47,196,1,,47,388,1,,47",
		L"0,4,,54467263,1660,1,,47",
		NULL, NULL);
	RegisterInternalFilter(_countof(VideoDec::sudFilters), VideoDec::sudFilters);
	RegisterInternalFilter(_countof(MpegDec::sudFilters), MpegDec::sudFilters);
	RegisterInternalFilter(_countof(AudioDec::sudFilters), AudioDec::sudFilters);
	RegisterInternalFilter(_countof(AudioSw::sudFilters), AudioSw::sudFilters, true);
	/* ϵͳ�˾� */
	RegisterSystemFilters(CLSID_LegacyAmFilterCategory);
	RegisterSystemSources();
}

void CFilterManager::EnumMatchingSources( CFile &file, MatchedFilters &filters )
{
	EnumMatchingSources(m_InternalCheckBytes, 0, file, filters);
	EnumMatchingSources(m_SystemCheckBytes, 1, file, filters);
}

void CFilterManager::EnumMatchingSources( SourceCheckBytes &chkBytesList, UINT nBasePriority, CFile &file, MatchedFilters &filters )
{
	for (SourceCheckBytes::const_iterator it = chkBytesList.begin(); it != chkBytesList.end(); it++)
	{
		bool bMatched = true;

		for (std::vector<CheckBytes>::const_iterator itSign = it->first.signs.begin(); itSign != it->first.signs.end(); itSign++)
		{
			const CheckBytes &chkBytes = *itSign;
			CAutoVectorPtr<char> szBuf;

			szBuf.Allocate(chkBytes.strSign.size());
			file.Seek(chkBytes.llOffset, chkBytes.llOffset >= 0 ? CFile::begin : CFile::end);
			if (file.Read(szBuf.m_p, chkBytes.strSign.size()) != chkBytes.strSign.size())
			{
				bMatched = false;
				break;
			}
			ApplyMask(szBuf.m_p, chkBytes.strMask.c_str(), chkBytes.strSign.size());
			if (memcmp(szBuf.m_p, chkBytes.strSign.c_str(), chkBytes.strSign.size()) != 0)
			{
				bMatched = false;
				break;
			}
		}
		if (bMatched)
		{
			filters.insert(MatchedFilter(it->second, nBasePriority));
		}
	}	
}

void CFilterManager::EnumMatchingFilters( const CAtlList<CMediaType>& mts, MatchedFilters& filters )
{
	UINT nPriority = 0; 
	EnumMatchingFilters(m_InternalMajorTypes, nPriority, mts, filters);
	EnumMatchingFilters(m_SystemMajorTypes, nPriority, mts, filters);
}

void CFilterManager::EnumMatchingFilters( FilterMajorTypes &majorTypes, UINT &nBasePriority, const CAtlList<CMediaType> &mts, MatchedFilters &filters )
{
	POSITION posMT = mts.GetHeadPosition();
	while (posMT != NULL)
	{
		const CMediaType &mt = mts.GetNext(posMT);
		FilterMajorTypes::iterator itMajor = majorTypes.find(mt.majortype);
		if (itMajor == majorTypes.end())
			continue;
		/* ��ȷƥ�� */
		std::pair<FilterMinorTypes::const_iterator, FilterMinorTypes::const_iterator> pii = itMajor->second.equal_range(mt.subtype);
		for (FilterMinorTypes::const_iterator itMinor = pii.first; itMinor != pii.second; itMinor++)
		{
			CFilter *pFilter = itMinor->second;
			filters.insert(MatchedFilter(pFilter, nBasePriority));
		}
		nBasePriority++;
	}
}

HRESULT CFilterManager::AddAudioSwitcherToGraph( IFilterGraph *pGraph, IBaseFilter **ppvObj )
{
	if (pGraph == NULL || ppvObj == NULL)
		return E_POINTER;
	FilterList::iterator it = m_InternalFilters.find(__uuidof(CAudioSwitcherFilter));
	if (it == m_InternalFilters.end())
		return E_FAIL;
	RIF(it->second->CreateInstance(NULL, ppvObj));
	return pGraph->AddFilter(*ppvObj, it->second->GetName());
}

HRESULT CFilterManager::RegisterInternalFilter( UINT nFilterCount, const InternalFilterSetupInfo* pSetupInfo, bool bFilterOnly )
{	
	if (m_InternalFilters.find(*pSetupInfo->pClsID) != m_InternalFilters.end())	// ��ע���ظ����˾�
		return S_FALSE;
	for (UINT i = 0; i < nFilterCount; i++)
	{
		const InternalFilterSetupInfo& setupInfo = pSetupInfo[i];
		CFilter *pFilter = new CFilterInternal(*setupInfo.pClsID, setupInfo.pszName, setupInfo.pfnCreateInstance, 
			setupInfo.nPinCount, setupInfo.pPins);
		m_InternalFilters.insert(CLSID(*setupInfo.pClsID), pFilter);
		if (!bFilterOnly)
		{
			/* ע���������� */
			for (UINT j = 0; j < setupInfo.nPinCount; j++)
			{
				const AMOVIESETUP_PIN &pinInfo = setupInfo.pPins[j];
				if (pinInfo.bOutput)
					continue;
				for (UINT k = 0; k < pinInfo.nMediaTypes; k++)
				{
					const REGPINTYPES &pinTypes = pinInfo.lpMediaType[k];
					m_InternalMajorTypes[*pinTypes.clsMajorType].insert(std::make_pair(*pinTypes.clsMinorType, pFilter));
				}
			}
		}		
	}
	return S_OK;
}

HRESULT CFilterManager::RegisterInternalSource( const CLSID &clsID, LPCWSTR pszSigns /*= NULL*/, LPCWSTR pszExts /*= NULL*/, ... )
{
	CFilter *pFilter;
	va_list args;

	FilterList::iterator it = m_InternalFilters.find(clsID);
	if (it == m_InternalFilters.end())
		return E_INVALIDARG;
	pFilter = it->second;
	va_start(args, pszSigns);
	while (pszSigns != NULL)
	{
		XTRACE(L"CheckBytes: %s\n", pszSigns);
		m_InternalCheckBytes.insert(std::make_pair(MediaSignature(pszSigns), pFilter));
		pszSigns = va_arg(args, LPCWSTR);
	}
	while (pszExts = va_arg(args, LPCWSTR))
	{
		XTRACE(L"Extension: %s\n", pszExts);
		m_InternalExtensions.insert(std::make_pair(CStringW(pszExts).MakeLower(), pFilter));
	}
	va_end(args);
	return S_OK;
}

HRESULT CFilterManager::RegisterSystemFilter( const RegisterFilterSetupInfo& setupInfo, bool bFilterOnly /*= false*/ )
{
	if (setupInfo.dwInPins != 0 && setupInfo.dwOutPins == 0)
		return S_FALSE;
	if (m_InternalFilters.find(setupInfo.clsID) != m_InternalFilters.end()
		|| m_SystemFilters.find(setupInfo.clsID) != m_SystemFilters.end())	// ��ע���ظ����˾�
		return S_FALSE;
	CFilter *pFilter = new CFilterRegister(setupInfo.clsID, setupInfo.strName);
	m_SystemFilters.insert(CLSID(setupInfo.clsID), pFilter);
	XTRACE(L"��ע��ϵͳ�˾� [%s(IN:%d, OUT:%d)]\n", setupInfo.strName, setupInfo.dwInPins, setupInfo.dwOutPins);
	if (!bFilterOnly)
	{
		/* ע���������� */
		for (boost::ptr_vector<RegisterPinSetupInfo>::const_iterator itPin = setupInfo.pins.begin(); itPin != setupInfo.pins.end(); itPin++)
		{
			const RegisterPinSetupInfo &pinInfo = *itPin;
			if (pinInfo.bOutput)
				continue;
			for (std::vector<MediaType>::const_iterator itMT = pinInfo.mts.begin(); itMT != pinInfo.mts.end(); itMT++)
			{
				const MediaType &mt = *itMT;
				m_SystemMajorTypes[mt.major].insert(std::make_pair(mt.minor, pFilter));
			}
		}
	}
	return S_OK;
}

HRESULT CFilterManager::RegisterSystemFilters( const CLSID &category )
{
	CComPtr<ICreateDevEnum> pCDE;
	CComPtr<IEnumMoniker> pEnum;

	RIF(pCDE.CoCreateInstance(CLSID_SystemDeviceEnum));
	RIF(pCDE->CreateClassEnumerator(CLSID_LegacyAmFilterCategory, &pEnum, CDEF_DEVMON_FILTER|CDEF_DEVMON_DMO));
	for (CComPtr<IMoniker> pMon; pEnum->Next(1, &pMon, NULL) == S_OK; pMon = NULL)
	{
		CComPtr<IPropertyBag> pProp;
		RegisterFilterSetupInfo filterInfo;
		CComVariant varCLSID, varName, varData; 
		CStringW strCLSID;
		LONG lIndex = 0;
		LPVOID pFilterData;
		UINT cbFilterData;

		if (FAILED(pMon->BindToStorage(NULL, NULL, IID_IPropertyBag, (LPVOID*)&pProp)))
			continue;
		/* �˾�CLSID */
		/*if (FAILED(pProp->Read(L"CLSID", &varCLSID, NULL)))
			continue;
		strCLSID = varCLSID.bstrVal;
		filterInfo.clsID = GUIDFromCString(strCLSID);*/
		/* �˾��� */
		if (FAILED(pProp->Read(L"FriendlyName", &varName, NULL)))
			continue;
		ASSERT(varName.vt == VT_BSTR);
		filterInfo.strName = varName.bstrVal;
		/* �˾�������Ϣ */
		if (FAILED(pProp->Read(L"FilterData", &varData, NULL)))
			continue;
		ASSERT(varData.vt == (VT_ARRAY|VT_UI1));
		SafeArrayLock(varData.parray);
		if (SafeArrayGetDim(varData.parray) != 1)
		{
			SafeArrayUnlock(varData.parray);
			continue;
		}
		SafeArrayAccessData(varData.parray, &pFilterData);
		cbFilterData = SafeArrayGetElemsize(varData.parray);
		DecodeFilterData((BYTE*)pFilterData, cbFilterData, filterInfo);
		SafeArrayUnlock(varData.parray);
		/* ע����˾� */
		RegisterSystemFilter(filterInfo, false);
	}
	return S_OK;
	
#if 0
	CStringW strCategory;
	HKEY hkFilters;
	DWORD dwFilters;
	WCHAR szSubKey[256];
	DWORD cchSubKey;

	strCategory = CStringFromGUID(category);
	if (RegOpenKeyExW(HKEY_CLASSES_ROOT, L"CLSID\\" + strCategory + L"\\Instance", 0, KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE, 
		&hkFilters) != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(GetLastError());
	if (RegQueryInfoKeyW(hkFilters, NULL, NULL, NULL, &dwFilters, NULL, NULL, NULL, NULL, NULL, NULL, 
		NULL) != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(GetLastError());
	cchSubKey = _countof(szSubKey);
	for (DWORD i = 0; i < dwFilters; i++, cchSubKey = _countof(szSubKey))
	{
		HKEY hkFilter;
		DWORD dwMaxValueLen;
		CStringW strCLSID;
		CAutoVectorPtr<BYTE> pValue;
		DWORD cbData;
		RegisterFilterSetupInfo filterInfo;
		
		if (RegEnumKeyExW(hkFilters, i, szSubKey, &cchSubKey, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
			break;
		if (RegOpenKeyExW(hkFilters, szSubKey, 0, KEY_QUERY_VALUE, &hkFilter) != ERROR_SUCCESS)
			break;
		if (RegQueryInfoKeyW(hkFilter, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &dwMaxValueLen, NULL, NULL))
			goto next_key;
		pValue.Allocate(dwMaxValueLen);
		/* �˾�CLSID */
		cbData = dwMaxValueLen;
		if (RegQueryValueExW(hkFilter, L"CLSID", NULL, NULL, pValue, &cbData) != ERROR_SUCCESS)
			goto next_key;
		strCLSID = (WCHAR*)pValue.m_p;
		filterInfo.clsID = GUIDFromCString(strCLSID);
		/* �˾��� */
		cbData = dwMaxValueLen;
		if (RegQueryValueExW(hkFilter, L"FriendlyName", NULL, NULL, pValue, &cbData) != ERROR_SUCCESS)
			goto next_key;
		filterInfo.strName = (WCHAR*)pValue.m_p;
		/* �˾�������Ϣ */
		cbData = dwMaxValueLen;
		if (RegQueryValueExW(hkFilter, L"FilterData", NULL, NULL, pValue, &cbData) != ERROR_SUCCESS)
			goto next_key;
		DecodeFilterData(pValue.m_p, cbData, filterInfo);
		/* ע����˾� */
		RegisterSystemFilter(filterInfo, false);
next_key:
		RegCloseKey(hkFilter);
	}
	RegCloseKey(hkFilters);
	return S_OK;
#endif
}

HRESULT CFilterManager::RegisterSystemSources( void )
{
	HKEY hkSources;
	DWORD dwFilters;
	WCHAR szSubKey[256];
	DWORD cchSubKey;

	if (RegOpenKeyExW(HKEY_CLASSES_ROOT, L"Media Type\\{e436eb83-524f-11ce-9f53-0020af0ba770}", 0, 
		KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE, &hkSources) != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(GetLastError());
	if (RegQueryInfoKeyW(hkSources, NULL, NULL, NULL, &dwFilters, NULL, NULL, NULL, NULL, NULL, NULL, 
		NULL) != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(GetLastError());
	for (DWORD i = 0; i < dwFilters; i++, cchSubKey = _countof(szSubKey))
	{
		HKEY hkSource;
		DWORD dwMaxValueLen;
		CAutoVectorPtr<BYTE> pValue;
		DWORD cbData;
		CStringW strCLSID;
		CLSID clsID;
		FilterList::iterator itFilter;
		CFilter *pFilter;
		
		if (RegEnumKeyExW(hkSources, i, szSubKey, &cchSubKey, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
			break;
		if (RegOpenKeyExW(hkSources, szSubKey, 0, KEY_QUERY_VALUE, &hkSource) != ERROR_SUCCESS)
			break;
		if (RegQueryInfoKeyW(hkSource, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &dwMaxValueLen, NULL, NULL))
			goto next_key;
		pValue.Allocate(dwMaxValueLen);
		// Դ�˾�CLSID
		cbData = dwMaxValueLen;
		if (RegQueryValueExW(hkSource, L"Source Filter", NULL, NULL, pValue, &cbData) != ERROR_SUCCESS)
			goto next_key;
		strCLSID = (WCHAR*)pValue.m_p;
		clsID = GUIDFromCString(strCLSID);
		if (clsID == CLSID_AsyncReader || clsID == CLSID_URLReader)
			goto next_key;
		itFilter = m_SystemFilters.find(clsID);
		if (itFilter == m_SystemFilters.end())
			goto next_key;
		pFilter = itFilter->second;
		// �ļ����
		for (int iIndex = 0; ; iIndex++)
		{
			CStringW strValueName;

			strValueName.Format(L"%d", iIndex);
			cbData = dwMaxValueLen;
			if (RegQueryValueExW(hkSource, strValueName, NULL, NULL, pValue, &cbData) != ERROR_SUCCESS)
				break;
			m_SystemCheckBytes.insert(std::make_pair(MediaSignature((WCHAR*)pValue.m_p), pFilter));
		}	
next_key:
		RegCloseKey(hkSource);
	}
	return S_OK;
}

HRESULT CFilterManager::DecodeFilterData( BYTE* pData, DWORD cbData, RegisterFilterSetupInfo &info )
{
	struct Header
	{
		DWORD dwVersion;
		DWORD dwMerit;
		DWORD dwPins;
		DWORD dwReserved;
	};

#define ChkLen(size) if(pPtr - pData + size > (int)cbData) return E_FAIL;

	BYTE *pPtr = pData;
	ChkLen(sizeof(Header));
	Header *pHeader = (Header*)pPtr;
	pPtr += sizeof(Header);
	if (pHeader->dwVersion != 2)	// ֻ֧�ְ汾2
		return E_FAIL;
	info.dwMerit = pHeader->dwMerit;
	info.pins.clear();
	info.dwInPins = info.dwOutPins = 0;
	while (pHeader->dwPins-- > 0)
	{
		std::auto_ptr<RegisterPinSetupInfo> pPinInfo(new RegisterPinSetupInfo);	
		ChkLen(1);
		BYTE n = *pPtr - 0x30;
		pPtr++;
		ChkLen(2);
		WORD pi = *(WORD*)pPtr;
		pPtr += 2;
		ASSERT(pi == 'ip');
		ChkLen(1);
		BYTE x33 = *pPtr;
		pPtr++;
		ASSERT(x33 == 0x33);
		ChkLen(8);
		pPinInfo->bOutput = (*pPtr & REG_PINFLAG_B_OUTPUT) != 0;
		if (pPinInfo->bOutput)
			info.dwOutPins++;
		else
			info.dwInPins++;
		pPtr += 8;
		ChkLen(12);
		DWORD dwTypes = *(DWORD*)pPtr;
		pPtr += 12;
		while (dwTypes-- > 0)
		{
			ChkLen(1);
			BYTE n = *pPtr - 0x30;
			pPtr++;
			ChkLen(2);
			WORD ty = *(WORD*)pPtr;
			pPtr += 2;
			ASSERT(ty == 'yt');
			ChkLen(5);
			BYTE x33 = *pPtr;
			pPtr++;
			ASSERT(x33 == 0x33);
			pPtr += 4;
			ChkLen(8);
			if (*(DWORD*)pPtr < (DWORD)(pPtr - pData + 8) || *(DWORD*)pPtr >= cbData
				|| *(DWORD*)(pPtr + 4) < (DWORD)(pPtr - pData + 8) || *(DWORD*)(pPtr + 4) >= cbData)
			{
				pPtr += 8;
				continue;
			}
			GUID majorType = *(GUID*)&pData[*(DWORD*)pPtr];
			pPtr += 4;
			GUID subType = *(GUID*)&pData[*(DWORD*)pPtr];
			pPtr += 4;
			pPinInfo->mts.push_back(MediaType(majorType, subType));
		}
		info.pins.push_back(pPinInfo);
	}
	return S_OK;

#undef ChkLen
}



