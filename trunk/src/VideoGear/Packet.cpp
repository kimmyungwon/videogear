#include "StdAfx.h"
#include "Packet.h"

CPacket::CPacket( void )
: m_pData(NULL)
{
}

CPacket::~CPacket(void)
{
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}

void CPacket::Initialize( const BYTE* pData, UINT cbData, double dPTS, double dDuration )
{
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	m_pData = new BYTE[cbData];
	m_cbData = cbData;
	memcpy_s(m_pData, m_cbData, pData, cbData);
	m_dPTS = dPTS;
	m_dDuration = dDuration;
}
//////////////////////////////////////////////////////////////////////////

CFFPacket::CFFPacket( AVRational& timebase, AVPacket* pPacket )
{
	ASSERT(pPacket != NULL);

	Initialize(pPacket->data, pPacket->size, pPacket->pts * timebase.num * 1000.0 / timebase.den, 
		pPacket->duration * timebase.num * 1000.0 / (double)timebase.den);
}