#pragma once

class CFakeFilterMapper2
	: public IFilterMapper2
{
	static bool ms_bInitialized;
public:
	CFakeFilterMapper2(void);
	virtual ~CFakeFilterMapper2(void);
protected:
	/* IFilterMapper2 */
	virtual HRESULT STDMETHODCALLTYPE CreateCategory(REFCLSID clsidCategory, DWORD dwCategoryMerit, LPCWSTR Description);
	virtual HRESULT STDMETHODCALLTYPE UnregisterFilter(const CLSID *pclsidCategory, LPCOLESTR szInstance, REFCLSID Filter);
	virtual HRESULT STDMETHODCALLTYPE RegisterFilter(REFCLSID clsidFilter, LPCWSTR Name, IMoniker **ppMoniker, const CLSID *pclsidCategory,
		LPCOLESTR szInstance, const REGFILTER2 *prf2);
	virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters(IEnumMoniker **ppEnum, DWORD dwFlags, BOOL bExactMatch, DWORD dwMerit,
		BOOL bInputNeeded, DWORD cInputTypes, const GUID *pInputTypes, const REGPINMEDIUM *pMedIn, const CLSID *pPinCategoryIn, BOOL bRender, 
		BOOL bOutputNeeded, DWORD cOutputTypes, const GUID *pOutputTypes, const REGPINMEDIUM *pMedOut, const CLSID *pPinCategoryOut);
private:
	CComPtr<IFilterMapper2>	m_pFM2;
};