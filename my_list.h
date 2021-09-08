#pragma once
#ifndef _MY_LIST_H_
#define _MY_LIST_H_
#include "my_allocator.h"
#include "my_iterator_traits.h"
#include "my_construct.h"
#include "my_algorithm_base.h"
namespace MyList
{
	//Time:		 2020 8/4
	//End time:	 2020 8/6
	template <typename _Tp>
	struct  _List_node
	{
		_List_node* next;
		_List_node* prev;
		_Tp data;
	};

	//������
	template<typename _Tp>
	struct _List_iterator
	{
		using iterator_category = MyIterator_traits::bidirectional_iterator_tag;
		using value_type		= _Tp;		//��ʾ�����������������
		using difference_type	= ptrdiff_t;	//��ʾ���� ����������
		using reference			= value_type&;		// ��û��ʹ�ù�
		using pointer			= value_type*;		// ��û��ʹ�ù�
		using size_type			= size_t;
		using self				= _List_iterator<_Tp>;


		using _Node				=_List_node<_Tp> ; //ָ��ڵ�
		_Node* node;

		_List_iterator(_Node* x) :node(x) {}
		//_List_iterator(const _List_iterator<_Tp>& __x) : node(__x.node) {}

		reference operator *()
		{

			return node->data;

		}
		self& operator++()
		{
			node = node->next;
			return *this;
		}
		self operator++(int)
		{
			self temp = *this;
			++*this;
			return temp;
		}

		self& operator--()
		{
			this->node = this->node->prev;
			return *this;

		}
		self operator--(int)
		{
			self temp = *this;
			--node;
			return temp;
		}
		bool operator!=(const self& x) const { return node != x.node; }
		bool operator==(const self& x) const { return node == x.node; }

	};


	template<typename _Tp,typename allocator=MyAllocator::allocator<_Tp>>
	class list
	{
	public:
		using iterator_category		= MyIterator_traits::bidirectional_iterator_tag;
		using value_type			= _Tp;		//��ʾ�����������������
		using difference_type		= ptrdiff_t;	//��ʾ���� ����������
		using reference				= value_type&;		// ��û��ʹ�ù�
		using pointer				= value_type*;		// ��û��ʹ�ù�
		using size_type				= size_t;

		using Node				= _List_node<_Tp>; //ָ��ڵ�
		using iterator			= _List_iterator<_Tp>;
		using const_iterator	= _List_iterator<_Tp>;
		using data_allocator	= MyAllocator::allocator<Node>; //Ĭ�Ϸ�����

		using self				= list<_Tp>;
	private:
		_List_node<_Tp>* get_node() 
		{
			return data_allocator::allocate(1);
		}

		void empty_initialize()
		{
			node = get_node();
			node->next = node;
			node->prev = node;
		}

		void fill_initialize(size_type n, const _Tp& value)
		{
			empty_initialize();
			
			insert(begin(),n, value);
			
		}
	protected:
		Node* node; 


	public:
		//node�Ǹ�class
		list()
		{
			empty_initialize();
		}
		list(size_type n, const _Tp& value)
		{
			fill_initialize(n, value);
		}
		~list()
		{
			clear();
			data_allocator::deallocate(node);

		}
		iterator begin()			{ return node->next; }
		const_iterator begin()const { return node->next; }
		iterator end  ()			{ return node; }
		const_iterator end  ()const { return node; }

		reference back() { return *(--end()); }
		reference front() { return *begin(); }

		Node* create_node(const _Tp& value)
		{
			Node* p = get_node();

			MyConstruct::construct(&p->data, value);
		
			return p;
		}
		//�޸���
		iterator insert(iterator pos, const _Tp& value)
		{

			Node* temp = create_node(value);
			temp->next = pos.node;
			temp->prev = pos.node->prev;
			pos.node->prev->next = temp;
			pos.node->prev = temp;
			return temp;
		}
		void insert(iterator pos, size_type n, const _Tp& value)
		{	
			for (; n > 0; --n)
				insert(pos, value);
		}
		void push_back(const _Tp& value) //
		{
			insert(end(), value);
		}
		void push_front(const _Tp& value) //
		{
			insert(begin(), value);
		}
		iterator erase(iterator pos)
		{
			Node* next_node = pos.node->next;

			pos.node->prev->next = next_node;
			next_node->prev = pos.node->prev;
			MyConstruct::destroy(&pos.node->data);
			data_allocator::deallocate(pos.node);
			return static_cast<iterator>(next_node);
		}
		iterator erase(iterator first,iterator last)
		{
			while (first != last)
			{
				erase(first++);
			}
			return last;
		}
		void pop_back()
		{
			erase(--end());
		}
		void pop_front()
		{
			erase(begin());
		}
		void clear()
		{
			erase(begin(), end());
		}  
		void swap(self& other) { MyAlgorithm_base::swap(node, other.node); }


		//����
		bool empty()const { return begin() == end(); }
		size_type size()const
		{
			size_type __result = 0;	
			MyAlgorithm_base::distance(begin(), end(), __result);
			return __result;
		}
		size_type max_size()const
		{
			return size_type (- 1);
		}


		//����
		void unique()
		{
			iterator first = begin();
			iterator last = end();
			if (first == last)return;
			iterator next = first;
			while (++next != last)
			{
				if (*first == *next)
					erase(next);
				else				
					first = next;
				next = first;		
			}
		}

		void transfer(iterator position, iterator first, iterator last)
		{
			last.node->prev->next = position.node;
			first.node->prev->next = last.node;
			position.node->prev->next = first.node;

			Node* tmp = position.node->prev;
			position.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = tmp;

		}


		void reverse()
		{
			//���������0����1 �Ͳ����κ�����
			if (node != node->next && node != node->next->next)
			{
				iterator first = begin();
				++first;
				while (first != end())
				{
					iterator old = first;
					++first;
					transfer(begin(), old, first);

				}
			}
		}
		//pos��ǰ����λ�� other�������λ��
		void splice(const_iterator pos, list& other)
		{
			if (!other.empty())
			{
				transfer(pos, other.begin(), other.end());

			}
		}
		void splice(iterator pos, list&, iterator first, iterator last) 
		{
			if (first != last)
				transfer(pos, first, last);
		}
		void splice(iterator pos, self&, iterator x)
		{
			iterator next = x;
			++next;
			if (pos == next || pos == x)return;
			transfer(pos, x, next);

		}

		void merge(list& other)
		{
			iterator first1		= begin();
			iterator last1		= end();
			iterator first2		= other.begin();
			iterator last2		= other.end();
			iterator next1		= first1;
			iterator next2		= first2;
			while (first1 != last1 && first2 != last2 ) //������һ��Ԫ��
				if (*first2 < *first1 ) //�����ϲ���һ��ҪС�ڱ��� Ϊ����������
				{
					iterator next = first2;
					transfer(first1, first2, ++next);
					first2 = next;
					++next1;
				}
				++first1;			
			if (first2 != last2) transfer(last1, first2, last2);
				
			
		}

	
	
	};
}

#endif // !_MY_LIST_H_
