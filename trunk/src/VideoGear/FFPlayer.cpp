#include "StdAfx.h"
#include "FFPlayer.h"
#include "VideoGear.h"
#include "Defines.h"
#include "Cpuid.h"

#define IO_BUFFER_SIZE	32768

int FFReadPacket( void *opaque, uint8_t *buf, int buf_size )
{
	CSource* pSource = static_cast<CSource*>(opaque);
	if (pSource != NULL && buf_size > 0)
		return (int) pSource->Read((BYTE*)buf, buf_size);
	else
		return 0;
}

int FFWritePacket( void *opaque, uint8_t *buf, int buf_size )
{
	return -1;
}

int64_t FFSeek( void *opaque, int64_t offset, int whence )
{
	CSource* pSource = static_cast<CSource*>(opaque);
	if (pSource != NULL)
		return pSource->Seek((LONGLONG)offset, whence);
	else
		return -1;
}

//////////////////////////////////////////////////////////////////////////

class CFFDecoder : public CWorker
{
public:
	CFFDecoder(CFFPlayer& Player)
		: m_Player(Player)
	{}
protected:
	CFFPlayer& m_Player;
};

class CFFDemuxer : public CFFDecoder
{
public:
	CFFDemuxer(CFFPlayer& Player)
		: CFFDecoder(Player)
	{}

	virtual void Execute(bool& bTerminated)
	{
		m_Player.Demuxe(bTerminated);
	}
};

class CFFVideoDecoder : public CFFDecoder
{
public:
	CFFVideoDecoder(CFFPlayer& Player)
		: CFFDecoder(Player)
	{}

	virtual void Execute(bool& bTerminated)
	{
		m_Player.DecodeVideo(bTerminated);
	}
};

class CFFAudioDecoder : public CFFDecoder
{
public:
	CFFAudioDecoder(CFFPlayer& Player)
		: CFFDecoder(Player)
	{}

	virtual void Execute(bool& bTerminated)
	{
		m_Player.DecodeAudio(bTerminated);
	}
};

//////////////////////////////////////////////////////////////////////////

CFFPlayer::CFFPlayer(void)
: m_State(PS_IDLE)
{
	m_pSource = NULL;  
	m_pffIOCtx = NULL;
	m_pffFmtCtx = NULL;
	m_pSwsCtx = NULL;
	m_pDemuxerThread = NULL;
	m_vidDecCtx.Stream = NULL;
	m_vidDecCtx.Thread = NULL;
	m_audDecCtx.Stream = NULL;
	m_audDecCtx.Thread = NULL;
}

CFFPlayer::~CFFPlayer(void)
{
	Clean();
	sws_freeContext(m_pSwsCtx);
	m_pSwsCtx = NULL;
}

HRESULT CFFPlayer::RenderFile( LPCTSTR pszFile )
{
	HRESULT hr;

	if (m_State != PS_IDLE)
		RIF(Clean());
	m_pSource = new CFileSource;
	hr = m_pSource->Open(pszFile);
	if (FAILED(hr))
	{
		SAFE_DELETE(m_pSource);
		return hr;
	}
	else
	{
		hr = RenderSource();
		if (SUCCEEDED(hr))
			m_State = PS_STOPPED;
		else
			SAFE_DELETE(m_pSource);
		return hr;
	}
}

HRESULT CFFPlayer::Play( void )
{
	if (m_State == PS_STOPPED)
	{		
		if (m_vidDecCtx.Stream != NULL)
		{
			m_vidDecCtx.Stream->discard = AVDISCARD_DEFAULT;
			m_vidDecCtx.Thread = new CThread(new CFFVideoDecoder(*this));
		}
		if (m_audDecCtx.Stream != NULL)
		{
			m_audDecCtx.Stream->discard = AVDISCARD_DEFAULT;
			m_audDecCtx.Thread = new CThread(new CFFAudioDecoder(*this));
		}
		if (m_vidDecCtx.Thread != NULL || m_audDecCtx.Thread != NULL)
			m_pDemuxerThread = new CThread(new CFFDemuxer(*this));
		m_State = PS_RUNNING;
		return S_OK;
	}
	else
	{
		//TODO: 恢复播放
		return E_NOTIMPL;
	}
}

