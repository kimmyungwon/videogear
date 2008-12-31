#pragma once

class CFakeFilterMapper2 : public CUnknown
						 , public IFilterMapper2
{
	DECLARE_IUNKNOWN
public:
	static IFilterMapper2* m_pFilterMapper;
public:
	static void Initialize(void);
	CFakeFilterMapper2(LPUNKNOWN pUnk);
	virtual ~CFakeFilterMapper2(void);
	HRESULT Register(LPCTSTR lpszFile);
protected:
	STDMETHODIMP CreateCategory(REFCLSID clsidCategory, DWORD dwCategoryMerit, LPCWSTR Description);
	STDMETHODIMP UnregisterFilter(const CLSID *pclsidCategory, LPCOLESTR szInstance, REFCLSID Filter);
	STDMETHODIMP RegisterFilter(REFCLSID clsidFilter, LPCWSTR Name, IMoniker **ppMoniker, const CLSID *pclsidCategory,
		LPCOLESTR szInstance, const REGFILTER2 *prf2);
	STDMETHODIMP EnumMatchingFilters(IEnumMoniker **ppEnum, DWORD dwFlags, BOOL bExactMatch, DWORD dwMerit, 
		BOOL bInputNeeded, DWORD cInputTypes, const GUID *pInputTypes, const REGPINMEDIUM *pMedIn, 
		const CLSID *pPinCategoryIn, BOOL bRender, BOOL bOutputNeeded, DWORD cOutputTypes, const GUID *pOutputTypes,
		const REGPINMEDIUM *pMedOut, const CLSID *pPinCategoryOut);
private:
	CComPtr<IFilterMapper2> m_pFM2;
};
