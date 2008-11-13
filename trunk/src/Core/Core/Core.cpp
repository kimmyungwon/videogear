// Core.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Core.h"
#include "Player.h"

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

// CCoreApp

BEGIN_MESSAGE_MAP(CCoreApp, CWinApp)
END_MESSAGE_MAP()


// CCoreApp ����

CCoreApp::CCoreApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCoreApp ����

CCoreApp theApp;


// CCoreApp ��ʼ��

BOOL CCoreApp::InitInstance()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	
	CWinApp::InitInstance();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

STDAPI VGCreatePlayer(__deref_out void **ppvObj)
{
	CheckPointer(ppvObj, E_POINTER);

	IPlayer *pPlayer;

	pPlayer = (IPlayer*)new CPlayer;
	pPlayer->AddRef();
	*ppvObj = (void*)pPlayer;
	return S_OK;
}