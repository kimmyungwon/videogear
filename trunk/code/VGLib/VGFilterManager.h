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

class CEnumGUID : public CVGUnknownImpl<IEnumGUID, IID_IEnumGUID>
{
private:
	set<GUID, guid_less>					m_items;
	set<GUID, guid_less>::const_iterator	m_iter;
public:
	CEnumGUID(void);
	void Add(const GUID &guid);
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
	typedef map<GUID, CFactoryTemplate*, guid_less> guid2ft_t;
	typedef map<GUID, guid2ft_t, guid_less> maj2subs_t;
	typedef map<GUID, CFactoryTemplate*, guid_less> filter_lookup_t;
private:
	//mt_lookup_t			m_lookupMT;		// 输入Pin的MediaType到滤镜的映射
	maj2subs_t			m_lookupMT;
	filter_lookup_t		m_lookupFlt;	// CLSID到滤镜的映射
public:
	CVGFilterManager(void);
	virtual ~CVGFilterManager(void);
	/* IVGFilterManager */
	virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters(IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, CLSID clsInMaj, CLSID clsInSub);
};
