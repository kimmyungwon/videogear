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

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE VGEnumMatchingFilters( IEnumMoniker **ppEnum,
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
	
	
	return E_NOTIMPL;
}