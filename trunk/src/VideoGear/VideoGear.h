// VideoGear.h : VideoGear DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoGearApp
// �йش���ʵ�ֵ���Ϣ������� VideoGear.cpp
//

class CVideoGearApp : public CWinApp
{
public:
	CVideoGearApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
