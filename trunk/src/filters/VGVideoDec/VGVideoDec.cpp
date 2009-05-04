// VGVideoDec.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "VGVideoDecFilter.h"

#ifdef REGISTER_FILTER

const AMOVIESETUP_PIN sudpPins[] = 
{
	{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CVGVideoDecFilter::ms_nPinTypesInCount, CVGVideoDecFilter::ms_PinTypesIn},
	{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CVGVideoDecFilter::ms_nPinTypesOutCount, CVGVideoDecFilter::ms_PinTypesOut}
};

const AMOVIESETUP_FILTER sudFilters[] =
{
	{&CLSID_VGVideoDecFilter, L"VideoGear - Video Decoder", MERIT_NORMAL, _countof(sudpPins), sudpPins}
};


CFactoryTemplate g_Templates[] =
{
	{sudFilters[0].strName, &CLSID_VGVideoDecFilter, CVGVideoDecFilter::CreateInstance, NULL, &sudFilters[0]}
};

int g_cTemplates = _countof(g_Templates); 

STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );
}
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );
}

#endif