#pragma once

#include "Packet.h"

class COutput
{
public:
	COutput(void);
	virtual ~COutput(void);
	virtual int GetType(void) const = 0;
	void Delivery(CPacket* pPacket);
private:
	typedef boost::ptr_vector<CPacket> PacketQueue;

	PacketQueue m_packq;
};

class CFFOutput : public COutput
{
public:
	CFFOutput(AVStream* pStream);
	virtual int GetType(void) const;
private:
	AVStream* m_pStream;
};