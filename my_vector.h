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
//������		100%
//���캯��	60%
//��������	100%
//����		100%
//Ԫ�ط���	100%
//�޸���		100%
namespace MyVector
{

	//Time:2020/5/21
	//std::vector	
	//End time 2020/6/27

	template<typename _Tp,typename allocator=MyAllocator::allocator<_Tp>>
	class vector
	{
		//����ʽд�� ����Ԥ�� global object
		_Tp* start;//��ʼ��λ��
		_Tp* finish;//����λ��
		_Tp* end_of_storage;//ȫ���Ĵ�С


		//����һ���ڴ�
		void uninitialized_fill(size_t n, const _Tp& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		//C++11��ʼ������ר����ʼ��
		void initializer_list_fill(size_t& init_size,std::initializer_list<_Tp> init)
		{
			start = allocate_and_fill_initializer_list(init_size,init);
			finish = start + init_size;
			end_of_storage = finish;
			
		}

	public:
		using size_type = size_t;	//��С
	private:
		size_type init_size = 1;//��¼��ʼ�������� C++11

	public:
		//STL��������� ����
		//������ vectorԴ������ԭ��ָ��
		using value_type		= _Tp;		//��ʾ�����������������
		using iterator			= value_type*;//�������������
		using const_iterator	= const value_type*;//�������������
		using difference_type	= ptrdiff_t;	//��ʾ���� ����������
		using reference			= value_type&;		
		using const_reference	= const value_type&;		
		using pointer			= value_type*;		

		//������
		using reverse_iterator		 = MyIterator::reverse_iterator<iterator>;
		using const_reverse_iterator = MyIterator::reverse_iterator<const_iterator>;

		//������
		using data_allocator = MyAllocator::allocator<_Tp>; //Ĭ�Ϸ�����

		//�����ķ����ڴ�
		iterator allocate_and_fill(size_type n, const _Tp& value)
		{
			//reverse_iterator::myiteratortest();
			iterator result = data_allocator::allocate(n);
			MyUninitialized::uninitialized_fill_n(result, n, value); //��������� ��ʼ���ǿ�
			return result;
		}
		//�����ķ����ڴ�
		//C++11
		iterator allocate_and_fill_initializer_list(size_t& init_number,std::initializer_list<_Tp> init)
		{
			//reverse_iterator::myiteratortest();
			iterator result = data_allocator::allocate(init_number);

			MyUninitialized::uninitialized_fill_n_initializer_list(result, init_number,init);
			return result;
		}

		//����ռ䲢�Ұ�ԭ���Ķ���������ȥ
		iterator allocate_and_copy(size_type n, const_iterator first, const_iterator last)
		{
			iterator result = data_allocator::allocate(n); //���÷�����
			MyUninitialized::uninitialized_copy(first, last, result);
			return result;
		}
	protected:
		void _insert_aux(iterator _pos, const _Tp& _x) 
		{
			if (finish != end_of_storage) //�ռ�δ���������
			{
				//�պ�д
			}
			else //�ռ������������
			{
				const size_type _old_size = size(); //����ԭ���Ĵ�С
				const size_type _len = _old_size != 0 ? 2 * _old_size : 1; //�������Ŵ�С
				iterator _new_start = data_allocator::allocate(_len); //�����ڴ�
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
					//�����쳣����
					throw;
				}
				MyConstruct::destroy(begin(), end());//��������
				data_allocator::deallocate(start); //�ͷ��ڴ�
				start = _new_start;
				finish = _new_finish;
				end_of_storage = _new_start + _len;

			}
		}
	public:

		//���캯��֮һ
		vector() :	start(data_allocator::allocate(1)), 
					finish(start),
					end_of_storage(finish) {}
		//���캯��֮�� nΪ���� valueΪ��n����ʼ��Ϊvalue
		explicit vector(const size_type n, const _Tp& value)
		{
			uninitialized_fill(n, value);
		}
		//���캯��֮4  ����ӵ�и� count Ĭ�ϲ���� T()=0 ʵ���������������и���
		explicit vector(size_type count)
		{
			uninitialized_fill(count, _Tp()); //��ʱ���� ȫ����Ϊ0
		}
		//C++11��׼
		//���캯��֮�� ����ӵ�� initializer_list init ���ݵ�����
		vector(std::initializer_list<_Tp> init)
		{
			init_size = init.size();//��¼�����ж�� ֱ�ӷ�����ô����ڴ�
			initializer_list_fill(init_size,init);
		}

