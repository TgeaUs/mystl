#pragma once
#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H
#include <type_traits>
namespace ty
{
	template <typename T>
	class share_ptr
	{
		
		constexpr share_ptr() noexcept = default;
		constexpr share_ptr(std::nullptr_t) noexcept {}

		template <typename Ux, std::en>
	};
}

#endif // ! MY_SHARED_PTR_H
