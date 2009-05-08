#pragma once

#include "Source.h"
#include "Thread.h"

enum PlayerState
{
	PS_IDLE,
	PS_STOPPED,
	PS_RUNNING,
	PS_PAUSED
};

class CFFPlayer;

class CFFPlayer
{
	friend class CFFDemuxer;
	friend class CFFVideoDecoder;
	friend class CFFAudioDecoder;
public:
	CFFPlayer(void);
	virtual ~CFFPlayer(void);
	/* ��Ⱦָ�����ļ� */
	HRESULT RenderFile(LPCTSTR pszFile);
	/* ����/�ָ� */
	HRESULT Play(void);
	/* ֹͣ���� */
	HRESULT Stop(void);
protected:
	/* ������������ */
	HRESULT Clean(void);
	/* ��Ⱦָ����ý��Դ */
	HRESULT RenderSource(void);
	/* ��Ⱦָ���ĸ�ý���������� */
	HRESULT RenderOutputs(AVInputFormat* pInFmt);
	/* ���ݲ�� */
	void Demuxe(bool& bTerminated);
	/* ��Ƶ���� */
	void DecodeVideo(bool& bTerminated);
	/* ɫ�ʿռ�ת�� */
	void ConvertAndAdd(AVFrame* pFrame);
	/* ��Ƶ���� */
	void DecodeAudio(bool& bTerminated);
private:
	typedef std::list<AVPacket*> PacketQueue;
	typedef std::list<AVFrame*> FrameQueue;
	struct DecoderContext
	{
		AVStream* Stream;
		CThread* Thread;
		PacketQueue Packets;
		CCritSec PacketsLock;
		FrameQueue Frames;
		CCritSec FramesLock;
	};

	PlayerState m_State;
	CSource* m_pSource; 
	ByteIOContext* m_pffIOCtx;
	AVFormatContext* m_pffFmtCtx;
	SwsContext* m_pSwsCtx;
	CThread* m_pDemuxerThread;
	DecoderContext m_vidDecCtx;
	DecoderContext m_audDecCtx;
};
