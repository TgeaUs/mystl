#pragma once
#ifndef _MY_SET_H_
#define _MY_SET_H_
#include "my_allocator.h"
#include "my_functional.h"
#include "my_tree.h"
namespace MySet

{	//set �������ֵ�ظ� �������޸�
	template <typename key,typename compare=MyFunctional::less<key>,typename allocator=MyAllocator::allocator<key>>
	class set
	{
	public:
		using key_type = key;
		using value_type = key;
		using key_compare = compare;
		using value_compare = compare;
	private:
		using rep_type = MyTree::rb_tree<key_type, value_type, MyFunctional::identity<value_type>, key_compare>;
		rep_type T;  // red-black tree representing set


	public:
		using pointer			= typename rep_type::const_pointer;
		using const_pointer		= typename rep_type::const_pointer;
		using reference			= typename rep_type::const_reference;
		using const_reference	= typename rep_type::const_reference;
		using iterator			= typename rep_type::const_iterator;
		using const_iterator	= typename rep_type::const_iterator;
		using size_type			= typename rep_type::size_type;
		using difference_type	= typename rep_type::difference_type;
		using allocator_type	= typename rep_type::allocator_type;

		set() :T() {}

		//������
		const_iterator begin()const { return T.begin(); }
		const_iterator end()const { return T.end(); }
		//����
		bool empty() const { return T.empty(); }
		size_type size() const { return T.size(); }
		size_type max_size() const { return T.max_size(); }

		MyPair::pair<iterator, bool> insert(const value_type& x)
		{
			return T.insert_unique(x);

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
#endif // !_MY_SET_H_
