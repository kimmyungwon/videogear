#include "StdAfx.h"
#include "Output.h"

COutput::COutput(void)
{
}

COutput::~COutput(void)
{
}

void COutput::Delivery( CPacket* pPacket )
{
	m_packq.push_back(pPacket);
}
//////////////////////////////////////////////////////////////////////////

CFFOutput::CFFOutput( AVStream* pStream )
: m_pStream(pStream)
{

}