#include "stdafx.h"

int GetSwsFlags(void)
{
	int CPUInfo[4];
	int nFlags = 0;
	
	__cpuid(CPUInfo, 1);
	if (CPUInfo[3] >> 23 & 1) nFlags |= SWS_CPU_CAPS_MMX;
	__cpuid(CPUInfo, 0x80000001);
	if (CPUInfo[3] >> 22 & 1) nFlags |= SWS_CPU_CAPS_MMX2;
	if (CPUInfo[3] >> 31 & 1) nFlags |= SWS_CPU_CAPS_3DNOW;
	return nFlags;
}