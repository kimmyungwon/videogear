// vgRHTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../vgRegistryHook/vgRegistryHook.h"

typedef HRESULT (STDAPICALLTYPE *DllRegisterServerProc)(void);

int wmain(int argc, wchar_t* argv[])
{
	vgRegistryHook::Instance().Enable();
	HMODULE handle = LoadLibraryW(L"MPCVideoDec.ax");
	DllRegisterServerProc dllRegSvr = (DllRegisterServerProc)GetProcAddress(handle, "DllRegisterServer");
	dllRegSvr();
	FreeLibrary(handle);
	vgRegistryHook::Instance().Disable();
	return 0;
}

