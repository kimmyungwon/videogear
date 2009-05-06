#pragma once

#include "Packet.h"

class COutput
{
public:
	COutput(void);
	virtual ~COutput(void);
	void Initialize(int nType);
	virtual MediaType GetType(void) const	{ return m_type; }
	void Delivery(CPacket* pPacket);	
private:
	typedef boost::ptr_vector<CPacket> PacketQueue;

	MediaType m_type;
	PacketQueue m_packq;
};

class CFFOutput : public COutput
{
public:
	void Initialize(AVStream* pStream);
};