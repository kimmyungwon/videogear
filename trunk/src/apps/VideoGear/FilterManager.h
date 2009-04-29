#pragma once

#include "..\..\dsutil\DSUtil.h"
#include "..\..\dSutil\MediaTypes.h"
#include "Filter.h"
#include "Util.h"

#define SUPPORT_AVI	1
#define SUPPORT_MK	1
#define SUPPORT_RM	1

struct MediaType
{
	GUID major;
	GUID minor;

	MediaType(const REGPINTYPES& rpt)
	{
		major = *rpt.clsMajorType;
		minor = *rpt.clsMinorType;
	}

	MediaType(const AM_MEDIA_TYPE& mt)
	{
		major = mt.majortype;
		minor = mt.subtype;
	}

	bool operator<(const MediaType& rhs) const
	{
		int ret = memcmp(&major, &rhs.major, sizeof(GUID));
		if (ret == 0)
			ret = memcmp(&minor, &rhs.minor, sizeof(GUID));
		return ret < 0;
	}
};

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	void Initialize(void);
	UINT EnumMatchingFilters(bool exactMatch, AM_MEDIA_TYPE* inputType, std::list<CFilter*>& filters);
protected:
	CFilter* RegisterFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
	CSourceFilter* RegisterSourceFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		LPCTSTR* chkbytes, size_t chkbytesCount, LPCTSTR* ext, size_t extCount);
	void RegisterAVI(void);
	void RegisterMK(void);
	void RegisterRM(void);
private:
	typedef boost::ptr_map<CLSID, CFilter> filters_t;
	typedef std::multimap<MediaType, CFilter*> input_types_t;  

	filters_t		m_filters;
	input_types_t	m_inputTypes;
};

extern CFilterManager g_filterMan;
