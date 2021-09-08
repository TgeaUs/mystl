#pragma once
#ifndef _MYITERATOR_H_
#define _MYITERATOR_H_
#include "my_iterator_traits.h"
namespace MyIterator
{
	template <typename Iterator>
	class reverse_iterator //���������� ��������
	{
		//��ǰ������������
		Iterator current;
	public:

		//��ȡ�� ��ȡ����
		using iterator_category = typename MyIterator_traits::iterator_traits<Iterator>::iterator_category;
		using value_type		= typename MyIterator_traits::iterator_traits<Iterator>::value_type;
		using difference_type	= typename MyIterator_traits::iterator_traits<Iterator>::difference_type;
		using reference			= typename MyIterator_traits::iterator_traits<Iterator>::reference;
		using pointer			= typename MyIterator_traits::iterator_traits<Iterator>::pointer;
		
		using iterator_type		= Iterator; //���������
		using _Self				= reverse_iterator<Iterator>;//���������

		reverse_iterator() = default; 
		explicit reverse_iterator(iterator_type __x) : current(__x) {}
		reverse_iterator(const _Self& __x) :current(__x.current) {};
		
		//return current value
		iterator_type base()const { return current; };

		//�����--������㴫��������end() ʱ�̼�ס����end()��ָ��δ֪����� ����һ��Ҫ--�����ó������һ��Ԫ�ص�����
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
			std::cout <<"my_iterator (value_type)		��ȡ������"  << typeid(value_type).name()<<"\n";
			std::cout << "my_iterator(difference_type) ��ȡ������" << typeid(difference_type).name()<<"\n";
			std::cout << "my_iterator(reference)		��ȡ������" << typeid(reference).name()<<"\n";
			std::cout << "my_iterator(pointer)		    ��ȡ������" << typeid(pointer).name()<<"\n";
			std::cout << "my_iterator(iterator_category)��ȡ������" << typeid(iterator_category).name()<<"\n";
		}
	};
}
#endif