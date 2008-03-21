// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <list>
#include <vector>
#include <map>
#include <set>
#include <hash_set>
#include <utility>
#include <ObjBase.h>
#include <dshow.h>
#include <streams.h>
#include <dvdmedia.h>
#include <comdef.h>
#include <InitGuid.h>
#include "../common/guliverkli/include/moreuuids.h"
#include "../common/guliverkli/src/dsutil/DSUtil.h"
#include "../common/guliverkli/src/dsutil/MediaTypes.h"
#include "../common/guliverkli/src/filters/parser/realmediasplitter/RealMediaSplitter.h"
#include "../common/guliverkli/src/filters/parser/matroskasplitter/MatroskaSplitter.h"
#include "../common/guliverkli/src/filters/switcher/audioswitcher/AudioSwitcher.h"