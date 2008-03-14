#pragma once

#include "VGIntf.h"

class CVGFilterManager : public IVGFilterManager
{
	VG_DECLARE_IUNKNOWN(CVGPlayer)
public:
	CVGFilterManager(void);
	~CVGFilterManager(void);
};
