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
	/* IEnumGUID */
	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, GUID *rgelt, ULONG *pceltFetched);
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
	virtual HRESULT STDMETHODCALLTYPE Reset(void);
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumGUID **ppenum);
};

class CVGFilterManager : public CVGUnknownImpl<IVGFilterManager, IID_IVGFilterManager>
{
private:
	typedef multimap<CVGMediaType, CFactoryTemplate*, equal_to<CVGMediaType> > mt_lookup_t;
	typedef mt_lookup_t::const_iterator mt_lookup_iter_t;
	typedef map<CGUID, CFactoryTemplate*, equal_to<CGUID> > filter_lookup_t;
private:
	mt_lookup_t			m_lookupMT;		// 输入Pin的MediaType到滤镜的映射
	filter_lookup_t		m_lookupFlt;	// CLSID到滤镜的映射
public:
	CVGFilterManager(void);
	virtual ~CVGFilterManager(void);
	/* IVGFilterManager */
	virtual HRESULT EnumMatchingFilters(IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, CLSID clsInMaj, CLSID clsInSub);
};