		//������
		_NODISCARD	const_iterator			 begin()	const noexcept ->decltype(start) { return start; }
		_NODISCARD	const_iterator			 end()		const noexcept ->decltype(start) { return finish; }
		_NODISCARD	const_reverse_iterator	 rend()		const noexcept { return const_reverse_iterator(begin()); }
		_NODISCARD	const_reverse_iterator	 rbegin()	const noexcept { return const_reverse_iterator(end()); }

		_NODISCARD	iterator				 begin()	noexcept ->decltype(start)		{ return start; }
		_NODISCARD	iterator				 end()		noexcept ->decltype(finish)		{ return finish; }
		_NODISCARD	reverse_iterator		 rbegin()	noexcept { return reverse_iterator(end()); }
		_NODISCARD	reverse_iterator		 rend()		noexcept { return reverse_iterator(begin()); }

		//����
		bool empty()			const noexcept { return begin() == end(); }
		size_type size()		const noexcept { return static_cast<size_type>((end() - begin())); } //�ж���Ԫ��
		size_type max_size()	const noexcept { return size_type(-1) / sizeof(_Tp); }
		size_type capacity()	const noexcept { return end_of_storage - begin(); } //�ж��ٿռ�
		void reverse(size_type new_cap) 
		{
			//if reverse>capacity,then allocate menmory,otherwise do nothing
			if (capacity() < new_cap)
			{
				const size_type old_size = size();
				iterator result = allocate_and_copy(new_cap, start, finish);
				MyConstruct::destroy(start, finish); //������������
				data_allocator::deallocate(start); //�ͷ��ڴ�
				start = result;
				finish = start + old_size;
				end_of_storage = start+new_cap;
			}
		}
		////Ԫ�ط���
		_NODISCARD reference		operator[](size_type n) noexcept			{ return *(begin() + n); }
		_NODISCARD const_reference  operator[](size_type n) const noexcept		{ return *(begin() + n); }
		_NODISCARD reference		front()	noexcept		{ return *begin(); }//�������˵ ��Ч��*begin()
		_NODISCARD const_reference  front() const noexcept	{ return *begin(); }
		_NODISCARD reference		back()	noexcept		{ return *(end() - 1); }
		_NODISCARD const_reference	back()	const noexcept	{ return *(end() - 1); }
		_NODISCARD value_type*		data()	noexcept		{ return start; } //C++11�±�׼data
		_NODISCARD const value_type*data()	const noexcept	{ return start; }
		_NODISCARD reference at(size_type pos)
		{
			if (pos >= size()) //��׼��Ҫ���׳��쳣
			{
				STL_THROW(std::out_of_range("out-of-range"));
			}
			return start[pos]; //��׼��Ҫ�󷵻ط���λ��
		}
		_NODISCARD const_reference at(size_type pos)const
		{
			if (pos >= size()) //��׼��Ҫ���׳��쳣
			{
				STL_THROW(std::out_of_range("out-of-range"));
			}
			return start[pos]; //��׼��Ҫ�󷵻ط���λ��
		}


		//�޸���
		iterator erase(iterator position)
		{
			if (position + 1 != end())
			{
				MyAlgorithm_base::copy(position + 1, finish, position);
			}
			--finish; 
			MyConstruct::destroy(finish);//���û����һ�� finish���ǵ���end_of_storage
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
		//posλ�ò���count��value
		void insert(iterator position, size_type count, const _Tp& value) 
		{
			if (count != 0)
			{
				//����ʣ��ռ�
				if (static_cast<size_type>(end_of_storage - finish) > count)
				{
					_Tp value_copy = value;
					const size_type elems_after = finish - position;
					iterator old_finish = finish;
					if (elems_after > count) //Ԫ������ƶ�
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
				else//û��ʣ��ռ�
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
						//�����쳣����
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
			if (finish != end_of_storage)//��������ִ�ռ�
			{
				MyConstruct::construct(finish, value);
				++finish;
			}
			else //������ִ�ռ�
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

		//////////////// ��������
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
