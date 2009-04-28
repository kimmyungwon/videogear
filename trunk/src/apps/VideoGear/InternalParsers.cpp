#include "stdafx.h"
#include "RegisterFilter.h"
#include "..\..\filters\parser\realmediasplitter\RealMediaSplitter.h"

#pragma comment(lib, "RealMediaSplitterDU.lib")

namespace RM
{
	const AMOVIESETUP_MEDIATYPE sudPinTypesIn[] =
	{
		{&MEDIATYPE_Stream, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn), sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, 0, NULL}
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesIn2[] =
	{
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV20},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV30},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV40},
		{&MEDIATYPE_Video, &MEDIASUBTYPE_RV41},
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut2[] =
	{
		{&MEDIATYPE_Video, &MEDIASUBTYPE_NULL},
	};

	const AMOVIESETUP_PIN sudpPins2[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn2), sudPinTypesIn2},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesOut2), sudPinTypesOut2}
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesIn3[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_14_4},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_28_8},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_ATRC},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_COOK},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_DNET},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_SIPR},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_AAC},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_RAAC},
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_RACP},
	};

	const AMOVIESETUP_MEDIATYPE sudPinTypesOut3[] =
	{
		{&MEDIATYPE_Audio, &MEDIASUBTYPE_PCM},
	};

	const AMOVIESETUP_PIN sudpPins3[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesIn3), sudPinTypesIn3},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, countof(sudPinTypesOut3), sudPinTypesOut3}
	};

	const AMOVIESETUP_FILTER sudFilter[] =
	{
		{&__uuidof(CRealMediaSplitterFilter), L"MPC - RealMedia Splitter", MERIT_NORMAL, countof(sudpPins), sudpPins},
		{&__uuidof(CRealMediaSourceFilter), L"MPC - RealMedia Source", MERIT_NORMAL, 0, NULL},
		{&__uuidof(CRealVideoDecoder), L"MPC - RealVideo Decoder", MERIT_NORMAL, countof(sudpPins2), sudpPins2},
		{&__uuidof(CRealAudioDecoder), L"MPC - RealAudio Decoder", MERIT_NORMAL, countof(sudpPins3), sudpPins3},
	};
};

CFactoryTemplate g_Templates[] =
{
	{RM::sudFilter[0].strName, RM::sudFilter[0].clsID, CreateInstance<CRealMediaSplitterFilter>, NULL, &RM::sudFilter[0]},
	{RM::sudFilter[1].strName, RM::sudFilter[1].clsID, CreateInstance<CRealMediaSourceFilter>, NULL, &RM::sudFilter[1]},
	{RM::sudFilter[2].strName, RM::sudFilter[2].clsID, CreateInstance<CRealVideoDecoder>, NULL, &RM::sudFilter[2]},
	{RM::sudFilter[3].strName, RM::sudFilter[3].clsID, CreateInstance<CRealAudioDecoder>, NULL, &RM::sudFilter[3]},
};