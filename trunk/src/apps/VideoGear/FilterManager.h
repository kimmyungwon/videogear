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

struct TCheckBytes
{
	CLSID clsID;
	int64_t offset;
	uint32_t length;
	uint32_t mask;
	std::string bytes;
};

typedef boost::ptr_map<CLSID, CFilter> TRegisteredFilters;
typedef std::list<TCheckBytes> TRegisteredCheckBytes;
typedef std::multimap<CString, CLSID> TRegisteredExtensions;
typedef std::multimap<TMediaType, CLSID> TRegisteredInputMediaTypes;

class CFilterManager
{
public:
	static TRegisteredFilters ms_regFilters;
	static TRegisteredCheckBytes ms_regChkBytes;
	static TRegisteredExtensions ms_regExts;
	static TRegisteredInputMediaTypes ms_regInputs;
	
	static void RegisterFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const AMOVIESETUP_FILTER* pAMovieSetup_Filter);
	static void RegisterSourceFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		CString chkbytes, LPCTSTR ext = NULL, ...);
	static void RegisterSourceFilter(LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
		const std::vector<CString>& chkbytes, LPCTSTR ext = NULL, ...);
	static void RegisterInternalFilters(void);
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	void Clear(void);
	HRESULT AddSourceFilter(LPCTSTR fileName, IBaseFilter*& filter);
	HRESULT Render(IBaseFilter* filter);
protected:
	bool CheckBytes(CFile& file, const TCheckBytes& chkbytes);
private:
	CComPtr<IFilterGraph2>	m_graph;
};
