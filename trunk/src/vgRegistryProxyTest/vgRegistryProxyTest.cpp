// vgRegistryProxyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vgRegistryProxy/vgRegistryImpl.h"

int wmain(int argc, wchar_t* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HKEY key1;
	vgRegistryImpl::GetInstance().RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear", 0, NULL, 0, KEY_ALL_ACCESS, NULL, 
		&key1, NULL);
	vgRegistryImpl::GetInstance().RegCloseKey(key1);

	vgRegistryImpl::GetInstance().Print();
	system("PAUSE");
	return 0;
}

