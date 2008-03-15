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

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

namespace VGF_RM
{
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
};

//////////////////////////////////////////////////////////////////////////

bool MatchPin(const AMOVIESETUP_PIN *pPinInfo, DWORD cTypes, const GUID *pTypes)
{
	return false;
}

HRESULT EnumMatchingFilters( CFactoryTemplate* pTemplates,
							int nTemplates,
							IVGFilterList *pList,
							DWORD dwFlags,
							BOOL bExactMatch,
							DWORD dwMerit,
							BOOL bInputNeeded,
							DWORD cInputTypes,
							const GUID *pInputTypes,
							const REGPINMEDIUM *pMedIn,
							const CLSID *pPinCategoryIn,
							BOOL bRender,
							BOOL bOutputNeeded,
							DWORD cOutputTypes,
							const GUID *pOutputTypes,
							const REGPINMEDIUM *pMedOut,
							const CLSID *pPinCategoryOut )
{
	CheckPointer(pList, E_POINTER);

	HRESULT hr = S_OK;
	bool bMatchIn = false, bMatchOut = false;
	IBaseFilter *pFilter = NULL;
	const AMOVIESETUP_FILTER *pFilterInfo;
	const AMOVIESETUP_PIN *pPinInfo;
	UINT nInPins = 0, nOutPins = 0;
	
	for (int idxFilter = 0; idxFilter < nTemplates - 1; idxFilter++)
	{
		pFilterInfo = pTemplates[idxFilter].m_pAMovieSetup_Filter;
		bMatchIn = false; bMatchOut = false;

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
				bMatchIn = MatchPin(pPinInfo, cInputTypes, pInputTypes);
				if (!bMatchIn)
					break;
			}
			else
			{
				nOutPins++;
				bMatchOut = MatchPin(pPinInfo, cOutputTypes, pOutputTypes);
				if (!bMatchOut)
					break;
			}
		}	// end for idxPin

		// 如果匹配则添加到结果列表
		if (bInputNeeded && nInPins == 0)
			continue;
		if (bOutputNeeded && nOutPins == 0)
			continue;	
		if (!bMatchIn || !bMatchOut)
			continue;
		pFilter = (CBaseFilter*)pTemplates[idxFilter].CreateInstance(NULL, &hr);
		if (SUCCEEDED(hr))
			pList->Add(pFilter);
	}

	return pList->GetCount() > 0 ? S_OK : E_FAIL;
}

HRESULT STDMETHODCALLTYPE VGEnumMatchingFilters( IVGFilterList **ppList,
												DWORD dwFlags,
												BOOL bExactMatch,
												DWORD dwMerit,
												BOOL bInputNeeded,
												DWORD cInputTypes,
												const GUID *pInputTypes,
												const REGPINMEDIUM *pMedIn,
												const CLSID *pPinCategoryIn,
												BOOL bRender,
												BOOL bOutputNeeded,
												DWORD cOutputTypes,
												const GUID *pOutputTypes,
												const REGPINMEDIUM *pMedOut,
												const CLSID *pPinCategoryOut )
{
	CheckPointer(ppList, E_POINTER);

	HRESULT hr;
	IVGFilterListPtr pFilters = new CVGFilterList;

	hr = EnumMatchingFilters(VGF_RM::g_Templates, VGF_RM::g_cTemplates, pFilters, dwFlags, bExactMatch, dwMerit, bInputNeeded, cInputTypes, pInputTypes, pMedIn, 
							pPinCategoryIn, bRender, bOutputNeeded, cOutputTypes, pOutputTypes, pMedOut, pPinCategoryOut);
	if (FAILED(hr))
		return hr;

	*ppList = pFilters;
	return S_OK;
}