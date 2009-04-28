#pragma once

#include "..\..\dsutil\DSUtil.h"
#include "..\..\dSutil\MediaTypes.h"

#include <initguid.h>
#include <moreuuids.h>

void RegisterFilter(const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
					const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
void RegisterSourceFilter(const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
						  LPCTSTR chkbytes, LPCTSTR ext = NULL, ...);

#if	SUPPORT_RM
void RegisterRM(void);
#endif // SUPPORT_RM