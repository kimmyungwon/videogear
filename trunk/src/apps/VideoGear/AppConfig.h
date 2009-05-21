#pragma once

enum VideoRenderMode
{
	VRM_VMR9,
	VRM_VMR9Renderless,
	VRM_EVR
};

class CAppConfig
{
public:
	/*  ”∆µ≈‰÷√ */
	VideoRenderMode m_VideoRenderMode;
	/* “Ù∆µ≈‰÷√ */
	bool m_bUseAudioSwitcher;

	CAppConfig(void);
};

extern CAppConfig g_AppCfg;