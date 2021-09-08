#pragma once
#ifndef MY_MULTISET_H_
#define MY_MYLTISET_H_
#include "my_allocator.h"
#include "my_functional.h"
#include "my_tree.h"
namespace MyMultiset
{
	//multiset 允许键值重复 但并不允许修改
	template <typename key, typename compare = MyFunctional::less<key>, typename allocator = MyAllocator::allocator<key>>
	class multiset
	{
	public:
		using key_type = key;
		using value_type = key;
		using key_compare = compare;
		using value_compare = compare;
		using rep_type = MyTree::rb_tree<key_type, value_type, MyFunctional::identity<value_type>, key_compare>;
		rep_type T;  // red-black tree representing set
	public:
		typedef typename rep_type::const_pointer pointer;
		typedef typename rep_type::const_pointer const_pointer;
		typedef typename rep_type::const_reference reference;
		typedef typename rep_type::const_reference const_reference;
		typedef typename rep_type::const_iterator iterator;
		typedef typename rep_type::const_iterator const_iterator;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename rep_type::allocator_type allocator_type;
		multiset() :T() {}

				//迭代器
		const_iterator begin()const { return T.begin(); }
		const_iterator end()const { return T.end(); }
		//容量
		bool empty() const { return T.empty(); }
		size_type size() const { return T.size(); }
		size_type max_size() const { return T.max_size(); }

		iterator insert(const value_type& x)
		{
			return T.insert_equal(x);

		}
		size_type erase(const key_type& x) {
			return T.erase(x);
		}
		iterator find(const key_type& x)  { return T.find(x); }
		size_type count(const key_type& x)  {
			return T.find(x) ==T.end() ? 0 : 1;
		}
		iterator lower_bound(const value_type& x)
		{
			T.lower_bound(x);
		}
		iterator upper_bound(const value_type& x)
		{
			T.upper_bound(x);
		}
		MyPair::pair<iterator, iterator> equal_range(const value_type& x)
		{
			T.equal_range(x);
		}
	};
}
#endif // !MY_MULTISET_H_
