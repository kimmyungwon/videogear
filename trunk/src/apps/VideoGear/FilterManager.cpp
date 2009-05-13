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

	const InternalFilterSetupInfo sudFilters[] =
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

	const InternalFilterSetupInfo sudFilters[] =
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

	const InternalFilterSetupInfo sudFilters[] =
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
	/* �ڲ��˾� */
	RegisterInternalFilter(_countof(AVI::sudFilters), AVI::sudFilters);
	RegisterInternalFilter(_countof(MKV::sudFilters), MKV::sudFilters);
	RegisterInternalFilter(_countof(RMDec::sudFilters), RMDec::sudFilters);
	RegisterInternalFilter(_countof(VideoDec::sudFilters), VideoDec::sudFilters);
	RegisterInternalFilter(_countof(AudioDec::sudFilters), AudioDec::sudFilters);
	RegisterInternalFilter(_countof(AudioSw::sudFilters), AudioSw::sudFilters, true);
	/* ϵͳ�˾� */
	RegisterSystemFilters();
}

CFilterManager::~CFilterManager(void)
{
	m_InternalFilters.clear();
	m_SystemFilters.clear();
}

HRESULT CFilterManager::EnumMatchingFilters( const CAtlList<CMediaType>& mts, CAtlList<CFilter*>& filters )
{
	UINT nInitCount = filters.GetCount();

	POSITION posMT = mts.GetHeadPosition();
	while (posMT != NULL)
	{
		const CMediaType &mt = mts.GetNext(posMT);
		MajorTypes::iterator it = m_InternalMajorTypes.find(mt.majortype);
		if (it == m_InternalMajorTypes.end())
			continue;
		/* ��ȷƥ�� */
		std::pair<MinorTypes::const_iterator, MinorTypes::const_iterator> pii = it->second.equal_range(mt.subtype);
		for (MinorTypes::const_iterator it = pii.first; it != pii.second; it++)
		{
			CFilter *pFilter = it->second;
			filters.AddTail(pFilter);
		}
		/* ģ��ƥ�� */
		if (mt.subtype != GUID_NULL)
		{
			std::pair<MinorTypes::const_iterator, MinorTypes::const_iterator> pii = it->second.equal_range(GUID_NULL);
			for (MinorTypes::const_iterator it = pii.first; it != pii.second; it++)
			{
				CFilter *pFilter = it->second;
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
	FilterList::iterator it = m_InternalFilters.find(__uuidof(CAudioSwitcherFilter));
	if (it == m_InternalFilters.end())
		return E_FAIL;
	RIF(it->second->CreateInstance(NULL, ppvObj));
	return pGraph->AddFilter(*ppvObj, it->second->GetName());
}

HRESULT CFilterManager::RegisterInternalFilter( UINT nFilterCount, const InternalFilterSetupInfo* pSetupInfo, bool bFilterOnly )
{	
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

HRESULT CFilterManager::RegisterSystemFilter( const RegisterFilterSetupInfo& setupInfo, bool bFilterOnly /*= false*/ )
{
	if (setupInfo.dwInPins == 0 && setupInfo.dwOutPins == 0)	// ��ע��Source
		return S_FALSE;
	if (setupInfo.dwInPins != 0 && setupInfo.dwOutPins == 0)	// ��ע��Renderer
		return S_FALSE;
	CFilter *pFilter = new CFilterRegister(setupInfo.clsID, setupInfo.strName);
	m_SystemFilters.insert(CLSID(setupInfo.clsID), pFilter);
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

HRESULT CFilterManager::RegisterSystemFilters( void )
{
	CStringW strCategory;
	HKEY hkFilters;
	DWORD dwFilters;
	WCHAR szSubKey[256];
	DWORD cchSubKey;

	strCategory = CStringFromGUID(CLSID_LegacyAmFilterCategory);
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

