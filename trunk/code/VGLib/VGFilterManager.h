#pragma once

#include "VGIntf.h"
#include "VGBaseObject.h"

using namespace std;

struct guid_less : public binary_function <GUID, GUID, bool> 
{
	bool operator()(REFGUID _Left, REFGUID _Right) const
	{
		return memcmp(&_Left, &_Right, sizeof(GUID)) < 0;
	}
};

class CEnumFilter : public CVGUnknownImpl<IEnumGUID, IID_IEnumGUID>
{
private:
	// 用于将滤镜按Merit降序排列
	struct merit_greater : public binary_function <const AMOVIESETUP_FILTER*, const AMOVIESETUP_FILTER*, bool> 	
	{
		bool operator()(const AMOVIESETUP_FILTER* _Left, const AMOVIESETUP_FILTER* _Right) const
		{
			return _Left->dwMerit > _Right->dwMerit;
		}
	};

	set<const AMOVIESETUP_FILTER*, merit_greater>					m_items;
	set<const AMOVIESETUP_FILTER*, merit_greater>::const_iterator	m_iter;
public:
	CEnumFilter(void);
	void Add(const AMOVIESETUP_FILTER* pFilter);
	size_t GetCount(void);
	/* IEnumGUID */
	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, GUID *rgelt, ULONG *pceltFetched);
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
	virtual HRESULT STDMETHODCALLTYPE Reset(void);
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumGUID **ppenum);
};

class CVGFilterManager : public CVGUnknownImpl<IVGFilterManager, IID_IVGFilterManager>
{
private:
	typedef multimap<GUID, const CFactoryTemplate*, guid_less> guid2ft_t;
	typedef pair<guid2ft_t::const_iterator, guid2ft_t::const_iterator> guid2ft_itpair_t;
	typedef map<GUID, guid2ft_t, guid_less> maj2subs_t;
	typedef map<GUID, const CFactoryTemplate*, guid_less> filter_lookup_t;
private:		
	CComPtr<IGraphBuilder>		m_pGB;
	CComPtr<IFilterMapper2>		m_pFM2;
	maj2subs_t					m_lookupMT;		// 输入Pin的MediaType到滤镜的映射
	filter_lookup_t				m_lookupFlt;	// CLSID到滤镜的映射
public:
	CVGFilterManager(void);
	virtual ~CVGFilterManager(void);
	/* IVGFilterManager */
	virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters(IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, 
														  CLSID clsInMaj, CLSID clsInSub);
	virtual HRESULT STDMETHODCALLTYPE Initialize(void);
};
