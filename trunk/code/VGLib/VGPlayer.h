#pragma once

#include "VGIntf.h"
#include "VGBaseObject.h"
#include "VGFilterManager.h"

class CVGPlayer : public CVGUnknownImpl<IVGPlayer, IID_IVGPlayer>
{
private:
	IVGFilterManagerPtr		m_pFM;	
public:
	CVGPlayer(void);
	virtual ~CVGPlayer(void);
	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID refiid, void **ppv);
};
