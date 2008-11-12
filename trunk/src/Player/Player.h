//---------------------------------------------------------------------------

#ifndef PlayerH
#define PlayerH

#include <list>
#include "FGManager.h"

using namespace std;

class TPlayer
{
public:
	enum { stUninit, stReady };

	TPlayer(void);
	virtual ~TPlayer(void);
	// ��ʼ��������
	HRESULT Initialize(HWND hVidWnd);
	// �ͷŲ�����
	HRESULT Uninitialize(void);
private:
    int m_iState;
	HWND m_hVidWnd;
	CComPtr<IGraphBuilder2>	m_pGB;
	CComQIPtr<IMediaEvent> m_pME;
};

//---------------------------------------------------------------------------
#endif
