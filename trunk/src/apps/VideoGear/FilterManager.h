#pragma once

#include "..\..\dsutil\DSUtil.h"
#include "..\..\dSutil\MediaTypes.h"
#include "Filter.h"

struct TMediaType
{
	GUID majorType;
	GUID subType;

	TMediaType(const GUID& major, const GUID& sub)
		: majorType(major), subType(sub)
	{}

	bool operator<(const TMediaType& rhs) const
	{
		int ret = memcmp(&majorType, &rhs.majorType, sizeof(GUID));
		if (ret == 0)
			ret = memcmp(&subType, &rhs.subType, sizeof(GUID)); 
		return ret < 0;
	}
};	

typedef boost::ptr_map<GUID, CFilter> TRegisteredFilters;
typedef std::multimap<TMediaType, GUID> TRegisteredInputMediaTypes;

class CFilterManager
{
public:
	static TRegisteredFilters ms_regFilters;
	static TRegisteredInputMediaTypes ms_regInputs;
	
	static void RegisterFilter(const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
	static void RegisterSourceFilter(const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		LPCTSTR chkbytes, LPCTSTR ext = NULL, ...);
	static void RegisterInternalFilters(void);
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
};
