#pragma once

#include "vgConfig.hpp"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/noncopyable.hpp>