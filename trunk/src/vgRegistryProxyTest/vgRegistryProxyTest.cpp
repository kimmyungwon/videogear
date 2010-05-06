// vgRegistryProxyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../vgRegistryProxy/vgRegistryProxy.h"

int wmain(int argc, wchar_t* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	vgRegistryProxy::GetInstance().Enable();

	HKEY key1;
	RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key1, NULL);
	RegCloseKey(key1);
// 	vgRegistryProxy::GetInstance().CreateNode();
// 	vgRegistryProxy::GetInstance().OpenNode(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear");
// 	vgRegistryProxy::GetInstance().CreateNode(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear");

	vgRegistryProxy::GetInstance().Disable();
	vgRegistryProxy::GetInstance().Print();
	system("PAUSE");
	return 0;
}

