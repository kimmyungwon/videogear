#pragma once

#include "VGIntf.h"
#include "VGFilterManager.h"

class CVGPlayer : public IVGPlayer
{
	VG_DECLARE_IUNKNOWN(CVGPlayer)
private:
	IVGFilterManagerPtr		m_pFilterManager;
public:
	CVGPlayer(void);
	~CVGPlayer(void);
	HRESULT STDMETHODCALLTYPE Initialize(void);
};
