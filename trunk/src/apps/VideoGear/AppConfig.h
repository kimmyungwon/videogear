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
	VideoRenderMode m_vmr;

	CAppConfig(void);
};

extern CAppConfig g_AppCfg;