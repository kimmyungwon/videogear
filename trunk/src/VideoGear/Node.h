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
	virtual ~CPin(void)	{}
	HRESULT Disconnect(void);
private:
	CNode* m_pOwner;
	PinDirection m_dir;
	MediaType m_mtMajor, m_mtSub;
	CPin* m_pConnected;
};

class CNode
{
public:
	CNode(void);
	virtual ~CNode(void);
	HRESULT GetPin(UINT nIndex, CPin*& pPin);
protected:
	void AddPin(CPin* pPin);
	HRESULT RemovePin(const CPin* pPin);
	HRESULT RemovePin(UINT nIndex);
	HRESULT RemoveAllPins(void);
private:
	typedef boost::ptr_vector<CPin> PinList;

	PinList m_pins;
};
