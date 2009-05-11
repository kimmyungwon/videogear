
// VideoGear.h : VideoGear 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

// CVideoGearApp:
// 有关此类的实现，请参阅 VideoGear.cpp
//

class CVideoGearApp : public CWinApp
{
public:
	CVideoGearApp();
	~CVideoGearApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVideoGearApp theApp;

#define SAFE_DELETE(ptr)	{if((ptr) != NULL) {delete (ptr); (ptr) = NULL;}}
#define RIF(cond)	{HRESULT hr = (cond); if (FAILED(hr)) return hr;}