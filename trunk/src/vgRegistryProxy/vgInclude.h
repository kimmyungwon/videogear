#pragma once

#include "vgCommon/vgInclude.h"

#include <cassert>
#include <map>
#include <string>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>