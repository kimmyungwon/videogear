#include "stdafx.h"
#include "AppConfig.h"

CAppConfig g_appCfg;

CAppConfig::CAppConfig(void)
{
	m_VideoRenderer = VR_VMR9;
}