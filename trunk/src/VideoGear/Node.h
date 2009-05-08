#pragma once

#include "Defines.h"
#include "Packet.h"

enum PinDirection {
	PDIR_INPUT,
	PDIR_OUTPUT
};

enum {
	MTYPE_NONE,
	MTYPE_STREAM,
	MTYPE_VIDEO,
	MTYPE_AUDIO,
	MTYPE_SUBTITLE,
	MTYPE_FFMPEG = 0x80000000
};	

struct MediaType
{
	int major;
	int minor;

	MediaType(void)
		: major(MTYPE_NONE), minor(MTYPE_NONE)
	{}
	
	MediaType(int nMajor, int nMinor)
		: major(nMajor), minor(nMinor)
	{}

	bool operator!=(const MediaType& rhs) const
	{
		return (major != rhs.major) || (minor != rhs.minor);
	}
};

class CPin
{
	friend class CNode;
public:
	CPin(CNode* pOwner, PinDirection dir, const MediaType& mt = MediaType(MTYPE_NONE, MTYPE_NONE));
	virtual ~CPin(void) {}
	CNode* GetNode(void)	{ return m_pOwner; }
	PinDirection GetDirection(void)	{ return m_dir; }
	const MediaType& GetMediaType(void) const	{ return m_mediaType; }
	void SetMediaType(const MediaType& mt)	{ m_mediaType = mt; }
	HRESULT Connect(CPin* pPin);
	CPin* ConnectedTo(void)	{ return m_pConnected; }
	HRESULT Disconnect(void);
	virtual HRESULT Deliver(CPacket* pPacket);
	virtual HRESULT Receive(CPacket* pPacket);
protected:
	CNode* m_pOwner;
	PinDirection m_dir;
	MediaType m_mediaType;
	CPin* m_pConnected;
};

enum NodeState
{	
	STATE_RUNNING,
	STATE_STOPPED,
	STATE_PAUSED
};

class CNode
{
	friend class CPin;
public:	
	CNode(void);
	virtual ~CNode(void);
	UINT GetPinCount(void)	{ return m_pins.size(); }
	CPin* GetPin(UINT nIndex);
	NodeState GetState(void) const	{ return m_state; }
	virtual HRESULT Run(void);
	virtual HRESULT Stop(void);
	virtual HRESULT Pause(void);
protected:
	void AddPin(CPin* pPin);
	HRESULT RemovePin(const CPin* pPin);
	HRESULT RemovePin(UINT nIndex);
	HRESULT RemoveAllPins(void);
	/* жиди */
	virtual HRESULT CheckInput(CPin* pPinIn) = 0;
	virtual HRESULT CompleteConnect(CPin* pPin, CPin* pPinRecv) = 0;
	virtual HRESULT BreakConnect(CPin* pPin) = 0;
	virtual HRESULT DoRun(void) = 0;
	virtual HRESULT DoStop(void) = 0;
	virtual HRESULT DoPause(void) = 0;
private:
	typedef boost::ptr_vector<CPin> PinList;

	PinList m_pins;
	NodeState m_state;
};
