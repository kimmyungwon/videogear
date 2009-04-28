#include "stdafx.h"
#include "FilterManager.h"
#include "..\..\filters\parser\matroskasplitter\MatroskaSplitter.h"

#pragma comment(lib, "MatroskaSplitterDU.lib")

namespace MK
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{	
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_Matroska},
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL}
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const AMOVIESETUP_FILTER sudFilter[] =
	{
		{&__uuidof(CMatroskaSplitterFilter), L"MPC - Matroska Splitter", MERIT_NORMAL, countof(sudpPins), sudpPins},
		{&__uuidof(CMatroskaSourceFilter), L"MPC - Matroska Source", MERIT_NORMAL, 0, NULL},
	};
};

void RegisterMK(void)
{
	using namespace MK;

	CFilterManager::RegisterFilter(sudFilter[0].strName, sudFilter[0].clsID, CreateInstance<CMatroskaSplitterFilter>, NULL, &sudFilter[0]);
	CFilterManager::RegisterSourceFilter(sudFilter[1].strName, sudFilter[1].clsID, CreateInstance<CMatroskaSourceFilter>, NULL,
		_T("0,4,,1A45DFA3"), _T(".mkv"), _T(".mka"), _T(".mks"), NULL);
}