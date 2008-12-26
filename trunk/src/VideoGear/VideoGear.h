
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


// 重写
public:
	virtual BOOL InitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVideoGearApp theApp;

//////////////////////////////////////////////////////////////////////////

#define JIF(ret)	{ if(FAILED(hr=(ret))) return hr; }

#define BeginEnumFilters(pFilterGraph, pEnumFilters, pBaseFilter) \
{CComPtr<IEnumFilters> pEnumFilters; \
	if(pFilterGraph && SUCCEEDED(pFilterGraph->EnumFilters(&pEnumFilters))) \
{ \
	for(CComPtr<IBaseFilter> pBaseFilter; S_OK == pEnumFilters->Next(1, &pBaseFilter, 0); pBaseFilter = NULL) \
{ \

#define EndEnumFilters }}}

#define BeginEnumCachedFilters(pGraphConfig, pEnumFilters, pBaseFilter) \
{CComPtr<IEnumFilters> pEnumFilters; \
	if(pGraphConfig && SUCCEEDED(pGraphConfig->EnumCacheFilter(&pEnumFilters))) \
{ \
	for(CComPtr<IBaseFilter> pBaseFilter; S_OK == pEnumFilters->Next(1, &pBaseFilter, 0); pBaseFilter = NULL) \
{ \

#define EndEnumCachedFilters }}}

#define BeginEnumPins(pBaseFilter, pEnumPins, pPin) \
{CComPtr<IEnumPins> pEnumPins; \
	if(pBaseFilter && SUCCEEDED(pBaseFilter->EnumPins(&pEnumPins))) \
{ \
	for(CComPtr<IPin> pPin; S_OK == pEnumPins->Next(1, &pPin, 0); pPin = NULL) \
{ \

#define EndEnumPins }}}

#define BeginEnumMediaTypes(pPin, pEnumMediaTypes, pMediaType) \
{CComPtr<IEnumMediaTypes> pEnumMediaTypes; \
	if(pPin && SUCCEEDED(pPin->EnumMediaTypes(&pEnumMediaTypes))) \
{ \
	AM_MEDIA_TYPE* pMediaType = NULL; \
	for(; S_OK == pEnumMediaTypes->Next(1, &pMediaType, NULL); DeleteMediaType(pMediaType), pMediaType = NULL) \
{ \

#define EndEnumMediaTypes(pMediaType) } if(pMediaType) DeleteMediaType(pMediaType); }}
