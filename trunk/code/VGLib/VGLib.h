// VGLib.h : VGLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

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
#pragma comment(lib, "zlibD.lib")
#pragma comment(lib, "strmbasd.lib")
//#pragma comment(lib, "dsutilDU.lib")
#pragma comment(lib, "filtersDU.lib")
#pragma comment(lib, "RealMediaSplitterDU.lib")
#pragma comment(lib, "MatroskaSplitterDU.lib")
#pragma comment(lib, "audioswitcherDU.lib")
#else
#pragma comment(lib, "zlibR.lib")
#pragma comment(lib, "strmbase.lib")
//#pragma comment(lib, "dsutilRU.lib")
#pragma comment(lib, "filtersRU.lib")
#pragma comment(lib, "RealMediaSplitterRU.lib")
#pragma comment(lib, "MatroskaSplitterRU.lib")
#pragma comment(lib, "audioswitcherRU.lib")
#endif