#define CleanPacketQueue(q)	while (q.size() > 0) { \
								AVPacket* pPacket = q.front();	\
								q.pop_front(); \
								av_free_packet(pPacket); \
								SAFE_DELETE(pPacket); \
							}

#define CleanFrameQueue(q)	while (q.size() > 0) { \
								AVFrame* pFrame = q.front();	\
								q.pop_front(); \
								av_free(pFrame); \
							}


HRESULT CFFPlayer::Stop( void )
{
	SAFE_DELETE(m_pDemuxerThread);
	
	SAFE_DELETE(m_vidDecCtx.Thread);	
	m_vidDecCtx.PacketsLock.Enter();
	CleanPacketQueue(m_vidDecCtx.Packets);
	m_vidDecCtx.PacketsLock.Leave();
	m_vidDecCtx.FramesLock.Enter();
	CleanFrameQueue(m_vidDecCtx.Frames);
	m_vidDecCtx.FramesLock.Leave();

	SAFE_DELETE(m_audDecCtx.Thread);
	m_audDecCtx.PacketsLock.Enter();
	CleanPacketQueue(m_audDecCtx.Packets);
	m_audDecCtx.PacketsLock.Leave();
	m_audDecCtx.FramesLock.Enter();
	CleanFrameQueue(m_audDecCtx.Frames);
	m_audDecCtx.FramesLock.Leave();

	m_State = PS_STOPPED;
	return S_OK;
}

HRESULT CFFPlayer::Clean( void )
{
	if (m_State != PS_IDLE)
		RIF(Stop());
	if (m_vidDecCtx.Stream != NULL && m_vidDecCtx.Stream->codec != NULL)
		avcodec_close(m_vidDecCtx.Stream->codec);
	if (m_audDecCtx.Stream != NULL && m_audDecCtx.Stream->codec != NULL)
		avcodec_close(m_audDecCtx.Stream->codec);
	if (m_pffFmtCtx != NULL)
	{
		av_close_input_stream(m_pffFmtCtx);
		m_pffFmtCtx = NULL;
	}
	if (m_pffIOCtx != NULL)
	{
		if (m_pffIOCtx->buffer != NULL)
			av_freep(&m_pffIOCtx->buffer);
		av_freep(&m_pffIOCtx);
	}
	SAFE_DELETE(m_pSource);
	m_State = PS_IDLE;
	return S_OK;
}

#define PROBE_BUF_MIN 2048
#define PROBE_BUF_MAX (1<<20)

