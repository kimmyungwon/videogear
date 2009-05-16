#pragma once

#include "VideoGear.h"

template<typename BaseT>
class CUIControlImpl : public BaseT
{
public:
	CUIControlImpl(void)
		: m_pCanvas(NULL), m_pPainter(NULL)
	{
	
	}

	virtual ~CUIControlImpl(void)	
	{
		SAFE_DELETE(m_pPainter);
		SAFE_DELETE(m_pCanvas);
	}
protected:
	virtual void DoPaint(Graphics &canvas);	
private:
	Bitmap		*m_pCanvas;
	Graphics	*m_pPainter;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

BEGIN_TEMPLATE_MESSAGE_MAP(CUIControlImpl, BaseT, BaseT)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

template<typename BaseT>
BOOL CUIControlImpl<BaseT>::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

template<typename BaseT>
void CUIControlImpl<BaseT>::OnPaint()
{
	CPaintDC dc(this);
	if (m_pPainter != NULL)
	{
		Graphics screen(dc);
		DoPaint(*m_pPainter);
		screen.DrawImage(m_pCanvas, 0, 0);
	}
}

typedef CUIControlImpl<CWnd> CUIControl;
