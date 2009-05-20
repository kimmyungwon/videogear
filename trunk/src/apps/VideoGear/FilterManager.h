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

struct MatchedFilter
{
	CFilter *pFilter;
	UINT nPriority;

	MatchedFilter(CFilter *_pFilter, UINT _nPriority)
		: pFilter(_pFilter), nPriority(_nPriority)
	{}
};

struct MeritOrder
{
	bool operator()(const MatchedFilter &_left, const MatchedFilter &_right) const
	{
		if (_left.pFilter == _right.pFilter)
			return false;
		else if (_left.nPriority != _right.nPriority)
			return (_left.nPriority < _right.nPriority);
		else if (_left.pFilter->GetMerit() != _right.pFilter->GetMerit())
			return (_left.pFilter->GetMerit() < _right.pFilter->GetMerit());
		else
			return _left.pFilter < _right.pFilter;
	}
};

typedef std::set<MatchedFilter, MeritOrder> MatchedFilters;

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	void Initialize(void);
	void EnumMatchingSources(CFile &file, MatchedFilters &filters);
	void EnumMatchingFilters(const CAtlList<CMediaType> &mts, MatchedFilters &filters);
	HRESULT AddAudioSwitcherToGraph(IFilterGraph *pGraph, IBaseFilter **ppvObj);
protected:
	HRESULT RegisterInternalFilter(UINT nFilterCount, const InternalFilterSetupInfo* pSetupInfo, bool bFilterOnly = false);
	HRESULT RegisterInternalSource(const CLSID &clsID, LPCWSTR pszSigns = NULL, LPCWSTR pszExts = NULL, ...);
	HRESULT RegisterSystemFilter(const RegisterFilterSetupInfo& setupInfo, bool bFilterOnly = false);
	HRESULT RegisterSystemFilters(const CLSID &category);
	HRESULT RegisterSystemSources(void);
	HRESULT DecodeFilterData(BYTE* pData, DWORD cbData, RegisterFilterSetupInfo &info);
private:
	typedef boost::ptr_map<CLSID, CFilter> FilterList;
	typedef std::multimap<GUID, CFilter*> FilterMinorTypes;
	typedef std::map<GUID, FilterMinorTypes> FilterMajorTypes;
	typedef std::multimap<CStringW, CFilter*> SourceExtensions;
	typedef std::multimap<MediaSignature, CFilter*> SourceCheckBytes;

	void EnumMatchingSources(SourceCheckBytes &chkBytesList, UINT nBasePriority, CFile &file, MatchedFilters &filters);
	void EnumMatchingFilters(FilterMajorTypes &majorTypes, UINT &nBasePriority, const CAtlList<CMediaType> &mts, MatchedFilters &filters);
private:
	FilterList m_InternalFilters, m_SystemFilters;
	FilterMajorTypes m_InternalMajorTypes, m_SystemMajorTypes;
	SourceExtensions m_InternalExtensions;
	SourceCheckBytes m_InternalCheckBytes, m_SystemCheckBytes;
 };

extern CFilterManager g_FilterMgr;