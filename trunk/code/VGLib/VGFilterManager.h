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

template<typename CompT>
class CEnumFilter : public CVGUnknownImpl<IEnumGUID, IID_IEnumGUID>
{
private:
	CVGFiltersT<CompT>							m_items;
	typename CVGFiltersT<CompT>::const_iterator	m_iter;
public:
	CEnumFilter(void)
	{
		m_iter = m_items.end();
	}

	void Add(const CVGFilter& flt)
	{
		m_items.insert(flt);
		Reset();	
	}

	size_t GetCount(void)
	{
		return m_items.size();
	}

	/* IEnumGUID */
	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, GUID *rgelt, ULONG *pceltFetched)
	{
		CheckPointer(rgelt, E_POINTER);
		ValidateReadWritePtr(rgelt, celt * sizeof(GUID));
		if (m_iter == m_items.end())
			return E_FAIL;

		ULONG nDone = 0;
		while (m_iter != m_items.end() && nDone < celt)
		{
			rgelt[nDone] = m_iter->clsID;
			m_iter++;
			nDone++;
		}	
		return nDone == celt ? S_OK : S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt)
	{
		if (m_iter == m_items.end())
			return E_FAIL;

		ULONG nDone = 0;
		while (m_iter != m_items.end() && nDone < celt)
		{
			m_iter++;
			nDone++;
		}
		return nDone == celt ? S_OK : S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE Reset(void)
	{
		m_iter = m_items.begin();
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumGUID **ppenum)
	{
		CEnumFilter *pEnum = new CEnumFilter;
		pEnum->m_items = m_items;
		pEnum->m_iter = m_iter;
		pEnum->AddRef();
		(*ppenum) = pEnum;
		return S_OK;
	}
};

class CVGFilterManager : public CVGUnknownImpl<IVGFilterManager, IID_IVGFilterManager>
{
private:
	typedef multimap<GUID, CVGFilter, guid_less> guid2ft_t;
	typedef pair<guid2ft_t::const_iterator, guid2ft_t::const_iterator> guid2ft_itpair_t;
	typedef map<GUID, guid2ft_t, guid_less> maj2subs_t;
	typedef map<GUID, CVGFilter, guid_less> filter_lookup_t;
private:		
	CComPtr<IGraphBuilder>		m_pGB;
	CComPtr<IFilterMapper2>		m_pFM2;
	maj2subs_t					m_lookupMT;		// 输入Pin的MediaType到滤镜的映射
	filter_lookup_t				m_lookupFlt;	// CLSID到滤镜的映射
	bool						m_bInternalFirst;	// 决定内部滤镜是否优先
protected:
	void AddFilterToMediaTypeLookup(REFCLSID clsMajor, REFCLSID clsMinor, const CVGFilter& flt);
	void RegisterSystemFilters(HKEY hkeyRoot, LPCWSTR lpszSubPath);
	HRESULT RegisterFilter(const CFactoryTemplate& templ);
	HRESULT RegisterFilter(REFCLSID clsID, LPCWSTR lpszName, DWORD dwMerit, ULONG nPins, const REGFILTERPINS* lpPins,
		LPFNNewCOMObject lpfnNew = NULL);
	HRESULT RegisterFilter(REFCLSID clsID, LPCWSTR lpszName, DWORD dwMerit, ULONG nPins2, const REGFILTERPINS2* lpPins2,
		LPFNNewCOMObject lpfnNew = NULL);
	HRESULT RegisterFilter(REFCLSID clsID, LPCWSTR lpszName, char* pBuf, DWORD nSize);
	/* 渲染 */
	HRESULT ConnectDirect(IPin* pPinOut, IBaseFilter* pBFIn, const CMediaType& mt);
	HRESULT ConnectDirect(IPin* pPinOut, const CVGFilter filterIn, const CMediaType& mt, IBaseFilter** pBF);
	template<typename CompT>
	HRESULT EnumMatchingFilters(CVGFiltersT<CompT> &ret, BOOL bExactMatch, DWORD dwMerit, 
		CLSID clsInMaj, CLSID clsInSub) const;
	HRESULT RenderFilter(IBaseFilter* pBF);
	HRESULT RenderPin(IPin* pPin);
	/* 信息 */
	PIN_DIRECTION GetPinDir(IPin* pPin);
	bool IsPinConnected(IPin* pPin);
public:
	CVGFilterManager(void);
	virtual ~CVGFilterManager(void);
	/* IVGFilterManager */
	virtual HRESULT STDMETHODCALLTYPE ClearGraph(void);
	virtual HRESULT STDMETHODCALLTYPE Initialize(void);
	virtual HRESULT STDMETHODCALLTYPE RenderFile(LPCWSTR lpszFileName);
	virtual void STDMETHODCALLTYPE SetInternalFirst(BOOL bInternalFirst);
	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID refiid, void **ppv);
};