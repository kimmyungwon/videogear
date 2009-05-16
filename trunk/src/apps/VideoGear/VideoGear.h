
// VideoGear.h : VideoGear Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

// CVideoGearApp:
// �йش����ʵ�֣������ VideoGear.cpp
//

class CVideoGearApp : public CWinApp
{
public:
	CVideoGearApp();
	~CVideoGearApp();
	virtual BOOL InitInstance();
private:
	Gdiplus::GdiplusStartupInput m_gpInput;
	ULONG_PTR m_ulToken;
private:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
};

extern CVideoGearApp theApp;

#define SAFE_DELETE(ptr)	{if((ptr) != NULL) {delete (ptr); (ptr) = NULL;}}
#define RIF(cond)	{HRESULT hr = (cond); if (FAILED(hr)) return hr;}