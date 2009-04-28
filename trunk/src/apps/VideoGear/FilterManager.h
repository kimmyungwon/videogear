#pragma once

#include "..\..\dsutil\DSUtil.h"
#include "..\..\dSutil\MediaTypes.h"
#include <initguid.h>
#include <moreuuids.h>

class CFilterManager
{
public:
	static void RegisterFilter(const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
	static void RegisterSourceFilter(const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		LPCTSTR chkbytes, LPCTSTR ext = NULL, ...);
	static void RegisterInternalFilters(void);
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
};
