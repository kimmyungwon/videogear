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
	CFilter(const CLSID& clsID);
	virtual ~CFilter(void);
	virtual const CStringW& GetName(void) const	= 0;
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv) = 0;
protected:	
	CLSID m_clsID;
};

class CFilterInternal : public CFilter
{
public:
	CFilterInternal(const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, UINT uiPinCount, const AMOVIESETUP_PIN* pPins);
	virtual const CStringW& GetName(void) const	{ return m_strName; }
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv);
private:
	CStringW m_strName;
	PFNCreateInstance m_pfnCreateInstance;
};

/*class CFilterRegister : public CFilter
{
public:
	CFilterRegister(const CLSID& clsID);
	virtual const CStringW& GetName(void) const	{ return m_strName; }
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv);
protected:
	void ExtractFilterData(BYTE *pData, UINT cbData);
private:
	CStringW m_strName;
};*/
