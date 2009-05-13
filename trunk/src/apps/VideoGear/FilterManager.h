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
	CAtlList<MediaType> mts;
};

struct RegisterFilterSetupInfo
{
	CLSID clsID;
	CStringW strName;
	DWORD dwMerit;
	DWORD dwInPins, dwOutPins;
	CAtlList<RegisterPinSetupInfo*> pins;

	~RegisterFilterSetupInfo(void)
	{
		POSITION pos = pins.GetHeadPosition();
		while (pins.GetCount() > 0)
		{
			RegisterPinSetupInfo *pPinInfo = pins.RemoveHead();
			SAFE_DELETE(pPinInfo);
		}
	}
};

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
	HRESULT EnumMatchingFilters(const CAtlList<CMediaType>& mts, CAtlList<CFilter*>& filters);
	HRESULT AddAudioSwitcherToGraph(IFilterGraph *pGraph, IBaseFilter **ppvObj);
protected:
	HRESULT RegisterInternalFilter(UINT nFilterCount, const InternalFilterSetupInfo* pSetupInfo, bool bFilterOnly = false);
	HRESULT RegisterSystemFilter(const RegisterFilterSetupInfo& setupInfo, bool bFilterOnly = false);
	HRESULT RegisterSystemFilters(void);
	HRESULT DecodeFilterData(BYTE* pData, DWORD cbData, RegisterFilterSetupInfo &info);
private:
	typedef CAtlMap<CLSID, CAutoPtr<CFilter> > FilterList;
	typedef CRBMultiMap<GUID, CFilter*> MinorTypes;
	typedef CAtlMap<GUID, MinorTypes> MajorTypes;

	FilterList m_InternalFilters, m_SystemFilters;
	MajorTypes m_InternalMajorTypes, m_SystemMajorTypes;
 };

extern CFilterManager g_FilterMgr;