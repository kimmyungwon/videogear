#pragma once

#include "Packet.h"
#include "Thread.h"

class COutput
{
public:
	COutput(void);
	virtual ~COutput(void);
	void Initialize(MediaType type);
	virtual MediaType GetType(void) const	{ return m_type; }
	void Delivery(CPacket* pPacket);	
private:
	typedef boost::ptr_vector<CPacket> PacketQueue;

	MediaType m_type;
	PacketQueue m_packQ;
	CCritSec m_packQLock;
};

class CFFOutput : public COutput
{
public:
	void Initialize(AVStream* pStream);
};