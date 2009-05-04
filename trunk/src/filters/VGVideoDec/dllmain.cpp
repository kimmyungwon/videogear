// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE hInstance, ULONG ulReason, __inout_opt LPVOID pv);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	DllEntryPoint(hModule, ul_reason_for_call, NULL);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

