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
	HRESULT EnumMatchingFilters(const CAtlList<CMediaType>& mts, CAtlList<CFilter*>& filters);
protected:
	HRESULT RegisterFilter(UINT uiFilterCount, const FilterSetupInfo* pSetupInfo);
private:
	typedef CAtlMap<CLSID, CAutoPtr<CFilter> > FilterList;
	typedef CRBMultiMap<GUID, CFilter*> MinorTypes;
	typedef CAtlMap<GUID, MinorTypes> MajorTypes;

	FilterList m_filters;
	MajorTypes m_majorTypes;
};