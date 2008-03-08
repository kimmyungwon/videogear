#pragma once

#include <vector>

using namespace std;

template<typename NameT, typename ValueT>
struct AriaTreeNode
{
	typedef vector<AriaTreeNode> children_t; 
	
	NameT Name;
	ValueT Value; 
	children_t Children;
};