//---------------------------------------------------------------------------

#ifndef PlayerH
#define PlayerH

#include <vector>
#include "ThreadUtils.h"
#include "FGManager.h"

using namespace std;

struct TElement
{
    String  strPath;
};

class TPlayList : public vector<TElement>, public TCritSec
{
public:
	TPlayList(void): cur_pos(0)	{}

	size_t   cur_pos;
};

class TPlayer
{
	friend int __fastcall EventThreadProc(void*);
public:
	enum { stUninit, stStopped, stPlaying };
	enum { vmDefault };

	TPlayer(void);
	virtual ~TPlayer(void);
	// ��ʼ��������
	HRESULT Initialize(TWinControl* pVidWnd);
	// �ͷŲ�����
	HRESULT Uninitialize(void);
	// ��ӵ������б�
	void AddToPlaylist(TElement& elem);
	// ��ʼ����
    void Play(void);
protected:
	// FilterGraph��Ϣ��Ӧ
	void OnGraphEvent(long lCode, LONG_PTR lParam1, LONG_PTR lParam2);
	// ��Ƶ������Ϣ����
    void __fastcall OnVidWndMsg(TMessage &Message);
private:
    int m_iState;
	TWinControl* m_pVidWnd;
	TWndMethod m_pVidWndOldWndProc;
    HANDLE m_hEventThread;
	CComPtr<IGraphBuilder2>	m_pGB;
	CComQIPtr<IMediaControl> m_pMC;
	CComQIPtr<IMediaEventEx> m_pME;
	CComQIPtr<IVideoWindow> m_pVW;
	TPlayList m_playlist;
};

//---------------------------------------------------------------------------
#endif
