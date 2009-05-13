#pragma once

typedef HRESULT (*PFNCreateInstance)(LPUNKNOWN, IBaseFilter**);

struct MediaType
{
	GUID major;
	GUID minor;

	MediaType(const GUID& _major, const GUID& _minor)
		: major(_major), minor(_minor)
	{}

	bool operator==(const MediaType& rhs) const
	{
		return major == rhs.major && minor == rhs.minor;
	}
};

class CFilter
{
public:
	CFilter(const CLSID& clsID, LPCWSTR pszName);
	virtual ~CFilter(void);
	const CLSID& GetCLSID(void) const	{ return m_clsID; }
	const CStringW& GetName(void) const	{ return m_strName; }
	DWORD GetMerit(void) const	{ return m_dwMerit; }
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv) = 0;
protected:	
	CLSID m_clsID;
	CStringW m_strName;
	DWORD m_dwMerit;
};

class CFilterInternal : public CFilter
{
public:
	CFilterInternal(const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, UINT uiPinCount, const AMOVIESETUP_PIN* pPins);
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv);
private:	
	PFNCreateInstance m_pfnCreateInstance;
};

class CFilterRegister : public CFilter
{
public:
	CFilterRegister(const CLSID& clsID, LPCWSTR pszName);
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv);
};
