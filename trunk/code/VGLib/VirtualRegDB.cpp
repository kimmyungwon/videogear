#include "StdAfx.h"
#include "VirtualRegDB.h"
#include "../Common/AriaDebug.h"

//static CVirtualRegDB gVrRegDB;

#define FAKE_HKEY 0xC0000000

static LONG (WINAPI *g_TrueRegCloseKey)( __in HKEY hKey ) = RegCloseKey;
static LONG (WINAPI *g_TrueRegOpenKeyA)( __in HKEY hKey, __in LPCSTR lpSubKey, __out PHKEY phkResult ) = RegOpenKeyA;
static LONG (WINAPI *g_TrueRegOpenKeyW)( __in HKEY hKey, __in LPCWSTR lpSubKey, __out PHKEY phkResult ) = RegOpenKeyW;

LONG WINAPI MyRegCloseKey( __in HKEY hKey )
{
	TraceA("RegOpenKey(%.8X)", hKey);
	return g_TrueRegCloseKey(hKey);
}

LONG WINAPI MyRegOpenKeyA( __in HKEY hKey, __in LPCSTR lpSubKey, __out PHKEY phkResult )
{
	TraceA("RegOpenKeyA(%.8X, %s)", hKey, lpSubKey);
	return g_TrueRegOpenKeyA(hKey, lpSubKey, phkResult);
}

LONG WINAPI MyRegOpenKeyW( __in HKEY hKey, __in LPCWSTR lpSubKey, __out PHKEY phkResult )
{
	TraceW(L"RegOpenKeyW(%.8X, %s)", hKey, lpSubKey);
	return g_TrueRegOpenKeyW(hKey, lpSubKey, phkResult);
}

CVirtualRegDB::CVirtualRegDB(void)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&g_TrueRegCloseKey, MyRegCloseKey);
	DetourAttach((PVOID*)&g_TrueRegOpenKeyA, MyRegOpenKeyA);
	DetourAttach((PVOID*)&g_TrueRegOpenKeyW, MyRegOpenKeyW);
	DetourTransactionCommit();
}

CVirtualRegDB::~CVirtualRegDB(void)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach((PVOID*)&g_TrueRegCloseKey, MyRegCloseKey);
	DetourDetach((PVOID*)&g_TrueRegOpenKeyA, MyRegOpenKeyA);
	DetourDetach((PVOID*)&g_TrueRegOpenKeyW, MyRegOpenKeyW);
	DetourTransactionCommit();
}
