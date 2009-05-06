#pragma once

#include "MediaType.h"

class CPacket
{
public:
	CPacket(MediaType type);
	virtual ~CPacket(void);
	MediaType GetType(void) const	{ return m_type; }
private:
	MediaType m_type;
};

class CFFPacket : public CPacket
{
public:
	CFFPacket(MediaType nType);
};
