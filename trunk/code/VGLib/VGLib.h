// VGLib.h : VGLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "VGFilterList.h"

// CVGLibApp
// �йش���ʵ�ֵ���Ϣ������� VGLib.cpp
//

class CVGLibApp : public CWinApp
{
public:
	CVGLibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "strmiids.lib")
#ifdef DEBUG
#pragma comment(lib, "strmbasd.lib")
#pragma comment(lib, "RealMediaSplitterDU.lib")
#endif

//////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE VGEnumMatchingFilters( IVGFilterList **ppEnum,
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
											  const CLSID *pPinCategoryOut );
