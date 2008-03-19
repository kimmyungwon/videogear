#pragma once

#include "VGIntf.h"
#include "VGBaseObject.h"

using namespace std;

class CEnumGUID : public CVGUnknownImpl<IEnumGUID, IID_IEnumGUID>
{
private:
	vector<GUID>	m_items;
	size_t			m_nIter;
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
	typedef map<GUID, CFactoryTemplate*> guid2ft_t;
	typedef map<GUID, guid2ft_t> maj2subs_t;
	typedef map<GUID, CFactoryTemplate*> filter_lookup_t;
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
