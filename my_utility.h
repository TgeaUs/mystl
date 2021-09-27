#pragma once
#ifndef MY_UTILITY_H
#define MY_UTILITY_H
#include <type_traits>
namespace my_utility
{
	//º¯Êý
	template <typename Ty>
	std::add_rvalue_reference_t<Ty> declval() noexcept;
}
#endif // !MY_UTILITY_H
