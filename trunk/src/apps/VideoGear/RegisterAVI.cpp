#include "stdafx.h"
#include "FilterManager.h"
#include "..\..\filters\parser\avisplitter\AviSplitter.h"

#pragma comment(lib, "AviSplitterDU.lib")

namespace AVI
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_Avi},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const AMOVIESETUP_FILTER sudFilter[] =
	{
		{&__uuidof(CAviSplitterFilter), L"MPC - Avi Splitter", MERIT_NORMAL+1, countof(sudpPins), sudpPins},
		{&__uuidof(CAviSourceFilter), L"MPC - Avi Source", MERIT_NORMAL+1, 0, NULL},
	};
};

void CFilterManager::RegisterAVI(void)
{
	using namespace AVI;
	
	RegisterFilter(sudFilter[0].strName, sudFilter[0].clsID, CreateInstance<CAviSplitterFilter>, NULL, &sudFilter[0]);
	RegisterSourceFilter(sudFilter[1].strName, sudFilter[1].clsID, CreateInstance<CAviSourceFilter>, NULL,
		ARRAYOF(LPCTSTR, _T("0,4,,52494646,8,4,,41564920"),	// 'RIFF' ... 'AVI '
						_T("0,4,,52494646,8,4,,41564958"),	// 'RIFF' ... 'AVIX'
						_T("0,4,,52494646,8,4,,414D5620")	// 'RIFF' ... 'AMV '
		), ARRAYOF(LPCTSTR, _T(".avi"), _T(".divx"), _T(".vp6"), _T(".amv")));
}