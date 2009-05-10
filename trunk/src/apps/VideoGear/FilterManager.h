#pragma once

typedef HRESULT (*PFNCreateInstanceProc)(LPUNKNOWN, IBaseFilter**);

class CFilterManager
{
public:
	CFilterManager(void);
	virtual ~CFilterManager(void);
protected:
	HRESULT RegisterFilter(LPCWSTR pszName, PFNCreateInstanceProc pfnCreateInst, const AMOVIESETUP_PIN* pPins, size_t nPinCount);
};
