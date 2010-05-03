// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define __STDC_CONSTANT_MACROS
#include <stdint.h>

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dshow.h>
#include <streams.h>

extern "C" {
#include "libavcodec/avcodec.h"
};
