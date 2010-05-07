// vgRegistryProxyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "vgRegistryProxy/vgRegistryImpl.h"

int wmain(int argc, wchar_t* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	vgRegistryImpl &impl = vgRegistryImpl::GetInstance();
	impl.AddKey(vgRegistryPath(HKEY_LOCAL_MACHINE, L"Software\\iLuE"), true);
	impl.AddKey(vgRegistryPath(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear"), true);

	HKEY key1;
	impl.RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft", 0, KEY_ALL_ACCESS, &key1);

	HKEY key2;
	impl.RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear\\Settings", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key2, NULL);

	impl.Print();
	system("PAUSE");
	return 0;
}

