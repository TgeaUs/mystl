#pragma once
#ifndef _MY_STACK_H_
#define _MY_STACK_H_
#include "my_list.h"
namespace MyStack
{
	template <typename T,typename container =MyList::list<T>>
	class stack
	{

	protected:
		container se;
	public:

		typedef typename container::value_type      value_type;
		typedef typename container::size_type       size_type;
		typedef typename container::reference       reference;

		stack() :se() {};
	
		void push(const value_type& x) { se.push_back(x); }
		void pop() { se.pop_back(); }
		value_type size() {return se.size(); }
		reference top() { return se.back(); }
		bool empty() const { return se.empty(); }
	};
}
#endif