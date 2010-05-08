#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

#define VG_NAMESPACE_BEGIN	\
	namespace VideoGear { \
		using namespace std; \
		using namespace boost;

#define VG_NAMESPACE_END	}