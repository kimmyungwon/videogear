#pragma once

#include "Defines.h"
#include "Packet.h"

enum PinDirection {
	PDIR_INPUT,
	PDIR_OUTPUT
};

enum MediaType{
	MTYPE_NONE,
	MTYPE_STREAM,
	MTYPE_VIDEO,
	MTYPE_AUDIO,
	MTYPE_SUBTITLE
};

enum MediaSubType
{
	MSTYPE_NONE
};	

class CPin
{
	friend class CNode;
public:
	CPin(CNode* pOwner, PinDirection dir, MediaType mtMajor, MediaSubType mtSub);
	virtual ~CPin(void) {}
	CNode* GetNode(void)	{ return m_pOwner; }
	PinDirection GetDirection(void)	{ return m_dir; }
	HRESULT Connect(CPin* pPin);
	CPin* ConnectedTo(void)	{ return m_pConnected; }
	HRESULT Disconnect(void);
	virtual HRESULT Deliver(CPacket* pPacket);
	virtual HRESULT Receive(CPacket* pPacket);
protected:
	CNode* m_pOwner;
	PinDirection m_dir;
	MediaType m_mtMajor;
	MediaSubType m_mtSub;
	CPin* m_pConnected;
};

class CNode
{
	friend class CPin;
public:
	CNode(void);
	virtual ~CNode(void);
	UINT GetPinCount(void)	{ return m_pins.size(); }
	CPin* GetPin(UINT nIndex);
protected:
	void AddPin(CPin* pPin);
	HRESULT RemovePin(const CPin* pPin);
	HRESULT RemovePin(UINT nIndex);
	HRESULT RemoveAllPins(void);
	/* жиди */
	virtual HRESULT CheckInput(CPin* pPinIn) = 0;
	virtual HRESULT CompleteConnect(CPin* pPin, CPin* pPinRecv) = 0;
	virtual HRESULT BreakConnect(CPin* pPin) = 0;
private:
	typedef boost::ptr_vector<CPin> PinList;

	PinList m_pins;
};
