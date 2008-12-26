#pragma once

#include "IGraphBuilder2.h"

class CVGPlayer
{
public:
	CVGPlayer(void);
	virtual ~CVGPlayer(void);
	HRESULT Initialize(HWND hwnd);
protected:
	void Clear(void);
private:
	CComPtr<IGraphBuilder2>	m_pGB;
	HANDLE					m_hIOCP;
	HANDLE					m_hMsgThrd;
};
