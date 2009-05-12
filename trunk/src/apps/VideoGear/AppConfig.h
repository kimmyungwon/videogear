#pragma once

enum VideoRenderMode
{
	VRM_VMR9,
	VRM_EVR
};

class CAppConfig
{
public:
	VideoRenderMode m_VideoRenderMode;
	bool m_bUseAudioSwitcher;

	CAppConfig(void);
};

extern CAppConfig g_AppCfg;