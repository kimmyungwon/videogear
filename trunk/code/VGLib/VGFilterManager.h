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

struct CVGFilter
{
	CLSID clsID;
	CStringW strName;
	DWORD dwMerit;
	LPFNNewCOMObject lpfnNew;

	CVGFilter(void): clsID(GUID_NULL), strName(L""), dwMerit(0), lpfnNew(NULL)	{}
	
	CVGFilter(REFCLSID _clsID, LPCWSTR _Name, DWORD _Merit, LPFNNewCOMObject _lpfnNew = NULL)
		:clsID(_clsID), strName(_Name), dwMerit(_Merit), lpfnNew(_lpfnNew)	{}

	CVGFilter(const CFactoryTemplate* _Templ)
	{
		CVGFilter(*_Templ->m_ClsID, _Templ->m_Name, _Templ->m_pAMovieSetup_Filter->dwMerit, _Templ->m_lpfnNew);
	}

	HRESULT CreateInstance(LPUNKNOWN pUnkOuter, IBaseFilter** ppBF) const
	{
		CheckPointer(ppBF, E_POINTER);

		HRESULT hr;
		if (lpfnNew != NULL)
		{
			*ppBF = (CBaseFilter*)lpfnNew(pUnkOuter, &hr);
			(*ppBF)->AddRef();
			return hr;
		}
		else
		{
			FAILED_RETURN(CoCreateInstance(clsID, pUnkOuter, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)ppBF));
			(*ppBF)->AddRef();
			return S_OK;
		}
	}

	CVGFilter& operator=(const CVGFilter& r)
	{
		clsID = r.clsID;
		strName = r.strName;
		dwMerit = r.dwMerit;
		lpfnNew = r.lpfnNew;
		return *this;
	}
	
	friend bool operator==(const CVGFilter& a, const CVGFilter& b)
	{
		return InlineIsEqualGUID(a.clsID, b.clsID) == TRUE;
	}
	
	friend bool operator>(const CVGFilter& a, const CVGFilter& b)
	{
		if (a.lpfnNew != NULL && b.lpfnNew == NULL)
			return true;
		else if (a.lpfnNew == NULL && b.lpfnNew != NULL)
			return false;
		else
			return a.dwMerit > b.dwMerit;
	}
};

typedef set<CVGFilter, greater<CVGFilter> > CVGFilters;

class CEnumFilter : public CVGUnknownImpl<IEnumGUID, IID_IEnumGUID>
{
private:
	CVGFilters					m_items;
	CVGFilters::const_iterator	m_iter;
public:
	CEnumFilter(void);
	void Add(const CVGFilter& flt);
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
	typedef multimap<GUID, CVGFilter, guid_less> guid2ft_t;
	typedef pair<guid2ft_t::const_iterator, guid2ft_t::const_iterator> guid2ft_itpair_t;
	typedef map<GUID, guid2ft_t, guid_less> maj2subs_t;
	typedef map<GUID, CVGFilter, guid_less> filter_lookup_t;
private:		
	CComPtr<IGraphBuilder>		m_pGB;
	CComPtr<IFilterMapper2>		m_pFM2;
	maj2subs_t					m_lookupMT;		// 输入Pin的MediaType到滤镜的映射
	filter_lookup_t				m_lookupFlt;	// CLSID到滤镜的映射
protected:
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
	HRESULT EnumMatchingFilters(CVGFilters &ret, BOOL bExactMatch, DWORD dwMerit, 
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
	virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters(IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, 
														  CLSID clsInMaj, CLSID clsInSub);
	virtual HRESULT STDMETHODCALLTYPE Initialize(void);
	virtual HRESULT STDMETHODCALLTYPE RenderFile(LPCWSTR lpszFileName);
	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID refiid, void **ppv);
};
