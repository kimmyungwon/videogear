#pragma once

#include "RegTree.h"

class CFakeFilterMapper2
	: public IFilterMapper2
{
	friend CFakeFilterMapper2* AfxGetFakeFM2(void);
public:
	bool m_bHooking;
public:
	void Initialize(void);
	void Uninitialize(void);
	HRESULT Register(LPCTSTR lpszFileName);
	CRegTreeNode* RegNodeFromHKEY(HKEY hKey);
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
protected:
	CFakeFilterMapper2(void);
private:
	CComPtr<IFilterMapper2>	m_pFM2;
	CCriticalSection m_lockHooking;
	std::map<HKEY, CStringW> m_rootKeys;
	CRegTree m_regDB;
};

CFakeFilterMapper2* AfxGetFakeFM2(void);