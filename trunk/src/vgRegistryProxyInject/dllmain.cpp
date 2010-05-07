// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "vgCommon/vgHook.h"
#include "vgRegistryProxy/vgRegistryImpl.h"

#define HOOK(name)		vgRegistryImpl::Real_##name = codeHook.Hook(vgRegistryImpl::Real_##name, vgRegistryImpl::Mine_##name)
#define UNHOOK(name)	codeHook.Unhook(vgRegistryImpl::Real_##name, vgRegistryImpl::Mine_##name)

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	static vgHook codeHook;
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		HOOK(RegCloseKey);
		HOOK(RegCreateKeyExW);
		HOOK(RegOpenKeyExW);
		break;
	case DLL_PROCESS_DETACH:
		UNHOOK(RegCloseKey);
		UNHOOK(RegCreateKeyExW);
		UNHOOK(RegOpenKeyExW);
		vgRegistryImpl::GetInstance().Print();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