HRESULT CFFPlayer::RenderSource( void )
{
	ASSERT_POINTER(m_pSource, CSource);
	
	UINT nMaxPacketSize, nSize;
	BYTE* pBuffer;
	CStringA strNameA;
	AVProbeData ffPrb;
	AVInputFormat* pInFmt;
	HRESULT hr;

	nMaxPacketSize = m_pSource->GetMaxPacketSize();
	nSize = nMaxPacketSize > 0 ? nMaxPacketSize : IO_BUFFER_SIZE;
	pBuffer = (BYTE*)av_malloc(nSize);
	if (pBuffer == NULL)
		return E_OUTOFMEMORY;
	m_pffIOCtx = av_alloc_put_byte(pBuffer, nSize, 0, m_pSource, FFReadPacket, FFWritePacket, FFSeek);
	if (m_pffIOCtx == NULL)
	{
		av_freep(&pBuffer);
		return E_OUTOFMEMORY;
	}
	m_pffIOCtx->is_streamed = 0;
	m_pffIOCtx->max_packet_size = nMaxPacketSize;
	/* 检查文件格式 */
	strNameA = m_pSource->GetNameA();
	ffPrb.filename = strNameA;
	ffPrb.buf = NULL;
	ffPrb.buf_size = 0;
	pInFmt = av_probe_input_format(&ffPrb, 0);
	if (pInFmt == NULL || (m_pffFmtCtx->flags & AVFMT_NOFILE) == 0)
	{
		for (int nProbeSize = PROBE_BUF_MIN; nProbeSize <= PROBE_BUF_MAX && pInFmt == NULL; nProbeSize <<= 1)
		{
			// 读取数据
			ffPrb.buf = (unsigned char*)av_realloc(ffPrb.buf, nProbeSize + AVPROBE_PADDING_SIZE);
			ffPrb.buf_size = get_buffer(m_pffIOCtx, ffPrb.buf, nProbeSize);
			memset(&ffPrb.buf[ffPrb.buf_size], 0, AVPROBE_PADDING_SIZE);
			if (url_fseek(m_pffIOCtx, 0, SEEK_SET) < 0)
			{
				if (FAILED(m_pSource->Open(m_pSource->GetName())))
					break;
			}
			// 猜测文件类型
			pInFmt = av_probe_input_format(&ffPrb, 1);
		}
		av_freep(&ffPrb.buf);
	}
	if (pInFmt == NULL)
	{
		av_freep(&m_pffIOCtx);
		av_freep(&pBuffer);
		return VGERR_NOT_SUPPORTED;
	}
	if (pInFmt != NULL && (pInFmt->flags & AVFMT_NEEDNUMBER) != 0)
	{
		if (!av_filename_number_test(ffPrb.filename))
		{
			av_freep(&m_pffIOCtx);
			av_freep(&pBuffer);
			return VGERR_NOT_SUPPORTED;
		}
	}

	hr = RenderOutputs(pInFmt);
	if (FAILED(hr))
	{
		av_freep(&m_pffIOCtx);
		av_freep(&pBuffer);
	}
	return hr;
}

HRESULT CFFPlayer::RenderOutputs( AVInputFormat* pInFmt )
{
	ASSERT_POINTER(m_pSource, CSource);
	ASSERT_POINTER(m_pffIOCtx, ByteIOContext);
	ASSERT_POINTER(pInFmt, AVInputFormat);

	UINT nRendered = 0;

	if (av_open_input_stream(&m_pffFmtCtx, m_pffIOCtx, m_pSource->GetNameA(), pInFmt, NULL) < 0)
		return VGERR_NOT_SUPPORTED;
	if (av_find_stream_info(m_pffFmtCtx) < 0)
		return VGERR_NOT_SUPPORTED;
	for (UINT i = 0; i < m_pffFmtCtx->nb_streams; i++)
	{
		AVStream* pStream = m_pffFmtCtx->streams[i];
		AVCodec* pCodec = avcodec_find_decoder(pStream->codec->codec_id);
		if (pCodec != NULL)
		{
			nRendered++;
			switch (pStream->codec->codec_type)
			{
			case CODEC_TYPE_VIDEO:
				if (m_vidDecCtx.Stream == NULL && pCodec != NULL && avcodec_open(pStream->codec, pCodec) >= 0)
				{
					m_pSwsCtx = sws_getCachedContext(m_pSwsCtx, pStream->codec->width, pStream->codec->height, pStream->codec->pix_fmt,
						pStream->codec->width, pStream->codec->height, PIX_FMT_YUYV422, SWS_FAST_BILINEAR | GetSwsFlags(), NULL, NULL, NULL);
					if (m_pSwsCtx != NULL)
					{
						m_vidDecCtx.Stream = pStream;
					}
					else
					{
						avcodec_close(pStream->codec);
						nRendered--;
					}
				}
				break;
			case CODEC_TYPE_AUDIO:
				if (m_audDecCtx.Stream == NULL && pCodec != NULL && avcodec_open(pStream->codec, pCodec) >= 0)
					m_audDecCtx.Stream = pStream;
				break;
			}
		}
	}
	if (m_pffFmtCtx->nb_streams > 0)
		if (m_pffFmtCtx->nb_streams == nRendered)
			return S_OK;
		else
			return nRendered > 0 ? S_FALSE : VGERR_CANNOT_RENDER;
	else
		return S_OK;
}

