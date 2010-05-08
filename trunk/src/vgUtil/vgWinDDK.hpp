#pragma once

#include "vgInclude.hpp"

VG_NAMESPACE_BEGIN

typedef __success(return >= 0) LONG NTSTATUS;
/*lint -save -e624 */  // Don't complain about different typedefs.
typedef NTSTATUS *PNTSTATUS;
/*lint -restore */  // Resume checking for different typedefs.

#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth

#include <PshPack1.h>

typedef struct _KEY_BASIC_INFORMATION {
	LARGE_INTEGER LastWriteTime;
	ULONG   TitleIndex;
	ULONG   NameLength;
	WCHAR   Name[1];            // Variable length string
} KEY_BASIC_INFORMATION, *PKEY_BASIC_INFORMATION;

typedef struct _KEY_NODE_INFORMATION {
	LARGE_INTEGER LastWriteTime;
	ULONG   TitleIndex;
	ULONG   ClassOffset;
	ULONG   ClassLength;
	ULONG   NameLength;
	WCHAR   Name[1];            // Variable length string
	//          Class[1];           // Variable length string not declared
} KEY_NODE_INFORMATION, *PKEY_NODE_INFORMATION;

typedef struct _KEY_FULL_INFORMATION {
	LARGE_INTEGER LastWriteTime;
	ULONG   TitleIndex;
	ULONG   ClassOffset;
	ULONG   ClassLength;
	ULONG   SubKeys;
	ULONG   MaxNameLen;
	ULONG   MaxClassLen;
	ULONG   Values;
	ULONG   MaxValueNameLen;
	ULONG   MaxValueDataLen;
	WCHAR   Class[1];           // Variable length
} KEY_FULL_INFORMATION, *PKEY_FULL_INFORMATION;

#include <PopPack.h>

typedef enum _KEY_INFORMATION_CLASS {
	KeyBasicInformation,
	KeyNodeInformation,
	KeyFullInformation,
	KeyNameInformation,
	KeyCachedInformation,
	KeyFlagsInformation,
	KeyVirtualizationInformation,
	KeyHandleTagsInformation,
	MaxKeyInfoClass  // MaxKeyInfoClass should always be the last enum
} KEY_INFORMATION_CLASS;

class WinDDK : public noncopyable
{
public:
	NTSTATUS (NTAPI *ZwQueryKey)(__in HANDLE KeyHandle, __in KEY_INFORMATION_CLASS KeyInformationClass, 
		__out_bcount_opt(Length) PVOID KeyInformation, __in ULONG Length, __out PULONG ResultLength);

	static WinDDK& GetInstance(void)
	{
		static WinDDK instance;
		return instance;
	}

	~WinDDK(void)
	{
		if (m_dllHandle != NULL)
		{
			FreeLibrary(m_dllHandle);
			m_dllHandle = NULL;
		}
	}
private:
	WinDDK(void)
	{
		m_dllHandle = LoadLibraryW(L"ntdll.dll");
		if (m_dllHandle == NULL)
			return;
		ZwQueryKey = (BOOST_TYPEOF(ZwQueryKey))GetProcAddress(m_dllHandle, "ZwQueryKey");
	}
private:
	HMODULE m_dllHandle;
};

VG_NAMESPACE_END