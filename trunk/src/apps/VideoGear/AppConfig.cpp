#include "stdafx.h"
#include "AppConfig.h"

CAppConfig g_AppCfg;

CAppConfig::CAppConfig( void )
{
	m_VideoRenderMode = VRM_VMR9Renderless;
	m_bUseAudioSwitcher = true;
}