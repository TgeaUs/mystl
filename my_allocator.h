#pragma once
#ifndef _MYALLOCATOR_H_
#define _MYALLOCATOR_H_
#include <iostream>
#include <new>
#include <cstddef> //ptrdiff_t


//�ڴ������ ��ͨ�Ľӹ�new
namespace MyAllocator
{
	template <typename _Tp>
	inline _Tp* _allocator(size_t size, _Tp*)
	{
		_Tp *temp = static_cast<_Tp*>(::operator new(size * sizeof(_Tp))); //Ϊ�������ռ���С
		return temp;
	}
	template <typename _Tp>
	inline void _deallocator(_Tp* buffer)
	{
		if (buffer == nullptr)
			std::cout << "��";
		::operator delete(buffer);
	}

	template <typename _Tp>
	class allocator
	{
	public:
		using value_type	= _Tp;		//��ʾ�����������������
		using size_type		= size_t;	//��С
		using difference_type = ptrdiff_t;	//��ʾ���� ����������
		using reference = value_type&;		// ��û��ʹ�ù�
		using pointer	= value_type*;		// ��û��ʹ�ù�
		
		//�����ڴ� ���κ��������� �����new
		static _Tp* allocate(size_type n)
		{
			return _allocator(n,(value_type*)0);//ת������ �������������ķ�����
		}

		//�ͷ��ڴ� ���κ��������� �����delete
		static void deallocate(pointer p)
		{
			_deallocator(p);
		}
		
	};




	//2021.9.2
	//Դ���� SGIԴ����
	//�ڴ�ط�����

	class __default_alloc_template
	{
	public:
		enum { __ALIGN = 8 };
		enum { __MAX_BYTES = 128 };
		enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
		
		
		static size_t ROUND_UP(size_t bytes) {
			std::cout << "������:" <<(((bytes)+__ALIGN - 1) & ~(__ALIGN - 1)) << "\n";
			return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
		}
		static  size_t FREELIST_INDEX(size_t bytes) {
			std::cout << "������:" << (((bytes)+__ALIGN - 1) / __ALIGN - 1) << "\n";
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}
	private:
		struct obj
		{
			struct  obj* next;
		};
		static obj* free_list[];
		static char* start_free;
		static char* end_free;
		static size_t heap_size;
	public:
		static char* chunk_alloc(size_t size, int& nobjs)
		{
			char* result;
			size_t total_bytes = size * nobjs;
			size_t bytes_left = end_free - start_free;

			if (bytes_left >= total_bytes) // start-end�㹻֧�ŷ���
			{
				result = start_free;
				start_free = start_free + total_bytes;
				return result;
			}
			else if(bytes_left >= size) //start-end������֧��
			{
				nobjs = bytes_left / size; //���Դ�start-end֮��ե�ɼ�������
				total_bytes = size * nobjs; //�����趨������

				result = start_free; //ͬ��
				start_free += total_bytes;
				return(result);
			}
			else
			{
				size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4); //��Ҫ������С

				if (bytes_left > 0) //��Ƭ �ҵ�һ����Ӧ�����ӹҳ�ȥ
				{
					obj ** my_free_list = free_list + FREELIST_INDEX(bytes_left);
					((obj*)start_free)->next = *my_free_list;
					*my_free_list = (obj*)start_free;
				}
				start_free = (char*)malloc(bytes_to_get);
				if (start_free == nullptr) //�������ʧ��
				{
					
				}

				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return(chunk_alloc(size, nobjs));

			}
		}

		static void* refill(size_t n)
		{
			int nobjs = 1;
			char* chunk = chunk_alloc(n, nobjs); //ע�����nobjs���� non-const to reference ���������п��ܱ��1��
			obj** my_free_list;
			obj* result;
			obj* current_obj, * next_obj;
			int i;
			
			if (1 == nobjs) //����Ƿ�chunk_alloc���Ĺ�
			{
				return chunk;
			}
			my_free_list = free_list + FREELIST_INDEX(n);

			

			result = (obj*)(chunk); //�ѵ�һ�����ȥ
			*my_free_list = next_obj = (obj*)(chunk + n); //��ת����һ��

			for (i = 1; ; i++)
			{
				current_obj = next_obj;
				next_obj = (obj*)((char*)next_obj + n);
				if (nobjs - 1 == i) {
					current_obj->next = nullptr;
					break;
				}
				else {
					current_obj->next = next_obj;
				}
			}
			return result;
		}
		static void* allocate(size_t n)
		{
			obj** my_free_list;
			obj* result;

			if (n > static_cast<size_t>(__MAX_BYTES)) //��Ҫ�Ĵ�С�����˷�Χ ��������
			{
				return malloc(n);
			}

			my_free_list = free_list + FREELIST_INDEX(n); //�ѵ�x��������
			result = *my_free_list; //������x�������ж���û���ڴ�
			if (result == nullptr) //�����x��û�и����ڴ�
			{
				std::cout << "������һ��";
				void* fill_true = refill(ROUND_UP(n));
				return fill_true;
			}

			*my_free_list = result->next;
			return result;
		}


	};

	__default_alloc_template::obj* 
		__default_alloc_template::free_list[__default_alloc_template::__NFREELISTS] = {nullptr}; //16
	char* __default_alloc_template::start_free = nullptr; 
	char* __default_alloc_template::end_free= nullptr; 
	size_t __default_alloc_template::heap_size = 0; 
}
#endif // !_MYALLOCATOR_H_
