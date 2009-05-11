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
	const CStringW& GetName(void) const	{ return m_strName; }
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv) = 0;
	virtual HRESULT CheckInputType(const CMediaType& mt) = 0; 
private:
	CLSID m_clsID;
	CStringW m_strName;
};

class CInternalFilter : public CFilter
{
public:
	CInternalFilter(const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, UINT uiPinCount, const AMOVIESETUP_PIN* pPins);
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv);
	virtual HRESULT CheckInputType(const CMediaType& mt);
private:
	typedef CAtlMap<GUID, int> MinorTypes;
	typedef CAtlMap<GUID, MinorTypes> MajorTypes;

	PFNCreateInstance m_pfnCreateInstance;
	MajorTypes m_majorTypes;
};
