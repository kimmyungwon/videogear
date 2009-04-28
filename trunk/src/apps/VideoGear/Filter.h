#pragma once

class CFilter
{
public:
	CFilter(LPCWSTR name, const CLSID& clsID);
	virtual ~CFilter(void)	{}
	virtual HRESULT CreateObject(IBaseFilter** ppvObj) = 0;	
protected:
	CStringW	m_name;
	CLSID		m_clsID;
};

class CInternalFilter : public CFilter
{
public:
	CInternalFilter(LPCWSTR name, const CLSID& clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit);
	virtual HRESULT CreateObject(IBaseFilter** ppvObj);
private:
	LPFNNewCOMObject	m_lpfnNew;
	LPFNInitRoutine		m_lpfnInit;
};