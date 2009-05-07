#include "StdAfx.h"
#include "Output.h"

COutput::COutput(void)
{
}

COutput::~COutput(void)
{
}

void COutput::Initialize( MediaType type )
{
	m_type = type;
}

void COutput::Delivery( CPacket* pPacket )
{
	CAutoLock lock(&m_packQLock);
	m_packQ.push_back(pPacket);
}
//////////////////////////////////////////////////////////////////////////

void CFFOutput::Initialize( AVStream* pStream )
{
	ASSERT(pStream != NULL);

	MediaType type;

	switch (pStream->codec->codec_type)
	{
	case CODEC_TYPE_VIDEO:
		type = MTYPE_VIDEO;
		break;
	case CODEC_TYPE_AUDIO:
		type = MTYPE_AUDIO;
		break;
	case CODEC_TYPE_SUBTITLE:
		type = MTYPE_SUBTITLE;
		break;
	default:
		type = MTYPE_UNKNOWN;
	}

	COutput::Initialize(type);
}