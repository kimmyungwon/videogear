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

#pragma comment(lib, "quartz.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "strmiids.lib")
#ifdef DEBUG
#pragma comment(lib, "strmbasd.lib")
#pragma comment(lib, "dsutilDU.lib")
#pragma comment(lib, "filtersDU.lib")
#pragma comment(lib, "RealMediaSplitterDU.lib")
#else
#pragma comment(lib, "strmbase.lib")
#pragma comment(lib, "dsutilRU.lib")
#pragma comment(lib, "filtersRU.lib")
#pragma comment(lib, "RealMediaSplitterRU.lib")
#endif
