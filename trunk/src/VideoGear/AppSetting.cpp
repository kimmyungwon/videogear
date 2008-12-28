#include "StdAfx.h"
#include "AppSetting.h"

CAppSetting* AfxGetAppSetting(void)
{
	static CAppSetting iAppSetting;
	
	return &iAppSetting;
}

CAppSetting::CAppSetting(void)
{
	INIT_PROPPERTY(InitialVideoSize, IVS_ZOOM100);
}

CAppSetting::~CAppSetting(void)
{
}
