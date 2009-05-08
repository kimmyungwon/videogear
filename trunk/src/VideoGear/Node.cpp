#include "StdAfx.h"
#include "Node.h"
#include "VideoGear.h"

CPin::CPin( CNode* pOwner, PinDirection dir, const MediaType& mt )
: m_pOwner(pOwner), m_dir(dir), m_mediaType(mt), m_pConnected(NULL)
{
}

HRESULT CPin::Connect( CPin* pPin )
{
	if (pPin == NULL)
		return E_POINTER;
	if (m_pOwner == NULL)
		return E_UNEXPECTED;
	if (m_pOwner->GetState() != STATE_STOPPED)
		return VGERR_NOT_STOPPED;
	if (m_pConnected != NULL)
		return VGERR_ALREADY_CONNECTED;
	if (m_dir == pPin->GetDirection())
		return VGERR_INVALID_DIRECTION;
	if (m_pConnected != NULL)
		return VGERR_ALREADY_CONNECTED;

	CPin* pPinOut;
	CPin* pPinIn;
	CNode* pNodeOut;
	CNode* pNodeIn;
	
	if (m_dir == PDIR_INPUT)
	{
		pPinOut = pPin;
		pPinIn = this;
	}
	else
	{
		pPinOut = this;
		pPinIn = pPin;
	}
	pNodeOut = pPinOut->GetNode();
	if (pPinOut == NULL)
		return E_UNEXPECTED;
	pNodeIn = pPinIn->GetNode();
	if (pPinIn == NULL)
		return E_UNEXPECTED;
	RIF(pNodeIn->CheckInput(pPinOut));
	RIF(pNodeOut->CompleteConnect(pPinOut, pPinIn));
	pPinOut->m_pConnected = pPinIn;
	RIF(pNodeIn->CompleteConnect(pPinIn, pPinOut));
	pPinIn->m_pConnected = pPinOut;
	return S_OK;
}

HRESULT CPin::Disconnect( void )
{
	if (m_pOwner == NULL)
		return E_UNEXPECTED;
	if (m_pOwner->GetState() != STATE_STOPPED)
		return VGERR_NOT_STOPPED;
	if (m_pConnected != NULL)
	{
		CNode* pNodeRecv;

		pNodeRecv = m_pConnected->GetNode();
		if (pNodeRecv == NULL)
			return E_UNEXPECTED;
		RIF(pNodeRecv->BreakConnect(m_pConnected));
		m_pConnected->m_pConnected = NULL;
		RIF(m_pOwner->BreakConnect(this));
		m_pConnected = NULL;
	}
	return S_OK;
}

HRESULT CPin::Deliver( CPacket* pPacket )
{
	if (pPacket == NULL)
		return E_POINTER;
	if (m_dir != PDIR_OUTPUT)
		return VGERR_NOT_SUPPORTED;
	if (m_pConnected == NULL)
		return VGERR_NOT_CONNECTED;
	return m_pConnected->Receive(pPacket);
}

HRESULT CPin::Receive( CPacket* pPacket )
{
	return E_NOTIMPL;
}
//////////////////////////////////////////////////////////////////////////

CNode::CNode(void)
: m_state(STATE_STOPPED)
{
}

CNode::~CNode(void)
{
}

CPin* CNode::GetPin( UINT nIndex )
{
	return nIndex < m_pins.size() ? &m_pins[nIndex] : NULL;
}

HRESULT CNode::Run( void )
{
	if (m_state != STATE_RUNNING)
	{	
		RIF(DoRun());
		m_state = STATE_RUNNING;
	}
	return S_OK;
}

HRESULT CNode::Stop( void )
{
	if (m_state != STATE_STOPPED)
	{	
		RIF(DoStop());
		m_state = STATE_STOPPED;
	}
	return S_OK;	
}

HRESULT CNode::Pause( void )
{
	if (m_state == STATE_RUNNING)
	{	
		RIF(DoPause());
		m_state = STATE_PAUSED;
		return S_OK;
	}
	else
		return E_FAIL;
}

void CNode::AddPin( CPin* pPin )
{
	ASSERT_POINTER(pPin, CPin);
	m_pins.push_back(pPin);
}

HRESULT CNode::RemovePin( const CPin* pPin )
{
	for (UINT i = 0; i < m_pins.size(); i++)
	{
		if (&m_pins[i] == pPin)
			return RemovePin(i);
	}
	return E_INVALIDARG;
}

HRESULT CNode::RemovePin( UINT nIndex )
{
	if (nIndex < m_pins.size())
	{
		RIF(m_pins[nIndex].Disconnect());
		m_pins.erase(m_pins.begin() + nIndex);
		return S_OK;
	}
	else
		return E_INVALIDARG;
}

HRESULT CNode::RemoveAllPins( void )
{
	while (m_pins.size() > 0)
	{
		RIF(RemovePin((UINT)0));
	}		
	return S_OK;
}