void CFFPlayer::Demuxe( bool& bTerminated )
{
	while (!bTerminated)
	{
		AVPacket* pPacket = new AVPacket;	
		if (av_read_frame(m_pffFmtCtx, pPacket) < 0)
		{
			SAFE_DELETE(pPacket);
			break;
		}

		if (m_vidDecCtx.Stream != NULL && pPacket->stream_index == m_vidDecCtx.Stream->index)
		{
			m_vidDecCtx.PacketsLock.Enter();
			m_vidDecCtx.Packets.push_back(pPacket);
			m_vidDecCtx.PacketsLock.Leave();
		}
		else if (m_audDecCtx.Stream != NULL && pPacket->stream_index == m_audDecCtx.Stream->index)
		{
			m_audDecCtx.PacketsLock.Enter();
			m_audDecCtx.Packets.push_back(pPacket);
			m_audDecCtx.PacketsLock.Leave();
		}
		else
		{
			av_free_packet(pPacket);
			SAFE_DELETE(pPacket);
		}
	}
}

void CFFPlayer::DecodeVideo( bool& bTerminated )
{
	while (!bTerminated)
	{
		AVPacket* pPacket;
		AVFrame* pFrame;
		int nGotPicture = 0, nBytesUsed = 0;
		
		if (!m_vidDecCtx.PacketsLock.TryEnter())
		{
			Sleep(1);
			continue;
		}
		if (m_vidDecCtx.Packets.size() == 0)
		{
			m_vidDecCtx.PacketsLock.Leave();		
			Sleep(1);
			continue;
		}
		pPacket = m_vidDecCtx.Packets.front();
		m_vidDecCtx.Packets.pop_front();
		m_vidDecCtx.PacketsLock.Leave();

		pFrame = avcodec_alloc_frame();
		ASSERT(pFrame != NULL);
		nBytesUsed = avcodec_decode_video2(m_vidDecCtx.Stream->codec, pFrame, &nGotPicture, pPacket);
		if (nGotPicture > 0)
		{
			TRACE("%I64d, %I64d\n", pPacket->pts, pPacket->duration);
			ConvertAndAdd(pFrame);
		}
		else
			av_free(pFrame);
		av_free_packet(pPacket);
		SAFE_DELETE(pPacket);
	}
}

void CFFPlayer::ConvertAndAdd( AVFrame* pFrame )
{
	m_vidDecCtx.FramesLock.Enter();
	m_vidDecCtx.Frames.push_back(pFrame);
	m_vidDecCtx.FramesLock.Leave();
}

void CFFPlayer::DecodeAudio( bool& bTerminated )
{
	/*while (!bTerminated)
	{
		AVPacket* pPacket;
		int nBytesUsed = 0;

		if (!m_audDecCtx.PacketsLock.TryEnter())
		{
			Sleep(1);
			continue;
		}
		if (m_audDecCtx.Packets.size() == 0)
		{
			m_audDecCtx.PacketsLock.Leave();		
			Sleep(1);
			continue;
		}
		pPacket = m_audDecCtx.Packets.front();
		m_audDecCtx.Packets.pop_front();
		m_audDecCtx.PacketsLock.Leave();

		ASSERT(pFrame != NULL);
		nBytesUsed = avcodec_decode_audio3(m_vidDecCtx.Stream->codec, pFrame, &nGotPicture, pPacket);
		if (nGotPicture > 0)
		{
			TRACE("%I64d, %I64d\n", pPacket->pts, pPacket->duration);
			m_audDecCtx.FramesLock.Enter();
			m_audDecCtx.Frames.push_back(pFrame);
			m_audDecCtx.FramesLock.Leave();
		}
		av_free_packet(pPacket);
		SAFE_DELETE(pPacket);
	}*/
}