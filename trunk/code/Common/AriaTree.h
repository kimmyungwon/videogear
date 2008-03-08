#pragma once

#include <vector>

template<typename NameT, typename ValueT>
class CAriaTreeNode;

template<typename NameT, typename ValueT>
class CAriaTree
{
public:
	typedef CAriaTreeNode<typename NameT, typename ValueT> node_t;
	typedef std::vector<node_t> nodes_t;

	typename nodes_t::const_iterator begin() const	{ return m_Roots.begin(); }
	typename nodes_t::const_iterator end() const	{ return m_Roots.end(); }
private:
	nodes_t		m_Roots;
};

template<typename NameT, typename ValueT>
class CAriaTreeNode
{
	template<typename NameT, typename ValueT> friend class CAriaTree;
public:
	typedef CAriaTree::nodes_t children_t; 

	typename NameT GetName() const	{ return m_Name; }
	typename ValueT GetValue() const	{ return m_Value; }
	typename children_t::const_iterator begin() const	{ return m_Children.begin(); }
	typename children_t::const_iterator end() const	{ return m_Children.end(); }
protected:
	CAriaTreeNode( NameT _Name, ValueT _Value ) : m_Name(_Name), m_Value(_Value)	{}
private:
	NameT			m_Name;
	ValueT			m_Value; 
	children_t		m_Children;
};