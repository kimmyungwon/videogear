#pragma once

#include "..\..\dsutil\DSUtil.h"
#include "..\..\dSutil\MediaTypes.h"
#include "Filter.h"
#include "Util.h"

#define SUPPORT_AVI	1
#define SUPPORT_MK	1
#define SUPPORT_RM	1

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	void Initialize(void);
	CFilter* RegisterFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
	CSourceFilter* RegisterSourceFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		LPCTSTR* chkbytes, size_t chkbytesCount, LPCTSTR* ext, size_t extCount);
protected:
	void RegisterAVI(void);
	void RegisterMK(void);
	void RegisterRM(void);
private:
	typedef boost::ptr_map<CLSID, CFilter> filters_t;

	filters_t	m_filters;
};

extern CFilterManager g_filterMan;
