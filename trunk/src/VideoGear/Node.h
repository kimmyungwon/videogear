#pragma once

enum PinDirection {
	PDIR_INPUT,
	PDIR_OUTPUT
};

enum MediaType{
	MTYPE_UNKNOWN = -1,
	MTYPE_VIDEO,
	MTYPE_AUDIO,
	MTYPE_SUBTITLE
};

class CPin
{
	friend class CNode;
public:
	CPin(CNode* pOwner, PinDirection dir, MediaType mtMajor, MediaType mtSub);
	virtual ~CPin(void) {}
	CNode* GetNode(void)	{ return m_pOwner; }
	PinDirection GetDirection(void)	{ return m_dir; }
	HRESULT Connect(CPin* pPin);
	HRESULT Disconnect(void);
private:
	CNode* m_pOwner;
	PinDirection m_dir;
	MediaType m_mtMajor, m_mtSub;
	CPin* m_pConnected;
};

class CNode
{
	friend class CNode;
public:
	CNode(void);
	virtual ~CNode(void);
	UINT GetPinCount(void);
	HRESULT GetPin(UINT nIndex, CPin*& pPin);
protected:
	void AddPin(CPin* pPin);
	HRESULT RemovePin(const CPin* pPin);
	HRESULT RemovePin(UINT nIndex);
	HRESULT RemoveAllPins(void);
	/* жиди */
	virtual HRESULT CheckInput(CPin* pPinIn) = 0;
	virtual HRESULT CompleteConnect(PinDirection dir, CPin* pPinIn) = 0;
private:
	typedef boost::ptr_vector<CPin> PinList;

	PinList m_pins;
};
