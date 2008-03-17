// VGLib.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "VGLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CVGLibApp

BEGIN_MESSAGE_MAP(CVGLibApp, CWinApp)
END_MESSAGE_MAP()


// CVGLibApp 构造

CVGLibApp::CVGLibApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CVGLibApp 对象

CVGLibApp theApp;


// CVGLibApp 初始化

BOOL CVGLibApp::InitInstance()
{
	CWinApp::InitInstance();

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(m_hInstance, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	PathAddBackslash(szPath);
	g_strDrvPath = szPath;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

struct SourceFilterInfoW
{
	CLSID clsid;
	const CFactoryTemplate *pTemplate;
	TCHAR *szChkBytes; 
	UINT nExts;
	LPCTSTR *lpExts;
};

namespace VGF_RM {

	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn), sudPinTypesIn},
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
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn2), sudPinTypesIn2},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesOut2), sudPinTypesOut2}
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
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn3), sudPinTypesIn3},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesOut3), sudPinTypesOut3}
	};

	const AMOVIESETUP_FILTER sudFilter[] =
	{
		{&__uuidof(CRealMediaSplitterFilter), L"RealMedia Splitter", MERIT_NORMAL, countof(sudpPins), sudpPins},
		{&__uuidof(CRealMediaSourceFilter), L"RealMedia Source", MERIT_NORMAL, 0, NULL},
		{&__uuidof(CRealVideoDecoder), L"RealVideo Decoder", MERIT_NORMAL, countof(sudpPins2), sudpPins2},
		{&__uuidof(CRealAudioDecoder), L"RealAudio Decoder", MERIT_NORMAL, countof(sudpPins3), sudpPins3},
	};

	CFactoryTemplate g_Templates[] =
	{
		{sudFilter[0].strName, sudFilter[0].clsID, CreateInstance<CRealMediaSplitterFilter>, NULL, &sudFilter[0]},
		{sudFilter[1].strName, sudFilter[1].clsID, CreateInstance<CRealMediaSourceFilter>, NULL, &sudFilter[1]},
		{sudFilter[2].strName, sudFilter[2].clsID, CreateInstance<CRealVideoDecoder>, NULL, &sudFilter[2]},
		{sudFilter[3].strName, sudFilter[3].clsID, CreateInstance<CRealAudioDecoder>, NULL, &sudFilter[3]},
	};

	int g_cTemplates = countof(g_Templates);

	LPCTSTR g_Exts[] = { _T(".rm"), _T(".rmvb"), _T(".ram") };

	SourceFilterInfoW g_Sources[] = 
	{
		{CLSID_AsyncReader, &g_Templates[1], _T("0,4,,2E524D46"), countof(g_Exts), g_Exts}
	};

	int g_cSources = countof(g_Sources);

};

namespace VGF_MK {
	
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{	
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_Matroska},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL}
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const AMOVIESETUP_FILTER sudFilter[] =
	{
		{&__uuidof(CMatroskaSplitterFilter), L"Matroska Splitter", MERIT_NORMAL, countof(sudpPins), sudpPins},
		{&__uuidof(CMatroskaSourceFilter), L"Matroska Source", MERIT_NORMAL, 0, NULL},
	};

	CFactoryTemplate g_Templates[] =
	{
		{sudFilter[0].strName, sudFilter[0].clsID, CreateInstance<CMatroskaSplitterFilter>, NULL, &sudFilter[0]},
		{sudFilter[1].strName, sudFilter[1].clsID, CreateInstance<CMatroskaSourceFilter>, NULL, &sudFilter[1]},
	};

	int g_cTemplates = countof(g_Templates);

	LPCTSTR g_Exts[] = { _T(".mkv"), _T(".mka"), _T(".mks") };

	SourceFilterInfoW g_Sources[] = 
	{
		{CLSID_AsyncReader, &g_Templates[1], _T("0,4,,1A45DFA3"), countof(g_Exts), g_Exts}
	};

	int g_cSources = countof(g_Sources);

};

//////////////////////////////////////////////////////////////////////////

inline bool MatchGUID(const GUID &guid1, const GUID &guid2)
{
	if (IsEqualCLSID(guid1, guid2))
		return true;
	else
		return (IsEqualCLSID(GUID_NULL, guid1) || IsEqualCLSID(GUID_NULL, guid2));
}

inline bool MatchPin(const AMOVIESETUP_PIN *pPinInfo, CLSID clsMaj, CLSID clsSub)
{
	for (UINT j = 0; j < pPinInfo->nMediaTypes; j++)
	{
		if (MatchGUID(clsMaj, *pPinInfo->lpMediaType[j].clsMajorType) && MatchGUID(clsSub, *pPinInfo->lpMediaType[j].clsMinorType))
			return true;
	}
	return false;
}

