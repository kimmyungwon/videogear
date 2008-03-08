#pragma once 

#ifdef _DEBUG
	#ifdef UNICODE
		#define Trace TraceW
	#else
		#define Trace TraceA
	#endif
#else
	#define Trace __noop
#endif // _DEBUG


static void TraceA( LPCSTR _Msg, ... )
{
	CStringA strMsg;
	va_list args;

	va_start(args, _Msg);
	strMsg.FormatV(_Msg, args);
	va_end(args);
	OutputDebugStringA(strMsg + "\n");
}

static void TraceW( LPCWSTR _Msg, ... )
{
	CStringW strMsg;
	va_list args;

	va_start(args, _Msg);
	strMsg.FormatV(_Msg, args);
	va_end(args);
	OutputDebugStringW(strMsg + L"\n");
}