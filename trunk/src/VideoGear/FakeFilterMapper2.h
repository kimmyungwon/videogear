#pragma once

class CFakeFilterMapper2 : public CUnknown
						 , public IFilterMapper2
{
public:
	CFakeFilterMapper2(LPUNKNOWN pUnk);
	virtual ~CFakeFilterMapper2(void);
private:
	CComPtr<IFilterMapper2> m_pFM2;
};