HRESULT EnumMatchingFilters( CFactoryTemplate* pTemplates,
							int nTemplates,
							IVGFilterList *pList,
							DWORD dwMerit,
							BOOL bInputNeeded,
							CLSID clsInMaj,
							CLSID clsInSub,
							BOOL bRender,
							BOOL bOutputNeeded,
							CLSID clsOutMaj,
							CLSID clsOutSub )
{
	CheckPointer(pTemplates, E_POINTER);
	CheckPointer(pList, E_POINTER);

	HRESULT hr = S_OK;
	bool bMatchIn = false, bMatchOut = false;
	IBaseFilter *pFilter = NULL;
	const AMOVIESETUP_FILTER *pFilterInfo;
	const AMOVIESETUP_PIN *pPinInfo;
	UINT nInPins = 0, nOutPins = 0;
	
	for (int idxFilter = 0; idxFilter < nTemplates; idxFilter++)
	{
		pFilterInfo = pTemplates[idxFilter].m_pAMovieSetup_Filter;
		bMatchIn = true; bMatchOut = true;

		// 如果该滤镜没有Pin则认为不匹配
		if (pFilterInfo->nPins == 0)
			continue;

		nInPins = 0;
		nOutPins = 0;
		for (UINT idxPin = 0; idxPin < pFilterInfo->nPins; idxPin++)
		{
			pPinInfo = &pFilterInfo->lpPin[idxPin];

			// 如果该Pin没有注册任何MediaType则认为不匹配
			if (pPinInfo->nMediaTypes == 0)
				continue;
			
			if (!pPinInfo->bOutput) 
			{
				nInPins++;
				bMatchIn = MatchPin(pPinInfo, clsInMaj, clsInSub);
				if (!bMatchIn)
					break;
			}
			else
			{
				nOutPins++;
				bMatchOut = MatchPin(pPinInfo, clsOutMaj, clsOutSub);
				if (bRender && !pPinInfo->bRendered)
					bMatchOut = false;
				if (!bMatchOut)
					break;
			}
		}	// end for idxPin

		if (bInputNeeded && nInPins == 0)
			bMatchIn = false;
		if (bOutputNeeded && nOutPins == 0)
			bMatchOut = false;
		// 如果匹配则添加到结果列表
		if (!bMatchIn || !bMatchOut)
			continue;
		pFilter = (CBaseFilter*)pTemplates[idxFilter].CreateInstance(NULL, &hr);
		if (SUCCEEDED(hr))
			pList->Add(pFilter, pTemplates[idxFilter].m_Name);
	}

	return pList->GetCount() > 0 ? S_OK : E_FAIL;
}

HRESULT STDMETHODCALLTYPE VGEnumMatchingFilters( IVGFilterList **ppList,
												DWORD dwMerit,
												BOOL bInputNeeded,
												CLSID clsInMaj,
												CLSID clsInSub,
												BOOL bRender,
												BOOL bOutputNeeded,
												CLSID clsOutMaj,
												CLSID clsOutSub )
{
	CheckPointer(ppList, E_POINTER);


	IVGFilterListPtr pFilters = new CVGFilterList;
	EnumMatchingFilters(VGF_RM::g_Templates, VGF_RM::g_cTemplates, pFilters, dwMerit, bInputNeeded, clsInMaj, clsInSub, bRender, bOutputNeeded, clsOutMaj, clsOutSub);
	EnumMatchingFilters(VGF_MK::g_Templates, VGF_MK::g_cTemplates, pFilters, dwMerit, bInputNeeded, clsInMaj, clsInSub, bRender, bOutputNeeded, clsOutMaj, clsOutSub);

	*ppList = pFilters;
	(*ppList)->AddRef();
	return (*ppList)->GetCount() > 0 ? S_OK : E_FAIL;
}

HRESULT EnumMatchingSource( SourceFilterInfoW *pSources, int nSources, LPCTSTR lpszFile, IBaseFilter **ppBF )
{
	CheckPointer(pSources, E_POINTER);
	CheckPointer(ppBF, E_POINTER);

	HRESULT hr;
	CString strExt = PathFindExtension(lpszFile);
	CComPtr<IFileSourceFilter> pFileSrc;
	
	for (int i = 0; i < nSources; i++)
	{
		for (UINT j = 0; j < pSources[i].nExts; j++)
		{
			if (strExt.CompareNoCase(pSources[i].lpExts[j]) == 0)
			{
				*ppBF = (CBaseFilter*)pSources[i].pTemplate->CreateInstance(NULL, &hr);
				if (SUCCEEDED(hr))
				{
					(*ppBF)->AddRef();
					hr = (*ppBF)->QueryInterface(IID_IFileSourceFilter, (LPVOID*)&pFileSrc);
					if (SUCCEEDED(hr) && SUCCEEDED(pFileSrc->Load(lpszFile, NULL)))
					{
						//pFileSrc->Release();
						return S_OK;
					}
				}
				break;
			}
		}
	}

	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE VGEnumMatchingSource( LPCTSTR lpszFile, IBaseFilter **ppBF )
{
	CheckPointer(ppBF, E_POINTER);
	if (!PathFileExists(lpszFile))
		return E_FAIL;

	HRESULT hr;

	hr = EnumMatchingSource(VGF_RM::g_Sources, VGF_RM::g_cSources, lpszFile, ppBF);
	if (SUCCEEDED(hr))
		return S_OK;
	hr = EnumMatchingSource(VGF_MK::g_Sources, VGF_MK::g_cSources, lpszFile, ppBF);
	if (SUCCEEDED(hr))
		return S_OK;
	// 没有合适的Source，使用默认的
	hr = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)ppBF);
	if (SUCCEEDED(hr))
		(*ppBF)->AddRef();
	return hr;
}