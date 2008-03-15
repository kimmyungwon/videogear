// VGLib.h : VGLib DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "VGFilterList.h"
#include "../common/guliverkli/src/filters/FilterApp.h"

// CVGLibApp
// 有关此类实现的信息，请参阅 VGLib.cpp
//

class CVGLibApp : public CFilterApp
{
public:
	CVGLibApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

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
#pragma comment(lib, "RealMediaSplitterRU.lib")
#endif

//////////////////////////////////////////////////////////////////////////

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
											  const CLSID *pPinCategoryOut );
