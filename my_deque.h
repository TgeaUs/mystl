#pragma once
#ifndef _MY_DEQUE_H_
#define _MY_DEQUE_H_
#include "my_allocator.h"
#include "my_uninitialized.h"
namespace MyDeque
{
	inline size_t deque_buf_size(size_t n, size_t size)
	{
		//n!=0 ��ôn���ǻ������Ĵ�С
		//n=0��n<512 ��ô���� 512/sizeof(size)��С

		//��Ϊ̫���� ÿ��������ֻ��Ҫһ��Ԫ�ؾ�����
		//n=0��n>512 ��ô����1
		return n != 0 ? n : (size < 512 ? size_t(512 / size) : size_t(1));
	}
	//buffer�Ĵ�С
	template <typename _Tp, size_t buffersize = 0>
	class Deque_iterator
	{
	public:		
		using iterator_category = MyIterator_traits::random_access_iterator_tag;
		using value_type		= _Tp;		//��ʾ�����������������
		using reference			= value_type&;		
		using pointer			= value_type*;		
		using size_type			= size_t;
		using difference_type	= ptrdiff_t;	//��ʾ���� ����������		
		using Self				= Deque_iterator<_Tp, buffersize>;

		_Tp** map_node;		//��Զָ�� ��ǰ��map
		_Tp* cur;			//��Զָ��ǰbuffer��Ԫ��
		_Tp* first;				//��Զָ�� ��ǰbuffer��ͷ ��������߽磩
		_Tp* last;				//��Զָ�� ��ǰbuffer��β�ͣ�������߽磩
	public:
		Deque_iterator() : cur(0), first(0), last(0), map_node(0) {}
		void set_node(_Tp** new_node)
		{
			map_node = new_node; //��ǰmap����Ľڵ�
			first = *new_node; //����first
			last = first + difference_type(deque_buf_size(buffersize, sizeof(_Tp))); //����last
		}
		static size_t buffer_size()
		{
			return deque_buf_size(buffersize, sizeof(_Tp));
		}
		reference operator*()const
		{
			return *cur;
		}
		Self& operator++() {
			++cur;
			if (cur == last) {
				set_node(map_node + 1);
				cur = first;
			}
			return *this;
		}
		Self operator++(int) {
			Self __tmp = *this;
			++* this;
			return __tmp;
		}
		Self& operator--() {
			if (cur == first) {
				set_node(map_node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		bool operator!=(const Self& __x) const { return !(*this == __x); }
		bool operator<(const Self& __x) const {
			return (map_node == __x.map_node) ?
				(cur < __x.cur) : (map_node < __x.map_node);
		}
		bool operator==(const Self& x)const {
			return x.first == x.last;
		}
		difference_type operator-(const Self& x) const 
		{
			//std::cout << buffer_size();
			return difference_type(buffer_size()) * (map_node - x.map_node - 1) + (cur - first) + (x.last - x.cur);
		}

		Self& operator+=(difference_type __n)
		{
			difference_type __offset = __n + (cur - first);
			if (__offset >= 0 && __offset < difference_type(buffer_size()))
				cur += __n;
			else {
				difference_type __node_offset =
					__offset > 0 ? __offset / difference_type(buffer_size())
					: -difference_type((-__offset - 1) / buffer_size()) - 1;
				set_node(map_node + __node_offset);
				cur = first +
					(__offset - __node_offset * difference_type(buffer_size()));
			}
			return *this;
		}
		Self operator+(difference_type n)const
		{
			Self temp = *this;
			return temp += n;
		}
	};

	template<typename _Tp, size_t buffersize = 0, typename allocator = MyAllocator::allocator<_Tp>>
	class deque
	{
	public:
		using value_type = _Tp;		//��ʾ�����������������
		using reference = value_type&;
		using pointer = value_type*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;	//��ʾ���� ����������		
		using iterator = Deque_iterator<_Tp, buffersize>;
	protected:
		_Tp** map;			//ָ��ǰmap�� �ڲ���buffer
		size_t map_size;	//map�Ĵ�С
		iterator start;		//��Զָ��  map��ͷ
		iterator finish;	//��Զָ�� map��β��
		
		enum { initial_map_size = 8 };
		static size_t buffer_size()
		{
			return deque_buf_size(buffersize, sizeof(_Tp));
		}
		/// <summary>
		/// ////////////////////////////////////
		/// </summary>
		using node_alloc_type = MyAllocator::allocator<_Tp>;
		_Tp* allocate_node() //����buffer�Ĵ�С
		{
			return node_alloc_type::allocate(deque_buf_size(buffersize, sizeof(_Tp))); //����1000����
		}
		void deallocate_node(pointer p) { node_alloc_type::deallocate(p); }

		/// <summary>
		/// ////////////////////////////////////
		/// </summary>
		using map_alloc_type = MyAllocator::allocator<_Tp*>;
		_Tp** allocate_map(size_t n) //����map�Ĵ�С
		{
			return map_alloc_type::allocate(n);
		}
		void deallocate_map(_Tp** p)
		{
			map_alloc_type::deallocate(p);
		}

		void create_nodes(_Tp** newstart, _Tp** newfinish)
		{
			_Tp** cur;
			for (cur = newstart; cur < newfinish; ++cur)
			{
				//std::cout << __func__ << "()" << __LINE__ << "�� " << cur << "ѭ��";
				*cur = allocate_node();
			}
		}


		void fiil_initialize(size_t buffer,const value_type& value)
		{
			_Tp** __cur;
			for (__cur = start.map_node; __cur < finish.map_node; ++__cur)
				MyUninitialized::uninitialized_fill(*__cur, *__cur + buffer_size(), value);
			MyUninitialized::uninitialized_fill(finish.first, finish.cur, value);

		}

		void initialize_map(size_t num_elements) //��ʼ����ÿһ��data member
		{
			#if ISDIALOGUE
			//��ʱ���ŵ�map�м���
			size_t num_nodes = num_elements / deque_buf_size(buffersize, sizeof(_Tp)) + 1;
			std::cout << "num_nodes��СΪ:" << num_nodes << "\n";

			map_size = MyAlgorithm_base::max((size_t)initial_map_size, num_nodes + 2);;
			std::cout << "map_size��СΪ:"<<map_size<<"\n";

			//��һ��ָ��ķ���
			//һ��ָ��=map�Ĵ�С
			//����ָ��=map_nodeλ��
			map = allocate_map(map_size);

			//��ָ֤���м�  ����ʱ���ŵ�map����ֵ    //8
			//std::cout << "newstart�ĵ�ַΪ:" << (map_size - num_nodes) / 2 << "\n";
			_Tp** newstart = map + (map_size - num_nodes) / 2;
			//std::cout << "newfinish�ĵ�ַΪ:" << (map_size - num_nodes) / 2 + num_nodes << "\n";
			_Tp** newfinish = newstart + num_nodes;
			
			create_nodes(newstart,newfinish); //��buffer���ٿռ�

			//���ú�����Ĵ�С ��start��finish���ñ߽� ������Ծ
			start.set_node(newstart);
			finish.set_node(newfinish - 1);
			//���õ�ǰ����ĵ�һ������
			start.cur = start.first; 
			//��Զָ�� ���һ����һ��λ��
			finish.cur = finish.first + num_elements % deque_buf_size(buffersize, sizeof(_Tp));
			#else
			size_t num_nodes = num_elements / deque_buf_size(buffersize, sizeof(_Tp)) + 1;

			map_size = MyAlgorithm_base::max((size_t)initial_map_size, num_nodes + 2);;

			map = allocate_map(map_size);

			_Tp** newstart = map + (map_size - num_nodes) / 2;
			_Tp** newfinish = newstart + num_nodes;

			create_nodes(newstart, newfinish); 

			start.set_node(newstart);
			finish.set_node(newfinish - 1);
			start.cur = start.first;
			finish.cur = finish.first + num_elements % deque_buf_size(buffersize, sizeof(_Tp));

			#endif
		}
	public:

		deque(size_type n, const value_type& value) :map(0), map_size(0), start(), finish()
		{
			initialize_map(n);
			fiil_initialize(n,value);
			
		}

		reference front() { return *start; }
		reference back() {
			iterator __tmp = finish;
			--__tmp;
			return *__tmp;
		}
		iterator begin() { return start; }
		iterator end() { return finish; }
		bool empty()const { return start == finish; }
		difference_type size()const { return finish - start; }
		void push_front(const value_type& x)
		{
			if (start.cur != finish.cur)
			{
				MyConstruct::construct(start.cur - 1,x);
				--start.cur;
			}
		}
		void push_back(const value_type& x)
		{
			if (finish.cur != finish.last - 1)
			{
				MyConstruct::construct(finish.cur, x);
				++finish.cur;
			}
		}
		void pop_front()
		{
			if (start.cur != start.cur - 1)
			{
				MyConstruct::destroy(start.cur);
				++start.cur;
			}
		}

		void pop_back_aux() 
		{
			deallocate_node(finish.first);
			--finish;
			MyConstruct::destroy(finish.cur);
		}

		void pop_back() 
		{
			if (finish.cur!= finish.first) 
			{
				--finish.cur;
				MyConstruct::destroy(finish.cur);
			}
			else
			{
				pop_back_aux();
			}
		}
		iterator insert_aux(iterator pos, const value_type& x)
		{
			difference_type index = pos - start; //��ǰ��λ�ú���ʼλ��
			value_type x_copy = x;
			if (index < size() / 2)
			{
				//std::cout << "�ҵ�start:" << &(*(start.cur))<<"\n";
				push_front(front()); //û���������������
				iterator front1 = start;
				++front1; 
				//std::cout << "�ҵ�front1:" << &(*(front1.cur)) << "\n";

				iterator front2 = front1;
				++front2;
				//std::cout << "�ҵ�front2:" << &(*(front2.cur)) << "\n";

				pos = start + index;
				iterator pos1 = pos;
				++pos1;
				MyAlgorithm_base::copy(front2, pos1, front1);
			}
			else
			{
				push_back(back());
				iterator __back1 = finish;
				--__back1;
				iterator __back2 = __back1;
				--__back2;
				pos = start + index;
				MyAlgorithm_base::copy_backward(pos, __back2, __back1);
			}
			*pos = x_copy;
			return pos;
		}
		iterator insert(iterator position, const value_type& x)
		{
			if (position.cur == start.cur)
			{
				//ǰ��
				push_front(x);
				return start;
			}
			else if (position.cur == finish.cur)
			{
				//β��
				push_back(x);
				iterator temp = finish;
				--temp;
				return temp;
			}
			else
			{
				return insert_aux(position, x);
			}
			
		}

		iterator erase(iterator pos)
		{
			iterator next = pos;
			++next;
			difference_type index = pos - start;
			if (index < (size() >> 1))
			{
				MyAlgorithm_base::copy_backward(start, pos, next);
				pop_front();
			}
			else
			{
				MyAlgorithm_base::copy(next, finish, pos);
				pop_back();
			}
			return  start + index;
		}

	};
}
#endif