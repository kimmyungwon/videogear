#pragma once

#include "vgInclude.h"

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

EXTERN_C
NTSTATUS
NTAPI
ZwQueryKey(
    __in HANDLE KeyHandle,
    __in KEY_INFORMATION_CLASS KeyInformationClass,
    __out_bcount_opt(Length) PVOID KeyInformation,
    __in ULONG Length,
    __out PULONG ResultLength
    );