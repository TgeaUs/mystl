#pragma once
#ifndef _MY_FUNCTIONAL_H_
#define _MY_FUNCTIONAL_H_
namespace MyFunctional
{
	template <class Arg, class Result>
	struct unary_function {
		typedef Arg argument_type;
		typedef Result result_type;
	};

	template <class Arg1, class Arg2, class Result>
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	template <typename T>
	class less :public binary_function<T, T, bool>
	{
	public:
		constexpr bool operator()(const T& _Left, const T& _Right) const
		{
			return _Left < _Right;
		}
	};
	template <class T>
	class identity :unary_function<T, bool>
	{
	public:
		constexpr T& operator()(const T& x)
		{
			return (T&)x;
		}
	};
}
#endif // !_MY_FUNCTIONAL_H_
