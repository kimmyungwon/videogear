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
	/* ��Ƶ���� */
	VideoRenderMode m_VideoRenderMode;
	/* ��Ƶ���� */
	bool m_bUseAudioSwitcher;

	CAppConfig(void);
};

extern CAppConfig g_AppCfg;