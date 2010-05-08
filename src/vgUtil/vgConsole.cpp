#include "vgConsole.h"

VG_NAMESPACE_BEGIN

Console& Console::GetInstance( void )
{
	static Console instance;
	return instance;
}

void Console::Print( const wchar_t *format, ... )
{
	va_list args;
	va_start(args, format);
	int length = _vscwprintf(format, args) + 1;
	wchar_t *message = (wchar_t*)calloc(length, sizeof(wchar_t));
	vswprintf_s(message, length, format, args);
	DWORD writtenChars;
	WriteConsoleW(m_outputHandle, message, length - 1, &writtenChars, NULL);
#ifdef _DEBUG
	OutputDebugStringW(message);
#endif
	free(message);
}

Console::Console( void )
{
	m_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

VG_NAMESPACE_END