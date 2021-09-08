#pragma once
#ifndef _MYITERATOR_H_
#define _MYITERATOR_H_
#include "my_iterator_traits.h"
namespace MyIterator
{
	template <typename Iterator>
	class reverse_iterator //这是适配器 迭代器的
	{
		//当前迭代器的类型
		Iterator current;
	public:

		//萃取机 萃取类型
		using iterator_category = typename MyIterator_traits::iterator_traits<Iterator>::iterator_category;
		using value_type		= typename MyIterator_traits::iterator_traits<Iterator>::value_type;
		using difference_type	= typename MyIterator_traits::iterator_traits<Iterator>::difference_type;
		using reference			= typename MyIterator_traits::iterator_traits<Iterator>::reference;
		using pointer			= typename MyIterator_traits::iterator_traits<Iterator>::pointer;
		
		using iterator_type		= Iterator; //正向迭代器
		using _Self				= reverse_iterator<Iterator>;//逆向迭代器

		reverse_iterator() = default; 
		explicit reverse_iterator(iterator_type __x) : current(__x) {}
		reverse_iterator(const _Self& __x) :current(__x.current) {};
		
		//return current value
		iterator_type base()const { return current; };

		//这里的--是针对你传进来的是end() 时刻记住！！end()是指向未知区域的 所以一定要--才能拿出来最后一个元素的数据
		reference operator*()const 
		{
			Iterator temp = current;
			return *--temp;
		}

		const value_type operator*()
		{
			Iterator temp = current;
			return *--temp;
		}

		pointer operator->() const
		{
			return &(operator*());
		}
		_Self& operator++()		//such as:  ++ ++ ++i
		{
			--current;
			return *this;
		}
		_Self operator++(int)		//such as:  i++
		{
			_Self _temp = *this;
			--current;
			return _temp;
		}
		_Self& operator--()		//such as:  -- -- --i
		{
			++current;
			return *this;
		}
		_Self operator--(int)		//such as:  i--
		{
			_Self _temp = *this;
			++current;
			return _temp;
		}
		_Self operator+(difference_type n)const
		{
			return _Self(current - n);
		}
		_Self operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}
		_Self operator-=(difference_type n)
		{
			current += n;
			return *this;
		}

		reference operator[](difference_type n)const
		{
			return *(*this + n);
		}
		static void myiteratortest()
		{
			std::cout <<"my_iterator (value_type)		萃取的类型"  << typeid(value_type).name()<<"\n";
			std::cout << "my_iterator(difference_type) 萃取的类型" << typeid(difference_type).name()<<"\n";
			std::cout << "my_iterator(reference)		萃取的类型" << typeid(reference).name()<<"\n";
			std::cout << "my_iterator(pointer)		    萃取的类型" << typeid(pointer).name()<<"\n";
			std::cout << "my_iterator(iterator_category)萃取的类型" << typeid(iterator_category).name()<<"\n";
		}
	};
}
#endif