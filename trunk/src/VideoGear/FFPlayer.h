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
	/* 渲染指定的文件 */
	HRESULT RenderFile(LPCTSTR pszFile);
	/* 播放/恢复 */
	HRESULT Play(void);
	/* 停止播放 */
	HRESULT Stop(void);
protected:
	/* 清理播放器环境 */
	HRESULT Clean(void);
	/* 渲染指定的媒体源 */
	HRESULT RenderSource(void);
	/* 渲染指定的该媒体的所有输出 */
	HRESULT RenderOutputs(AVInputFormat* pInFmt);
	/* 数据拆解 */
	void Demuxe(bool& bTerminated);
	/* 视频解码 */
	void DecodeVideo(bool& bTerminated);
	/* 色彩空间转换 */
	void ConvertAndAdd(AVFrame* pFrame);
	/* 音频解码 */
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
