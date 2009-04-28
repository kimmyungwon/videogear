#pragma once

class CFilter
{
public:
	CFilter(LPCTSTR name, const CLSID& clsID);
	virtual ~CFilter(void)	{}
	virtual HRESULT CreateObject(IBaseFilter** ppvObj) = 0;	
	const CString& GetName(void)	{ return m_name; }
protected:
	CString	m_name;
	CLSID	m_clsID;
};

class CInternalFilter : public CFilter
{
public:
	CInternalFilter(LPCTSTR name, const CLSID& clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit);
	virtual HRESULT CreateObject(IBaseFilter** ppvObj);
private:
	LPFNNewCOMObject	m_lpfnNew;
	LPFNInitRoutine		m_lpfnInit;
};