#pragma once

class CFakeFilterMapper2
	: public IFilterMapper2
{
	static bool ms_bInitialized;
	static CCriticalSection ms_lockHooking;
public:
	static CFakeFilterMapper2* ms_pFilterMapper2;
public:
	CFakeFilterMapper2(void);
	virtual ~CFakeFilterMapper2(void);
	HRESULT Register(LPCTSTR lpszFileName);
	HKEY RegisterMediaType(CStringW strCLSID);
	void RegisterMediaType(uint32_t nID, const CStringW& strChkBytes);
	/* IFilterMapper2 */
	virtual HRESULT STDMETHODCALLTYPE CreateCategory(REFCLSID clsidCategory, DWORD dwCategoryMerit, LPCWSTR Description);
	virtual HRESULT STDMETHODCALLTYPE UnregisterFilter(const CLSID *pclsidCategory, LPCOLESTR szInstance, REFCLSID Filter);
	virtual HRESULT STDMETHODCALLTYPE RegisterFilter(REFCLSID clsidFilter, LPCWSTR Name, IMoniker **ppMoniker, 
		const CLSID *pclsidCategory, LPCOLESTR szInstance, const REGFILTER2 *prf2);
	virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters(IEnumMoniker **ppEnum, DWORD dwFlags, BOOL bExactMatch, DWORD dwMerit,
		BOOL bInputNeeded, DWORD cInputTypes, const GUID *pInputTypes, const REGPINMEDIUM *pMedIn, const CLSID *pPinCategoryIn, 
		BOOL bRender, BOOL bOutputNeeded, DWORD cOutputTypes, const GUID *pOutputTypes, const REGPINMEDIUM *pMedOut, 
		const CLSID *pPinCategoryOut);
	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
private:
	struct RegisteredMediaType
	{
		
	};

	CComPtr<IFilterMapper2>	m_pFM2;
	std::map<CStringW, uint32_t> m_MediaTypeIDs;
	std::vector<RegisteredMediaType> m_RegisteredMediaTypes;
};