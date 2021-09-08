#pragma once
#ifndef _MYVECTOR_H_
#define _MYVECTOR_H_
#include <iostream>
#include <cstddef> //ptrdiff_t
#include "my_uninitialized.h"
#include "my_allocator.h"
#include "my_iterator_traits.h"
#include "my_iterator.h"
#include "my_config.h"
//迭代器		100%
//构造函数	60%
//析构函数	100%
//容量		100%
//元素访问	100%
//修改器		100%
namespace MyVector
{

	//Time:2020/5/21
	//std::vector	
	//End time 2020/6/27

	template<typename _Tp,typename allocator=MyAllocator::allocator<_Tp>>
	class vector
	{
		//防御式写法 可以预防 global object
		_Tp* start;//开始的位置
		_Tp* finish;//最后的位置
		_Tp* end_of_storage;//全部的大小


		//分配一波内存
		void uninitialized_fill(size_t n, const _Tp& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		//C++11初始化序列专属初始化
		void initializer_list_fill(size_t& init_size,std::initializer_list<_Tp> init)
		{
			start = allocate_and_fill_initializer_list(init_size,init);
			finish = start + init_size;
			end_of_storage = finish;
			
		}

	public:
		using size_type = size_t;	//大小
	private:
		size_type init_size = 1;//记录初始化的数量 C++11

	public:
		//STL的五个部件 必有
		//迭代器 vector源代码是原生指针
		using value_type		= _Tp;		//表示你这个迭代器的类型
		using iterator			= value_type*;//你迭代器的类型
		using const_iterator	= const value_type*;//你迭代器的类型
		using difference_type	= ptrdiff_t;	//表示距离 两个迭代器
		using reference			= value_type&;		
		using const_reference	= const value_type&;		
		using pointer			= value_type*;		

		//适配器
		using reverse_iterator		 = MyIterator::reverse_iterator<iterator>;
		using const_reverse_iterator = MyIterator::reverse_iterator<const_iterator>;

		//分配器
		using data_allocator = MyAllocator::allocator<_Tp>; //默认分配器

		//真正的分配内存
		iterator allocate_and_fill(size_type n, const _Tp& value)
		{
			//reverse_iterator::myiteratortest();
			iterator result = data_allocator::allocate(n);
			MyUninitialized::uninitialized_fill_n(result, n, value); //这是填东西了 初始化那块
			return result;
		}
		//真正的分配内存
		//C++11
		iterator allocate_and_fill_initializer_list(size_t& init_number,std::initializer_list<_Tp> init)
		{
			//reverse_iterator::myiteratortest();
			iterator result = data_allocator::allocate(init_number);

			MyUninitialized::uninitialized_fill_n_initializer_list(result, init_number,init);
			return result;
		}

		//分配空间并且把原来的东西拷贝过去
		iterator allocate_and_copy(size_type n, const_iterator first, const_iterator last)
		{
			iterator result = data_allocator::allocate(n); //调用分配器
			MyUninitialized::uninitialized_copy(first, last, result);
			return result;
		}
	protected:
		void _insert_aux(iterator _pos, const _Tp& _x) 
		{
			if (finish != end_of_storage) //空间未满的情况下
			{
				//日后写
			}
			else //空间已满的情况下
			{
				const size_type _old_size = size(); //保存原来的大小
				const size_type _len = _old_size != 0 ? 2 * _old_size : 1; //两倍扩张大小
				iterator _new_start = data_allocator::allocate(_len); //分配内存
				iterator _new_finish = _new_start;
				STL_TRY
				{
					_new_finish=MyUninitialized::uninitialized_copy(start,_pos,_new_start);
					MyConstruct::construct(_new_finish, _x);
					++_new_finish;
					_new_finish = MyUninitialized::uninitialized_copy(_pos, finish, _new_finish);
					
				}
				STL_CATCH_ALL
				{
					//暂做异常中立
					throw;
				}
				MyConstruct::destroy(begin(), end());//调用析构
				data_allocator::deallocate(start); //释放内存
				start = _new_start;
				finish = _new_finish;
				end_of_storage = _new_start + _len;

			}
		}
	public:

		//构造函数之一
		vector() :	start(data_allocator::allocate(1)), 
					finish(start),
					end_of_storage(finish) {}
		//构造函数之三 n为数量 value为把n个初始化为value
		explicit vector(const size_type n, const _Tp& value)
		{
			uninitialized_fill(n, value);
		}
		//构造函数之4  构造拥有个 count 默认插入的 T()=0 实例的容器。不进行复制
		explicit vector(size_type count)
		{
			uninitialized_fill(count, _Tp()); //临时对象 全部都为0
		}
		//C++11标准
		//构造函数之九 构造拥有 initializer_list init 内容的容器
		vector(std::initializer_list<_Tp> init)
		{
			init_size = init.size();//记录整体有多大 直接分配那么大的内存
			initializer_list_fill(init_size,init);
		}

		//迭代器
		_NODISCARD	const_iterator			 begin()	const noexcept ->decltype(start) { return start; }
		_NODISCARD	const_iterator			 end()		const noexcept ->decltype(start) { return finish; }
		_NODISCARD	const_reverse_iterator	 rend()		const noexcept { return const_reverse_iterator(begin()); }
		_NODISCARD	const_reverse_iterator	 rbegin()	const noexcept { return const_reverse_iterator(end()); }

		_NODISCARD	iterator				 begin()	noexcept ->decltype(start)		{ return start; }
		_NODISCARD	iterator				 end()		noexcept ->decltype(finish)		{ return finish; }
		_NODISCARD	reverse_iterator		 rbegin()	noexcept { return reverse_iterator(end()); }
		_NODISCARD	reverse_iterator		 rend()		noexcept { return reverse_iterator(begin()); }

		//容量
		bool empty()			const noexcept { return begin() == end(); }
		size_type size()		const noexcept { return static_cast<size_type>((end() - begin())); } //有多少元素
		size_type max_size()	const noexcept { return size_type(-1) / sizeof(_Tp); }
		size_type capacity()	const noexcept { return end_of_storage - begin(); } //有多少空间
		void reverse(size_type new_cap) 
		{
			//if reverse>capacity,then allocate menmory,otherwise do nothing
			if (capacity() < new_cap)
			{
				const size_type old_size = size();
				iterator result = allocate_and_copy(new_cap, start, finish);
				MyConstruct::destroy(start, finish); //调用析构函数
				data_allocator::deallocate(start); //释放内存
				start = result;
				finish = start + old_size;
				end_of_storage = start+new_cap;
			}
		}
		////元素访问
		_NODISCARD reference		operator[](size_type n) noexcept			{ return *(begin() + n); }
		_NODISCARD const_reference  operator[](size_type n) const noexcept		{ return *(begin() + n); }
		_NODISCARD reference		front()	noexcept		{ return *begin(); }//规格书上说 等效于*begin()
		_NODISCARD const_reference  front() const noexcept	{ return *begin(); }
		_NODISCARD reference		back()	noexcept		{ return *(end() - 1); }
		_NODISCARD const_reference	back()	const noexcept	{ return *(end() - 1); }
		_NODISCARD value_type*		data()	noexcept		{ return start; } //C++11新标准data
		_NODISCARD const value_type*data()	const noexcept	{ return start; }
		_NODISCARD reference at(size_type pos)
		{
			if (pos >= size()) //标准库要求抛出异常
			{
				STL_THROW(std::out_of_range("out-of-range"));
			}
			return start[pos]; //标准库要求返回访问位置
		}
		_NODISCARD const_reference at(size_type pos)const
		{
			if (pos >= size()) //标准库要求抛出异常
			{
				STL_THROW(std::out_of_range("out-of-range"));
			}
			return start[pos]; //标准库要求返回访问位置
		}


		//修改器
		iterator erase(iterator position)
		{
			if (position + 1 != end())
			{
				MyAlgorithm_base::copy(position + 1, finish, position);
			}
			--finish; 
			MyConstruct::destroy(finish);//如果没有这一步 finish还是等于end_of_storage
			return position;
		}
		iterator erase(iterator first, iterator last)
		{
			auto i = MyAlgorithm_base::copy(last, finish, first);
			MyConstruct::destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}
		void clear() { erase(begin(), end()); }

		void insert(iterator postion, const _Tp& value)
		{

		}
		//pos位置插入count个value
		void insert(iterator position, size_type count, const _Tp& value) 
		{
			if (count != 0)
			{
				//还有剩余空间
				if (static_cast<size_type>(end_of_storage - finish) > count)
				{
					_Tp value_copy = value;
					const size_type elems_after = finish - position;
					iterator old_finish = finish;
					if (elems_after > count) //元素向后移动
					{
						MyUninitialized::uninitialized_copy(finish - count, finish, finish);
						finish += count;
						MyAlgorithm_base::copy_backward(position, old_finish - count, old_finish);
						MyAlgorithm_base::fill(position, position + count, value_copy);
					}
					else 
					{
						MyUninitialized::uninitialized_fill_n(finish, count - elems_after, value_copy);
						finish += count - elems_after;
						MyUninitialized::uninitialized_copy(position, old_finish, finish);
						finish += elems_after;
						MyAlgorithm_base::fill(position, old_finish, value_copy);
					}

				}
				else//没有剩余空间
				{
					const size_type old_size = size();
					const size_type len = old_size + MyAlgorithm_base::max(old_size, count);
					iterator new_start = data_allocator::allocate(len);
					iterator new_finish = new_start;
					STL_TRY
					{
						new_finish = MyAlgorithm_base::copy(begin(),position,new_start);
						new_finish = MyUninitialized::uninitialized_fill_n(new_finish, count, value);
						new_finish = MyUninitialized::uninitialized_copy(position, finish, new_finish);
					}
					STL_CATCH_ALL
					{
						//暂做异常中立
						throw;
					}
					MyConstruct::destroy(start, finish);
					data_allocator::deallocate(start);
					start = new_start;
					finish = new_finish;
					end_of_storage = new_start + len;
				}
			}
		}


		void insert(iterator position, iterator first, iterator last)
		{
			if (first != last) {
				size_type n = 0;
				MyAlgorithm_base::distance(first, last, n);
				if (size_type(end_of_storage - finish) >= n) {
					const size_type elems_after = finish - position;
					iterator old_finish(finish);
					if (elems_after > n) {
						MyUninitialized::uninitialized_copy(finish - n, finish, finish);
						finish += n;
						MyAlgorithm_base::copy_backward(position, old_finish - n, old_finish);
						MyAlgorithm_base::copy(first, last, position);
					}
					else {
						MyUninitialized::uninitialized_copy(first + elems_after, last, finish);
						finish += n - elems_after;
						MyUninitialized::uninitialized_copy(position, old_finish, finish);
						finish += elems_after;
						MyAlgorithm_base::copy(first, first + elems_after, position);
					}
				}
				else {
					const size_type old_size = size();
					const size_type len = old_size + MyAlgorithm_base::max(old_size, n);
					iterator new_start(data_allocator::allocate(len));
					iterator new_finish(new_start);
					STL_TRY{
					  new_finish = MyUninitialized::uninitialized_copy(start, position, new_start);
					  new_finish = MyUninitialized::uninitialized_copy(first, last, new_finish);
					  new_finish = MyUninitialized::uninitialized_copy(position, finish, new_finish);
					}
						STL_CATCH_ALL
					{
						throw;
					}
					MyConstruct::destroy(start, finish);
					data_allocator::deallocate(start);
					start = new_start;
					finish = new_finish;
					end_of_storage = new_start + len;
				}
			}
		}

		void push_back(const _Tp& value) 
		{
			if (finish != end_of_storage)//针对与有现存空间
			{
				MyConstruct::construct(finish, value);
				++finish;
			}
			else //针对无现存空间
			{
				_insert_aux(end(), value);
			}
		}
		void pop_back()
		{
			--finish;
			MyConstruct::destroy(finish);
		}
		void resize(size_type new_size, const _Tp& x)
		{
			if (new_size < size())
			{
				erase(begin() + new_size, end());
			}
			else
			{
				insert(end(), new_size - size(), x);
			}

		}
		void resize(size_type new_size)
		{
			if (new_size < size())
			{
				erase(begin() + new_size, end());
			}
		}

		//////////////// 测试用例
		void test_init()
		{
		}
		void test_size()
		{
			std::cout << __func__ << "\n";
			std::cout << "start size:\t" << sizeof(start)<<'\n';
			std::cout << std::endl << std::endl;
		}
		void test_type()
		{
			std::cout << __func__ << "\n";
			std::cout << "start:\t\t" << typeid(start).name() << "\n";
			std::cout <<"data_allocator:\t"<< typeid(data_allocator).name()<<"\n";
			std::cout <<"value_type:\t"<< typeid(value_type).name() << "\n";
			std::cout << std::endl << std::endl;
		}
		void test_typesize()
		{

		}
	};

}
#endif // !_MYSTING_H_
