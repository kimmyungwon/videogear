#pragma once

#include "Filter.h"

struct FilterSetupInfo
{
	const CLSID* pClsID;
	LPCWSTR pszName;
	PFNCreateInstance pfnCreateInstance;
	UINT nPinCount;
	const AMOVIESETUP_PIN* pPins;
};

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	HRESULT EnumMatchingFilters(const CAtlList<CMediaType>& mts, CAtlList<CFilter*>& filters);
	HRESULT AddAudioSwitcherToGraph(IFilterGraph *pGraph, IBaseFilter **ppvObj);
protected:
	HRESULT RegisterFilter(UINT nFilterCount, const FilterSetupInfo* pSetupInfo, bool bFilterOnly = false);
private:
	typedef CAtlMap<CLSID, CAutoPtr<CFilter> > FilterList;
	typedef CRBMultiMap<GUID, CFilter*> MinorTypes;
	typedef CAtlMap<GUID, MinorTypes> MajorTypes;

	FilterList m_filters;
	MajorTypes m_majorTypes;
 };

extern CFilterManager g_FilterMgr;