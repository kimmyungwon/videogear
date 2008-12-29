#pragma once

#include "VideoGear.h"

#define DEFINE_PROPERTY(type,name)	private: type m_prop##name;	\
									public: const type& Get##name(void)	\
										{ CAutoLock lock(this); return m_prop##name; }	\
									public:	void Set##name(const type& v)	\
										{ CAutoLock lock(this); m_prop##name = v; }

#define INIT_PROPPERTY(name,value)	m_prop##name = (value);

enum {
	IVS_DONTCARE = 0,
	IVS_ZOOM50,
	IVS_ZOOM100,
	IVS_ZOOM200
};

enum {
	VARM_VIDEO = 0,
	VARM_SCREEN,
	VARM_KEEP_4x3,
	VARM_KEEP_16x9
};

class CAppSetting : public CCritSec
{
public:
	CAppSetting(void);
	virtual ~CAppSetting(void);
	DEFINE_PROPERTY(UINT, InitialVideoSize)
	DEFINE_PROPERTY(UINT, VideoAspectRatioMode)
};

CAppSetting& AfxGetAppSetting(void);