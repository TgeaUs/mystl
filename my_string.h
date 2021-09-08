#pragma once
#ifndef _MY_STRING_H_
#define _MY_STRING_H_
#include "my_straits.h"
#include "my_allocator.h"
#include <exception>
#include <iostream>
namespace MyString
{

	//class out_of_range : public std::logic_error {
	//public:
	//	out_of_range(const string<char>&& what_arg) : std::logic_error(what_arg) { }
	//};

	//void
	//	__out_of_range(const char* s)
	//{
	//	throw out_of_range(s);
	//}
	//class length_error : public std::logic_error {
	//public:
	//	length_error(const string<char>& what_arg) : std::logic_error(what_arg) { }
	//};
	//void __length_error(const char* s)
	//{
	//	throw length_error(s);
	//}

#ifdef __STL_USE_EXCEPTIONS

	extern void __out_of_range(const char*);
	extern void __length_error(const char*);

#define OUTOFRANGE(cond) \
  do { if (cond) __out_of_range (#cond); } while (0)
#define LENGTHERROR(cond) \
  do { if (cond) __length_error (#cond); } while (0)

#else

#include <cassert>
#define OUTOFRANGE(cond) assert (!(cond))
#define LENGTHERROR(cond) assert (!(cond))

#endif

	//Start: 2020 10/23
	/*
		按照标准库的string仿写
		源代码为 gcc-2.95/libstdc++
	*/
	template<typename CharT, typename Traits=MyStriats::char_traits<CharT>, typename allocator = MyAllocator::allocator<CharT>>
	class string
	{
		struct Rep
		{
			size_t lenght; //字符串长度
			size_t capacity; //字符串容量
			size_t reference_count; //计数器
			bool selfish; //是否变动过 true=改动 false=未改动

			//这里的1代表sizeof(Rep)
			CharT* data() { return reinterpret_cast<CharT*>(this + 1); }

			void release() { if (--reference_count == 0) delete this; }

			inline static void* operator new (size_t s, size_t extra)
			{
				return allocator::allocate(s + extra + sizeof(CharT));
			}
			inline static void operator delete (void* ptr)
			{
				//官方用的内存池设计 日后写了在把注释取消掉把
				//allocator::deallocate(ptr, sizeof(Rep) +reinterpret_cast<Rep*>(ptr)->res *sizeof(charT));
			}

			//copy
			void copy(size_t pos, const CharT* s, size_t n)
			{
				if (n)
					Traits::copy(data() + pos, s, n);
			}

			//move
			inline void move(size_t pos, const CharT* s, size_t n)
			{
				if (n)
					Traits::move(data() + pos, s, n);
			}


			//forb_size
			static size_t frob_size(size_t s)
			{
				size_t i = 16;
				while (i < s) i *= 2;
				return i;
			}

			//create
			static Rep* create(size_t extra)
			{
				extra = frob_size(extra + 1);
				Rep* p = new (extra) Rep;
				p->capacity = extra;
				p->reference_count = 1;
				p->selfish = false;
				return p;
			}

			//clone
			CharT* clone()
			{
				Rep* p = create(lenght);
				p->copy(0, data(), lenght);
				p->lenght = lenght;
				return p->data();
			}

			//grab
			CharT* grab()
			{
				if (selfish) //如果改动
					return clone();
				////没改动
				++reference_count;
				return data();
			}

			inline static bool excess_slop(size_t s, size_t r)
			{
				return 2 * (s <= 16 ? 16 : s) < r;
			}
		};

		Rep nilRep = { 0,0,1,false };
		CharT* aData;//真正的字符串储存位置

		Rep* rep() const { return reinterpret_cast<Rep*>(aData) - 1; }

		void repup(Rep* p) { rep()->release(); aData = p->data(); }

	public:
		typedef size_t size_type;
		static const size_type npos = static_cast<size_type>(-1);
		size_type length() const
		{
			return rep()->lenght;
		}
		size_type capacity() const
		{
			return rep()->capacity;
		}
		const CharT* data() const
		{
			return rep()->data();
		}
		size_type max_size() const
		{
			return (npos - 1) / sizeof(CharT);
		}		// XXX
		inline bool check_realloc(size_type s) const
		{
			s += sizeof(CharT);
			rep()->selfish = false;
			return (
				rep()->reference_count > 1
				|| s > capacity()
				|| Rep::excess_slop(s, capacity()));
		}
		//范围 [cstr, cstr + count2) 中的字符
		/**
		* pos	-	将被替换的子串起始位置
		* count	-	将被替换的子串长度
		* cstr	-	指向用于替换的字符串的指针
		* count2	-	用于替换的字符数
		**/
		string& replace(size_type pos, size_type n1, const CharT* s, size_type n2)
		{

			const size_type len = length();
			OUTOFRANGE(pos > len);
			if (n1 > len - pos)
				n1 = len - pos;
			LENGTHERROR(len - n1 > max_size() - n2);
			size_t newlen = len - n1 + n2;

			if (check_realloc(newlen))
			{
				Rep* p = Rep::create(newlen);
				//核心
				p->copy(0, data(), pos);
				p->copy(pos + n2, data() + pos + n1, len - (pos + n1));
				p->copy(pos, s, n2);
				repup(p);
			}
			else
			{
				rep()->move(pos + n2, data() + pos + n1, len - (pos + n1));
				rep()->copy(pos, s, n2);
			}
			rep()->lenght = newlen;

			return *this;
		}

		//5) 以范围 [s, s+count) 中的字符的副本替换内容。此范围能含空字符。
		string& assign(const CharT* s, size_type n)
		{
			return replace(0, npos, s, n);
		}
		//6) 以 s 所指向的空终止字符串的内容替换内容。由首个空字符，用 Traits::length(s) 确定字符串长度。
		string& assign(const CharT* s)
		{
			return assign(s, Traits::length(s));
		}	

		string(const CharT* s) :aData(nilRep.grab())
		{
			assign(s);
		}

		string(const string& str) : aData(str.rep()->grab()) { }

		friend std::ostream& operator<< (std::ostream& o, const string& s)
		{
			return o.write(s.data(), s.length());
		}
	};
}

#endif // !#define _MY_STRING_H_
