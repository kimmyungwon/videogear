#include "StdAfx.h"
#include "Node.h"
#include "VideoGear.h"

CPin::CPin( CNode* pOwner, PinDirection dir, MediaType mtMajor, MediaType mtSub )
: m_pOwner(pOwner), m_dir(dir), m_mtMajor(mtMajor), m_mtSub(m_mtSub), m_pConnected(NULL)
{

}

HRESULT CPin::Disconnect( void )
{
	return E_NOTIMPL;
}
//////////////////////////////////////////////////////////////////////////

CNode::CNode(void)
{
}

CNode::~CNode(void)
{
}

HRESULT CNode::GetPin( UINT nIndex, CPin*& pPin )
{
	if (nIndex < m_pins.size())
	{
		pPin = &m_pins[nIndex];
		return S_OK;
	}
	else
	{
		pPin = NULL;
		return E_FAIL;
	}
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
		m_pins.erase(m_pins.erase(m_pins.begin() + nIndex));
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

