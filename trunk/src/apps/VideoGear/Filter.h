#pragma once

#include "DSUtil.h"

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

struct CheckBytes
{
	LONGLONG llOffset;
	LONGLONG llLength;
	std::string strMask;
	std::string strSign;
};

struct MediaSignature
{
	CStringW strChkBytes;
	std::vector<CheckBytes> signs;

	MediaSignature(LPCWSTR pszChkBytes)
	{
		Assign(pszChkBytes);
	}

	void Assign(LPCWSTR pszChkBytes)
	{
		CAtlList<CStringW> lst;
		
		strChkBytes = pszChkBytes;
		signs.clear();
		Explode(strChkBytes, lst, L',');
		while (lst.GetCount() >= 4)
		{
			CheckBytes sign;
			CStringW strOff = lst.RemoveHead();
			CStringW strLen = lst.RemoveHead();
			CStringW strMask = lst.RemoveHead();
			CStringW strBytes = lst.RemoveHead();
			sign.llOffset = _wcstoi64(strOff, NULL, 10);
			sign.llLength = _wcstoi64(strLen, NULL, 10);
			CStringToBin(strBytes, sign.strSign);
			if (!strMask.IsEmpty())
				CStringToBin(strMask, sign.strMask);
			else
				sign.strMask.resize(sign.strSign.size(), (char)0xFF);
			signs.push_back(sign);
		}
	}

	bool operator<(const MediaSignature &rhs) const
	{
		return strChkBytes < rhs.strChkBytes;
	}
	
	MediaSignature& operator=(const MediaSignature &rhs)
	{
		Assign(rhs.strChkBytes);
		return *this;
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

class CFilterDMO : public CFilter
{
public:
	CFilterDMO(const CLSID &clsID, LPCWSTR pszName, const CLSID &clsidCategory);
	virtual HRESULT CreateInstance(LPUNKNOWN pUnk, IBaseFilter** ppv);
private:
	CLSID m_clsidCategory;
};
