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

void CPacket::Initialize( const BYTE* pData, UINT cbData, LONGLONG llPTS, LONGLONG llDuration )
{
	if (m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	m_pData = new BYTE[cbData];
	m_cbData = cbData;
	memcpy_s(m_pData, m_cbData, pData, cbData);
	m_llPTS = llPTS;
	m_llDuration = llDuration;
}
//////////////////////////////////////////////////////////////////////////

CFFPacket::CFFPacket( AVPacket* pPacket )
{
	ASSERT(pPacket != NULL);

	Initialize(pPacket->data, pPacket->size, pPacket->pts, pPacket->duration);
}