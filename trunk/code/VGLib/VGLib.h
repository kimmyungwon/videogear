// VGLib.h : VGLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVGLibApp
// �йش���ʵ�ֵ���Ϣ������� VGLib.cpp
//

class CVGLibApp : public CWinApp
{
public:
	CVGLibApp();
	~CVGLibApp();
	CString GetAppRoot(void)	{ return m_strAppRoot; }

private:
	CString		m_strAppRoot;

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};