// VGLib.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "VGLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CVGLibApp

BEGIN_MESSAGE_MAP(CVGLibApp, CWinApp)
END_MESSAGE_MAP()


// CVGLibApp ����

CVGLibApp::CVGLibApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CVGLibApp ����

CVGLibApp theApp;


// CVGLibApp ��ʼ��

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
	
	for (int idxFilter = 0; idxFilter < nTemplates - 1; idxFilter++)
	{
		pFilterInfo = pTemplates[idxFilter].m_pAMovieSetup_Filter;
		bMatchIn = true; bMatchOut = true;

		// ������˾�û��Pin����Ϊ��ƥ��
		if (pFilterInfo->nPins == 0)
			continue;

		nInPins = 0;
		nOutPins = 0;
		for (UINT idxPin = 0; idxPin < pFilterInfo->nPins; idxPin++)
		{
			pPinInfo = &pFilterInfo->lpPin[idxPin];

			// �����Pinû��ע���κ�MediaType����Ϊ��ƥ��
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
		// ���ƥ������ӵ�����б�
		if (!bMatchIn || !bMatchOut)
			continue;
		pFilter = (CBaseFilter*)pTemplates[idxFilter].CreateInstance(NULL, &hr);
		if (SUCCEEDED(hr))
			pList->Add(pFilter);
	}

	return pList->GetCount() > 0 ? S_OK : S_FALSE;
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

	HRESULT hr;
	IVGFilterListPtr pFilters = new CVGFilterList;

	hr = EnumMatchingFilters(VGF_RM::g_Templates, VGF_RM::g_cTemplates, pFilters, dwMerit, bInputNeeded, clsInMaj, clsInSub, bRender, bOutputNeeded, clsOutMaj, clsOutSub);
	if (FAILED(hr))
		return hr;

	*ppList = pFilters;
	(*ppList)->AddRef();
	return S_OK;
}

HRESULT EnumMatchingSource( SourceFilterInfoW *pSources, int nSources, IVGFilterList *pList, LPCTSTR lpszFile )
{
	CheckPointer(pSources, E_POINTER);
	CheckPointer(pList, E_POINTER);

	HRESULT hr;
	CString strExt = PathFindExtension(lpszFile);
	IBaseFilter *pFilter = NULL;
	
	for (int i = 0; i < nSources; i++)
	{
		for (UINT j = 0; j < pSources[i].nExts; j++)
		{
			if (strExt.CompareNoCase(pSources[i].lpExts[j]) == 0)
			{
				pFilter = (CBaseFilter*)pSources[i].pTemplate->CreateInstance(NULL, &hr);
				if (SUCCEEDED(hr))
					pList->Add(pFilter);
				break;
			}
		}
	}

	return pList->GetCount() > 0 ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE VGEnumMatchingSource( IVGFilterList **ppList, LPCTSTR lpszFile )
{
	CheckPointer(ppList, E_POINTER);

	HRESULT hr;
	*ppList = new CVGFilterList;
	(*ppList)->AddRef();

	hr = EnumMatchingSource(VGF_RM::g_Sources, VGF_RM::g_cSources, *ppList, lpszFile);
	if (SUCCEEDED(hr))
	{
		(*ppList)->AddRef();
		return S_OK;
	}
	// û�к��ʵ�Source��ʹ��Ĭ�ϵ�
	IBaseFilter *pFilter = NULL;
	hr = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)&pFilter);
	if (SUCCEEDED(hr))
		(*ppList)->Add(pFilter);
	return hr;
}