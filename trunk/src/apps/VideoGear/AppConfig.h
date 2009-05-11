#pragma once

enum VideoRenderer
{
	VR_DEFAULT,
	VR_VMR9,
	VR_EVR
};

class CAppConfig
{
public:
	VideoRenderer m_VideoRenderer;

	CAppConfig(void);
};

extern CAppConfig g_appCfg;