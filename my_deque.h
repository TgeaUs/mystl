#pragma once
#ifndef _MY_DEQUE_H_
#define _MY_DEQUE_H_
#include "my_allocator.h"
#include "my_uninitialized.h"
namespace MyDeque
{
	inline size_t deque_buf_size(size_t n, size_t size)
	{
		//n!=0 那么n就是缓冲区的大小
		//n=0且n<512 那么就是 512/sizeof(size)大小

		//因为太大了 每个缓冲区只需要一个元素就行了
		//n=0且n>512 那么就是1
		return n != 0 ? n : (size < 512 ? size_t(512 / size) : size_t(1));
	}
	//buffer的大小
	template <typename _Tp, size_t buffersize = 0>
	class Deque_iterator
	{
	public:		
		using iterator_category = MyIterator_traits::random_access_iterator_tag;
		using value_type		= _Tp;		//表示你这个迭代器的类型
		using reference			= value_type&;		
		using pointer			= value_type*;		
		using size_type			= size_t;
		using difference_type	= ptrdiff_t;	//表示距离 两个迭代器		
		using Self				= Deque_iterator<_Tp, buffersize>;

		_Tp** map_node;		//永远指向 当前的map
		_Tp* cur;			//永远指向当前buffer的元素
		_Tp* first;				//永远指向 当前buffer的头 （主用与边界）
		_Tp* last;				//永远指向 当前buffer的尾巴（主用与边界）
	public:
		Deque_iterator() : cur(0), first(0), last(0), map_node(0) {}
		void set_node(_Tp** new_node)
		{
			map_node = new_node; //当前map里面的节点
			first = *new_node; //设置first
			last = first + difference_type(deque_buf_size(buffersize, sizeof(_Tp))); //设置last
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
		using value_type = _Tp;		//表示你这个迭代器的类型
		using reference = value_type&;
		using pointer = value_type*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;	//表示距离 两个迭代器		
		using iterator = Deque_iterator<_Tp, buffersize>;
	protected:
		_Tp** map;			//指向当前map和 内部的buffer
		size_t map_size;	//map的大小
		iterator start;		//永远指向  map的头
		iterator finish;	//永远指向 map的尾巴
		
		enum { initial_map_size = 8 };
		static size_t buffer_size()
		{
			return deque_buf_size(buffersize, sizeof(_Tp));
		}
		/// <summary>
		/// ////////////////////////////////////
		/// </summary>
		using node_alloc_type = MyAllocator::allocator<_Tp>;
		_Tp* allocate_node() //分配buffer的大小
		{
			return node_alloc_type::allocate(deque_buf_size(buffersize, sizeof(_Tp))); //暂用1000测试
		}
		void deallocate_node(pointer p) { node_alloc_type::deallocate(p); }

		/// <summary>
		/// ////////////////////////////////////
		/// </summary>
		using map_alloc_type = MyAllocator::allocator<_Tp*>;
		_Tp** allocate_map(size_t n) //分配map的大小
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
				//std::cout << __func__ << "()" << __LINE__ << "行 " << cur << "循环";
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

		void initialize_map(size_t num_elements) //初始化好每一个data member
		{
			#if ISDIALOGUE
			//暂时开放的map有几个
			size_t num_nodes = num_elements / deque_buf_size(buffersize, sizeof(_Tp)) + 1;
			std::cout << "num_nodes大小为:" << num_nodes << "\n";

			map_size = MyAlgorithm_base::max((size_t)initial_map_size, num_nodes + 2);;
			std::cout << "map_size大小为:"<<map_size<<"\n";

			//第一级指针的分配
			//一级指针=map的大小
			//二级指针=map_node位置
			map = allocate_map(map_size);

			//保证指向中间  把暂时开放的map给赋值    //8
			//std::cout << "newstart的地址为:" << (map_size - num_nodes) / 2 << "\n";
			_Tp** newstart = map + (map_size - num_nodes) / 2;
			//std::cout << "newfinish的地址为:" << (map_size - num_nodes) / 2 + num_nodes << "\n";
			_Tp** newfinish = newstart + num_nodes;
			
			create_nodes(newstart,newfinish); //把buffer开辟空间

			//设置好区块的大小 让start和finish做好边界 方便跳跃
			start.set_node(newstart);
			finish.set_node(newfinish - 1);
			//设置当前区块的第一个数据
			start.cur = start.first; 
			//永远指向 最后一个下一个位置
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
			difference_type index = pos - start; //当前的位置和起始位置
			value_type x_copy = x;
			if (index < size() / 2)
			{
				//std::cout << "我的start:" << &(*(start.cur))<<"\n";
				push_front(front()); //没看明白这里想干嘛
				iterator front1 = start;
				++front1; 
				//std::cout << "我的front1:" << &(*(front1.cur)) << "\n";

				iterator front2 = front1;
				++front2;
				//std::cout << "我的front2:" << &(*(front2.cur)) << "\n";

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
				//前插
				push_front(x);
				return start;
			}
			else if (position.cur == finish.cur)
			{
				//尾插
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