#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cassert>
#include <map>
#include <string>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/typeof/typeof.hpp>