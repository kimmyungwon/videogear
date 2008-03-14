#pragma once

#include "VGIntf.h"
#include "../Common/guliverkli/src/filters/parser/realmediasplitter/RealMediaSplitter.h"

_COM_SMARTPTR_TYPEDEF(IGraphBuilder, __uuidof(IGraphBuilder));

class CVGFilterManager : public IVGFilterManager
{
	VG_DECLARE_IUNKNOWN(CVGPlayer)
private:
	IGraphBuilderPtr	m_pGB;	
public:
	CVGFilterManager(void);
	~CVGFilterManager(void);
	HRESULT STDMETHODCALLTYPE Initialize(void);
};
