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
protected:
	HRESULT RegisterFilter(UINT uiFilterCount, const FilterSetupInfo* pSetupInfo);
private:
	boost::ptr_vector<CFilter> m_filters;
};
