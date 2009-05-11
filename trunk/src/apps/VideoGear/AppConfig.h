#pragma once

enum VideoRenderMode
{
	VRM_DEFAULT,
	VRM_VMR9,
	VRM_EVR
};

class CAppConfig
{
public:
	VideoRenderMode m_VideoRenderMode;

	CAppConfig(void);
};

extern CAppConfig g_AppCfg;