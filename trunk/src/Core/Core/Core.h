// Core.h : Core DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCoreApp
// �йش���ʵ�ֵ���Ϣ������� Core.cpp
//

class CCoreApp : public CWinApp
{
public:
	CCoreApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
