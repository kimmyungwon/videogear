#pragma once

#include "VGIntf.h"

class CVGPlayer : public IVGPlayer
{
	VG_DECLARE_IUNKNOWN(CVGPlayer)
public:
	CVGPlayer(void);
	~CVGPlayer(void);
};
