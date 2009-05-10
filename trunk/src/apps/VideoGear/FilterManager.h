#pragma once

#include "Filter.h"

struct FilterSetupInfo
{
	const CLSID* pClsID;
	LPCWSTR pszName;
	PFNCreateInstance pfnCreateInstance;
	UINT uiPinCount;
	const AMOVIESETUP_PIN* pPins;
};

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	HRESULT EnumMatchingFilters(const CAtlList<CMediaType>& mts, std::list<CFilter*>& filters);
protected:
	HRESULT RegisterFilter(UINT uiFilterCount, const FilterSetupInfo* pSetupInfo);
private:
	typedef std::map<CLSID, CFilter*> FilterList;
	FilterList m_filters;
};