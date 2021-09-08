#pragma once
#ifndef MY_STRAITS_H_
#define MY_STRAITS_H_
namespace MyStriats
{
	//char_traits ����һ��������ģ�壬�Ը������ַ����ͳ�������ַ����ַ����Ƚϲ���
	template<class CharT> 
	struct char_traits
	{
		using char_type = CharT;
		static bool ne(const char_type& c1, const char_type& c2)
		{
			return !(c1 == c2);
		}
		static char_type eos() { return char_type(); } // the null character

		//��ֵ�ַ� c2 ���ַ� c1
		static void assign(char_type& c1, const char_type& c2)
		{
			c1 = c2;
		}
		/**
		* dest	-	ָ��Ҫ���Ƶ��ַ�����ָ��
		* src	-	ָ������Դ���ַ�����ָ��
		* count	-	Ҫ���Ƶ��ַ���
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
