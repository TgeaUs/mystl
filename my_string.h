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
		���ձ�׼���string��д
		Դ����Ϊ gcc-2.95/libstdc++
	*/
	template<typename CharT, typename Traits=MyStriats::char_traits<CharT>, typename allocator = MyAllocator::allocator<CharT>>
	class string
	{
		struct Rep
		{
			size_t lenght; //�ַ�������
			size_t capacity; //�ַ�������
			size_t reference_count; //������
			bool selfish; //�Ƿ�䶯�� true=�Ķ� false=δ�Ķ�

			//�����1����sizeof(Rep)
			CharT* data() { return reinterpret_cast<CharT*>(this + 1); }

			void release() { if (--reference_count == 0) delete this; }

			inline static void* operator new (size_t s, size_t extra)
			{
				return allocator::allocate(s + extra + sizeof(CharT));
			}
			inline static void operator delete (void* ptr)
			{
				//�ٷ��õ��ڴ����� �պ�д���ڰ�ע��ȡ������
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
				if (selfish) //����Ķ�
					return clone();
				////û�Ķ�
				++reference_count;
				return data();
			}

			inline static bool excess_slop(size_t s, size_t r)
			{
				return 2 * (s <= 16 ? 16 : s) < r;
			}
		};

		Rep nilRep = { 0,0,1,false };
		CharT* aData;//�������ַ�������λ��

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
		//��Χ [cstr, cstr + count2) �е��ַ�
		/**
		* pos	-	�����滻���Ӵ���ʼλ��
		* count	-	�����滻���Ӵ�����
		* cstr	-	ָ�������滻���ַ�����ָ��
		* count2	-	�����滻���ַ���
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
				//����
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

		//5) �Է�Χ [s, s+count) �е��ַ��ĸ����滻���ݡ��˷�Χ�ܺ����ַ���
		string& assign(const CharT* s, size_type n)
		{
			return replace(0, npos, s, n);
		}
		//6) �� s ��ָ��Ŀ���ֹ�ַ����������滻���ݡ����׸����ַ����� Traits::length(s) ȷ���ַ������ȡ�
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
