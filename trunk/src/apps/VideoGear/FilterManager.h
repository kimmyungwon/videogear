#pragma once

#include "..\..\dsutil\DSUtil.h"
#include "..\..\dSutil\MediaTypes.h"
#include "Filter.h"
#include "Util.h"

#define SUPPORT_AVI			1
#define SUPPORT_MK			1
#define SUPPORT_RM			1
#define SUPPORT_VIDEODEC	1
#define SUPPORT_AUDIODEC	1

struct MediaType
{
	GUID major;
	GUID minor;

	MediaType(const GUID& ma, const GUID& mi)
	{
		major = ma;
		minor = mi;
	}

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
	HRESULT EnumMatchingFilters(bool exactMatch, const GUID* inputTypes, size_t inputTypeCount, std::list<CFilter*>& filters);
	HRESULT EnumMatchingSource(LPCTSTR fileName, CSourceFilter*& filter);
protected:
	CFilter* RegisterFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
	CSourceFilter* RegisterSourceFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		LPCTSTR* chkbytes, size_t chkbytesCount, LPCTSTR* ext, size_t extCount);
	void RegisterAVI(void);
	void RegisterMK(void);
	void RegisterRM(void);
	void RegisterVideoDec(void);
	void RegisterAudioDec(void);
private:
	typedef boost::ptr_map<CLSID, CFilter> filters_t;
	typedef boost::ptr_map<CLSID, CSourceFilter> sources_t;
	typedef std::multimap<MediaType, CFilter*> input_types_t;  

	filters_t		m_filters;
	sources_t		m_sources;
	input_types_t	m_inputTypes;
};

extern CFilterManager g_filterMan;
