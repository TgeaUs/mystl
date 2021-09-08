#pragma once
#ifndef _TYPE_TRAITS_H
#define _TYPE_TRAITS_H


#pragma once
#include <iostream>
namespace mytypetraits
{
	struct true_type
	{
		bool operator()()
		{
			return true;
		}
	};
	struct false_type
	{
		bool operator()()
		{
			return false;
		}
	};

	template <typename _Ty>
	struct remove_const
	{
		typedef _Ty type;
	};
	template <typename _Ty>
	struct remove_const<const _Ty>
	{
		typedef _Ty type;
	};



	template <typename _Ty>
	struct Mintegral_constant :false_type {};

	template <>
	struct Mintegral_constant<void> :true_type {};

	template <typename _Ty>
	struct isvoid :public Mintegral_constant<typename remove_const<_Ty>::type> {};


}
#endif