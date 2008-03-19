// VGLib.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "VGLib.h"
#include "VGPlayer.h"

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

const AMOVIESETUP_MEDIATYPE sudPinTypesIn4[] =
{	
	{&MEDIATYPE_Stream, &MEDIASUBTYPE_Matroska},
	{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL}
};

const AMOVIESETUP_PIN sudpPins4[] =
{
	{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn4), sudPinTypesIn4},
	{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
};

const AMOVIESETUP_MEDIATYPE sudPinTypesIn5[] =
{
	{&MEDIATYPE_Audio, &MEDIASUBTYPE_NULL}
};

const AMOVIESETUP_MEDIATYPE sudPinTypesOut5[] =
{
	{&MEDIATYPE_Audio, &MEDIASUBTYPE_NULL}
};

const AMOVIESETUP_PIN sudpPins5[] =
{
	{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn5), sudPinTypesIn5},
	{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesOut5), sudPinTypesOut5}
};

const AMOVIESETUP_FILTER sudFilter[] =
{
	{&__uuidof(CRealMediaSplitterFilter), L"RealMedia Splitter", MERIT_NORMAL, countof(sudpPins), sudpPins},
	{&__uuidof(CRealMediaSourceFilter), L"RealMedia Source", MERIT_NORMAL, 0, NULL},
	{&__uuidof(CRealVideoDecoder), L"RealVideo Decoder", MERIT_NORMAL, countof(sudpPins2), sudpPins2},
	{&__uuidof(CRealAudioDecoder), L"RealAudio Decoder", MERIT_NORMAL, countof(sudpPins3), sudpPins3},
	{&__uuidof(CMatroskaSplitterFilter), L"Matroska Splitter", MERIT_NORMAL, countof(sudpPins4), sudpPins4},
	{&__uuidof(CMatroskaSourceFilter), L"Matroska Source", MERIT_NORMAL, 0, NULL},
	{&__uuidof(CAudioSwitcherFilter), L"AudioSwitcher", MERIT_DO_NOT_USE, countof(sudpPins5), sudpPins5},
};

CFactoryTemplate g_Templates[] =
{
	{sudFilter[0].strName, sudFilter[0].clsID, CreateInstance<CRealMediaSplitterFilter>, NULL, &sudFilter[0]},
	{sudFilter[1].strName, sudFilter[1].clsID, CreateInstance<CRealMediaSourceFilter>, NULL, &sudFilter[1]},
	{sudFilter[2].strName, sudFilter[2].clsID, CreateInstance<CRealVideoDecoder>, NULL, &sudFilter[2]},
	{sudFilter[3].strName, sudFilter[3].clsID, CreateInstance<CRealAudioDecoder>, NULL, &sudFilter[3]},
	{sudFilter[4].strName, sudFilter[4].clsID, CreateInstance<CMatroskaSplitterFilter>, NULL, &sudFilter[4]},
	{sudFilter[5].strName, sudFilter[5].clsID, CreateInstance<CMatroskaSourceFilter>, NULL, &sudFilter[5]},
	{sudFilter[6].strName, sudFilter[6].clsID, CreateInstance<CAudioSwitcherFilter>, NULL, &sudFilter[6]}
};

int g_cTemplates = countof(g_Templates);

LPCWSTR g_Exts[] = { _T(".rm"), _T(".rmvb"), _T(".ram") };
LPCWSTR g_Exts2[] = { _T(".mkv"), _T(".mka"), _T(".mks") };

SourceFilterInfoW g_Sources[] = 
{
	{CLSID_AsyncReader, &g_Templates[1], _T("0,4,,2E524D46"), countof(g_Exts), g_Exts},
	{CLSID_AsyncReader, &g_Templates[5], _T("0,4,,1A45DFA3"), countof(g_Exts2), g_Exts2},
};

int g_cSources = countof(g_Sources);

//////////////////////////////////////////////////////////////////////////

typedef void(CALLBACK *LPFNEnumFilterProc)(PVOID pUser, const CLSID* clsID, LPCWSTR lpszName, DWORD dwMerit,
											UINT nPins, const REGFILTERPINS* lpPin);
typedef void(CALLBACK *LPFNEnumSourceProc)(PVOID pUser, const CLSID* clsID, LPCWSTR lpszName, DWORD dwMerit,
												 UINT nPins, const REGFILTERPINS* lpPin, LPCWSTR lpszChkBytes,
												 UINT nExts, LPCWSTR* ppszExts);

__forceinline void EnumInternalFilter(const AMOVIESETUP_FILTER& filter, LPFNEnumFilterProc lpfnProc, PVOID pUser)
{
	lpfnProc(pUser, filter.clsID, filter.strName, filter.dwMerit, filter.nPins, filter.lpPin);
}

__forceinline void EnumInternalSource(const AMOVIESETUP_FILTER& filter, const SourceFilterInfoW& source, 
									  LPFNEnumSourceProc lpfnProc, PVOID pUser)
{
	lpfnProc(pUser, filter.clsID, filter.strName, filter.dwMerit, filter.nPins, filter.lpPin, source.szChkBytes, 
		source.nExts, source.lpExts);
}

HRESULT STDMETHODCALLTYPE VGCreateInstance(const CLSID* clsID, IBaseFilter **ppBF)
{
	CheckPointer(ppBF, E_POINTER);

	HRESULT hr;
	
	for (int i=0; i<g_cTemplates; i++)
	{
		if (IsEqualCLSID(*g_Templates[i].m_ClsID, *clsID))
		{
			(*ppBF) = (CBaseFilter*)g_Templates[i].CreateInstance(NULL, &hr);
			if (FAILED(hr))
				return hr;
			(*ppBF)->AddRef();
			return S_OK;
		}
	}
	*ppBF = NULL;
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE VGEnumInternalFilters(LPFNEnumFilterProc lpfnProc, PVOID pUser)
{
	CheckPointer(lpfnProc, E_POINTER);
	
	EnumInternalFilter(sudFilter[0], lpfnProc, pUser);
	EnumInternalFilter(sudFilter[2], lpfnProc, pUser);
	EnumInternalFilter(sudFilter[3], lpfnProc, pUser);
	EnumInternalFilter(sudFilter[4], lpfnProc, pUser);
	EnumInternalFilter(sudFilter[6], lpfnProc, pUser);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE VGEnumInternalSources(LPFNEnumSourceProc lpfnProc, PVOID pUser)
{
	CheckPointer(lpfnProc, E_POINTER);

	EnumInternalSource(*g_Sources[0].pTemplate->m_pAMovieSetup_Filter, g_Sources[0], lpfnProc, pUser);
	EnumInternalSource(*g_Sources[1].pTemplate->m_pAMovieSetup_Filter, g_Sources[1], lpfnProc, pUser);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE VGCreatePlayer(IVGPlayer **ppvObject)
{
	CheckPointer(ppvObject, E_POINTER);

	IVGPlayerPtr pPlayer = new CVGPlayer;
	*ppvObject = pPlayer;
	return S_OK;
}