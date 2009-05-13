#pragma once

#include "Filter.h"

struct InternalFilterSetupInfo
{
	const CLSID* pClsID;
	LPCWSTR pszName;
	PFNCreateInstance pfnCreateInstance;
	UINT nPinCount;
	const AMOVIESETUP_PIN* pPins;
};

struct RegisterPinSetupInfo
{
	bool bOutput;
	std::vector<MediaType> mts;
};

struct RegisterFilterSetupInfo
{
	CLSID clsID;
	CStringW strName;
	DWORD dwMerit;
	DWORD dwInPins, dwOutPins;
	boost::ptr_vector<RegisterPinSetupInfo> pins;
};

struct MeritOrder
{
	bool operator()(CFilter *const _left, CFilter *const _right) const
	{
		if (_left->GetMerit() != _right->GetMerit())
			return (_left->GetMerit() > _right->GetMerit());
		else 
			return (_left < _right);
	}
};

typedef std::set<CFilter*, MeritOrder> MatchedFilters;

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	void EnumMatchingFilters(const CAtlList<CMediaType>& mts, MatchedFilters& filters);
	HRESULT AddAudioSwitcherToGraph(IFilterGraph *pGraph, IBaseFilter **ppvObj);
protected:
	HRESULT RegisterInternalFilter(UINT nFilterCount, const InternalFilterSetupInfo* pSetupInfo, bool bFilterOnly = false);
	HRESULT RegisterSystemFilter(const RegisterFilterSetupInfo& setupInfo, bool bFilterOnly = false);
	HRESULT RegisterSystemFilters(void);
	HRESULT DecodeFilterData(BYTE* pData, DWORD cbData, RegisterFilterSetupInfo &info);
private:
	typedef boost::ptr_map<CLSID, CFilter> FilterList;
	typedef std::multimap<GUID, CFilter*> MinorTypes;
	typedef std::map<GUID, MinorTypes> MajorTypes;

	FilterList m_InternalFilters, m_SystemFilters;
	MajorTypes m_InternalMajorTypes, m_SystemMajorTypes;
 };

extern CFilterManager g_FilterMgr;