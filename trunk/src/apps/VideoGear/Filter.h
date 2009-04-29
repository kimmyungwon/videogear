#pragma once

#include "Util.h"

class CFilter
{
public:
	CFilter(LPCTSTR name, const CLSID& clsID)
		: m_name(name), m_clsID(clsID)
	{
	}

	virtual ~CFilter(void)	{}
	virtual HRESULT CreateObject(IBaseFilter** ppvObj) = 0;	
	const CString& GetName(void) const	{ return m_name; }
	const CLSID& GetClassID(void) const	{ return m_clsID; }
protected:
	CString	m_name;
	CLSID	m_clsID;
};

class CSourceFilter : public CFilter
{
public:
	CSourceFilter(LPCTSTR name, const CLSID& clsID)
		: CFilter(name, clsID)
	{
	}

	virtual void AddSignature(const CString& chkbytes)
	{
		std::list<CString> tokens;
		file_sign_t* sign;
		
		sign = new file_sign_t;
		ExtractStrings(_T(','), chkbytes, tokens);
		while (tokens.size() >= 4)
		{
			file_sign_t::part_t part;
			
			part.offset = _tcstoi64(tokens.front(), NULL, 10);
			tokens.pop_front();
			part.length = _tcstol(tokens.front(), NULL, 10);
			tokens.pop_front();
			tokens.pop_front();	// ºöÂÔmask
			HexToBin(tokens.front(), part.bytes);
			tokens.pop_front();
			sign->parts.push_back(part);
		}
		m_fileSigns.push_back(sign);
	}

	bool CheckSignature(CFile& file)
	{
		for (file_sign_list_t::const_iterator itSign = m_fileSigns.begin(); itSign != m_fileSigns.end(); itSign++)
		{
			bool matched = false;			
			for (file_sign_t::parts_t::const_iterator itPart = itSign->parts.begin(); itPart != itSign->parts.end(); itPart++)
			{
				CAutoVectorPtr<char> buffer;
				
				file.Seek(itPart->offset, itPart->offset >= 0 ? CFile::begin : CFile::end);
				buffer.Allocate(itPart->length);
				if (file.Read(buffer.m_p, itPart->length) == itPart->length
					&& memcmp(itPart->bytes.c_str(), buffer.m_p, itPart->length) == 0)
					matched = true;
				else
				{
					matched = false;
					break;
				}
			}
			if (matched)
				return true;
		}
		return false;
	}
private:
	struct file_sign_t
	{
		struct part_t{
			LONGLONG offset;
			UINT length;
			std::string bytes;
		};
		typedef std::vector<part_t> parts_t;
		parts_t parts;
	};
	typedef boost::ptr_vector<file_sign_t> file_sign_list_t;

	file_sign_list_t	m_fileSigns;
};

template<typename BaseT>
class CInternal : public BaseT
{
public:
	CInternal(LPCTSTR name, const CLSID& clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit)
		: BaseT(name, clsID), m_lpfnNew(lpfnNew), m_lpfnInit(lpfnInit)
	{
	}

	virtual HRESULT CreateObject(IBaseFilter** ppvObj)
	{
		HRESULT hr;
		CBaseFilter* bf;

		if (m_lpfnNew == NULL)
			return E_UNEXPECTED;
		if (ppvObj == NULL)
			return E_POINTER;
		bf = static_cast<CBaseFilter*>(m_lpfnNew(NULL, &hr));
		if (SUCCEEDED(hr) && bf != NULL)
		{
			bf->AddRef();
			*ppvObj = (IBaseFilter*)bf;
		}
		return hr;
	}
private:
	LPFNNewCOMObject	m_lpfnNew;
	LPFNInitRoutine		m_lpfnInit;
};

class CInternalFilter : public CInternal<CFilter>
{
public:
	CInternalFilter(LPCTSTR name, const CLSID& clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit)
		: CInternal<CFilter>(name, clsID, lpfnNew, lpfnInit)
	{
	}
};

class CInternalSourceFilter : public CInternal<CSourceFilter>
{
public:
	CInternalSourceFilter(LPCTSTR name, const CLSID& clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit)
		: CInternal<CSourceFilter>(name, clsID, lpfnNew, lpfnInit)
	{
	}
};