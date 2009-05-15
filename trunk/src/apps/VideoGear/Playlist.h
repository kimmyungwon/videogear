#pragma once

#include "Thread.h"

class CPlaylist : public CCritSec
{
public:
	CPlaylist(void);
	virtual ~CPlaylist(void);
};
