#pragma once
#ifndef MY_STRAITS_H_
#define MY_STRAITS_H_
namespace MyStriats
{
	//char_traits 类是一种特性类模板，对给定的字符类型抽象基础字符和字符串比较操作
	template<class CharT> 
	struct char_traits
	{
		using char_type = CharT;
		static bool ne(const char_type& c1, const char_type& c2)
		{
			return !(c1 == c2);
		}
		static char_type eos() { return char_type(); } // the null character

		//赋值字符 c2 给字符 c1
		static void assign(char_type& c1, const char_type& c2)
		{
			c1 = c2;
		}
		/**
		* dest	-	指向要复制的字符串的指针
		* src	-	指向复制来源的字符串的指针
		* count	-	要复制的字符数
		**/
		static char_type* copy(char_type* dest, const char_type* src, std::size_t count) 
		{
			for (; count--; )
				assign(dest[count], src[count]);
			return dest;
		}
		static size_t length(const char_type* s)
		{
			size_t l = 0;
			while (ne(*s++, eos()))
				++l;
			return l;
		}
	};

	template<>
	struct char_traits<char>
	{
		typedef char char_type;
		static size_t length(const char_type* s)
		{
			return std::strlen(s);
		}
		static char_type* copy(char_type* s1, const char_type* s2, size_t n)
		{
			return (char_type*)memcpy(s1, s2, n);
		}

		static char_type* move(char_type* s1, const char_type* s2, size_t n)
		{
			return (char_type*)memmove(s1, s2, n);
		}
	};
	 
}
#endif // !MY_STRAITS_H_
