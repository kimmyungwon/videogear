#include "stdafx.h"
#include "FilterManager.h"
#include "..\..\filters\transform\mpcvideodec\MPCVideoDecFilter.h"

#pragma comment(lib, "MPCVideoDecDU.lib")

namespace VideoDec
{
	const AMOVIESETUP_PIN sudpPinsVideoDec[] =
	{
		{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesInCount,  CMPCVideoDecFilter::sudPinTypesIn},
		{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, CMPCVideoDecFilter::sudPinTypesOutCount, CMPCVideoDecFilter::sudPinTypesOut}
	};

	const AMOVIESETUP_FILTER sudFilters[] =
	{
		{&__uuidof(CMPCVideoDecFilter), L"MPC - Video decoder", /*MERIT_DO_NOT_USE*/0x40000001, countof(sudpPinsVideoDec), sudpPinsVideoDec}
	};
}

void CFilterManager::RegisterVideoDec(void)
{
	using namespace VideoDec;
	
	RegisterFilter(sudFilters[0].strName, &__uuidof(CMPCVideoDecFilter), CreateInstance<CMPCVideoDecFilter>, NULL, &sudFilters[0]);
}