// vgRHTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../vgRegistryHook/vgRegistryHook.h"

typedef HRESULT (STDAPICALLTYPE *DllRegisterServerProc)(void);

int wmain(int argc, wchar_t* argv[])
{
	vgRHStartup();
	HMODULE handle = LoadLibraryW(L"MPCVideoDec.ax");
	DllRegisterServerProc dllRegSvr = (DllRegisterServerProc)GetProcAddress(handle, "DllRegisterServer");
	dllRegSvr();
	FreeLibrary(handle);
	vgRHShutdown();
	return 0;
}

