#include "StdAfx.h"
#include "Source.h"

int FFReadPacket( void *opaque, uint8_t *buf, int buf_size )
{
	if (opaque != NULL && buf_size > 0)
		return (int)((CStream*)opaque)->Read((BYTE*)buf, buf_size);
	else
		return 0;
}

int FFWritePacket( void *opaque, uint8_t *buf, int buf_size )
{
	if (opaque != NULL && buf_size > 0)
		return (int)((CStream*)opaque)->Write((BYTE*)buf, buf_size);
	else
		return 0;	
}

int64_t FFSeek( void *opaque, int64_t offset, int whence )
{
	if (opaque != NULL)
		return ((CStream*)opaque)->Seek((LONGLONG)offset, whence);
	else
		return -1;
}

int FFReadPause( void *opaque, int pause )
{
	return AVERROR(ENOSYS);
}

int64_t FFReadSeek( void *opaque, int stream_index, int64_t timestamp, int flags )
{
	return AVERROR(ENOSYS);
}

//////////////////////////////////////////////////////////////////////////

CSource::CSource( CStream* pStream, bool bOwnsObject /*= false*/ )
: m_pStream(pStream), m_bOwnsObject(bOwnsObject)
{

}

const CString& CSource::GetFileName( void ) const
{
	ASSERT(m_pStream != NULL);
	return m_pStream->GetName();
}

CStringA CSource::GetFileNameA( void ) const
{
	ASSERT(m_pStream != NULL);
	USES_CONVERSION;
	return T2A(m_pStream->GetName());
}

HRESULT CSource::Reload( void )
{
	ASSERT(m_pStream != NULL);
	return m_pStream->Open(m_pStream->GetName());
}
//////////////////////////////////////////////////////////////////////////

CFFSource::CFFSource( CStream* pStream, bool bOwnsObject /*= false*/ )
: CSource(pStream, bOwnsObject)
{
	if (m_pStream != NULL)
	{
		UINT nMaxPacketSize, nSize;
		BYTE* pBuffer;

		nMaxPacketSize = m_pStream->GetMaxPacketSize();
		nSize = nMaxPacketSize > 0 ? nMaxPacketSize : SOURCE_BUFFER_SIZE;
		pBuffer = (BYTE*)av_malloc(nSize > 0 ? nSize : SOURCE_BUFFER_SIZE);
		if (pBuffer == NULL)
			return;
		init_put_byte(this, pBuffer, nSize, 0, m_pStream, FFReadPacket, FFWritePacket, FFSeek);
		is_streamed = 0;
		max_packet_size = nMaxPacketSize;
		read_pause = FFReadPause;
		read_seek = FFReadSeek;
	}
}

CFFSource::~CFFSource(void)
{
	if (buffer != NULL)
		av_freep(&buffer); 
	if (m_bOwnsObject && m_pStream != NULL)
	{
		m_pStream->Close();
		delete m_pStream;
		m_pStream = NULL;
	}
}