#pragma once
#ifndef _MYALLOCATOR_H_
#define _MYALLOCATOR_H_
#include <iostream>
#include <new>
#include <cstddef> //ptrdiff_t


//内存分配器 普通的接管new
namespace MyAllocator
{
	template <typename _Tp>
	inline _Tp* _allocator(size_t size, _Tp*)
	{
		_Tp *temp = static_cast<_Tp*>(::operator new(size * sizeof(_Tp))); //为了连续空间打大小
		return temp;
	}
	template <typename _Tp>
	inline void _deallocator(_Tp* buffer)
	{
		if (buffer == nullptr)
			std::cout << "空";
		::operator delete(buffer);
	}

	template <typename _Tp>
	class allocator
	{
	public:
		using value_type	= _Tp;		//表示你这个迭代器的类型
		using size_type		= size_t;	//大小
		using difference_type = ptrdiff_t;	//表示距离 两个迭代器
		using reference = value_type&;		// 从没有使用过
		using pointer	= value_type*;		// 从没有使用过
		
		//分配内存 无任何特殊意义 纯粹的new
		static _Tp* allocate(size_type n)
		{
			return _allocator(n,(value_type*)0);//转化类型 这是两个参数的分配器
		}

		//释放内存 无任何特殊意义 纯粹的delete
		static void deallocate(pointer p)
		{
			_deallocator(p);
		}
		
	};




	//2021.9.2
	//源代码 SGI源代码
	//内存池分配器

	class __default_alloc_template
	{
	public:
		enum { __ALIGN = 8 };
		enum { __MAX_BYTES = 128 };
		enum { __NFREELISTS = __MAX_BYTES / __ALIGN };
		
		
		static size_t ROUND_UP(size_t bytes) {
			std::cout << "调整了:" <<(((bytes)+__ALIGN - 1) & ~(__ALIGN - 1)) << "\n";
			return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
		}
		static  size_t FREELIST_INDEX(size_t bytes) {
			std::cout << "落在了:" << (((bytes)+__ALIGN - 1) / __ALIGN - 1) << "\n";
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

			if (bytes_left >= total_bytes) // start-end足够支撑分配
			{
				result = start_free;
				start_free = start_free + total_bytes;
				return result;
			}
			else if(bytes_left >= size) //start-end不足以支撑
			{
				nobjs = bytes_left / size; //可以从start-end之中榨干几个出来
				total_bytes = size * nobjs; //重新设定总容量

				result = start_free; //同上
				start_free += total_bytes;
				return(result);
			}
			else
			{
				size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4); //索要两倍大小

				if (bytes_left > 0) //碎片 找到一个相应的篮子挂出去
				{
					obj ** my_free_list = free_list + FREELIST_INDEX(bytes_left);
					((obj*)start_free)->next = *my_free_list;
					*my_free_list = (obj*)start_free;
				}
				start_free = (char*)malloc(bytes_to_get);
				if (start_free == nullptr) //如果分配失败
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
			char* chunk = chunk_alloc(n, nobjs); //注意这个nobjs传递 non-const to reference 所以他会有可能变成1的
			obj** my_free_list;
			obj* result;
			obj* current_obj, * next_obj;
			int i;
			
			if (1 == nobjs) //检查是否被chunk_alloc更改过
			{
				return chunk;
			}
			my_free_list = free_list + FREELIST_INDEX(n);

			

			result = (obj*)(chunk); //把第一块给出去
			*my_free_list = next_obj = (obj*)(chunk + n); //跳转到下一块

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

			if (n > static_cast<size_t>(__MAX_BYTES)) //索要的大小超过了范围 不做服务
			{
				return malloc(n);
			}

			my_free_list = free_list + FREELIST_INDEX(n); //把第x号链表复制
			result = *my_free_list; //给他第x号链表判断有没有内存
			if (result == nullptr) //如果此x号没有给他内存
			{
				std::cout << "分配了一次";
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
