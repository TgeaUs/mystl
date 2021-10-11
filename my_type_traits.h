#pragma once
#ifndef _MYTYPETRAITS_H_
#define _MYTYPETRAITS_H_
#define __STL_TEMPLATE_NULL template<>


namespace MyType_traits
{
	//这些都是标准库的源代码 叫做类型萃取 就是萃取你的基础类型是不是重要的 特别是指针
	struct __true_type {};
	struct __false_type {};
	template <class _Tp>
	struct __type_traits {
		using     this_du__true_typemmy_member_must_be_first = __true_type;
		using     has_trivial_default_constructor	= __false_type;
		using     has_trivial_copy_constructor		= __false_type;
		using     has_trivial_assignment_operator	= __false_type;
		using     has_trivial_destructor			= __false_type;
		using     is_POD_type						= __false_type;
	};

	//泛化指针
	template <class _Tp>
	struct __type_traits<_Tp*>
	{
		using     has_trivial_default_constructor	= __true_type;
		using     has_trivial_copy_constructor		= __true_type;
		using     has_trivial_assignment_operator	=__true_type;
		using     has_trivial_destructor	= __true_type;
		using     is_POD_type				= __true_type;
	};	
	//现在开始无指针的 特化类型
	__STL_TEMPLATE_NULL
	struct __type_traits<bool>
	{
		using     has_trivial_default_constructor = __true_type;
		using     has_trivial_default_constructor	= __true_type;
		using     has_trivial_copy_constructor		= __true_type;
		using     has_trivial_assignment_operator	=__true_type;
		using     has_trivial_destructor	= __true_type;
		using     is_POD_type				= __true_type;
	};
	__STL_TEMPLATE_NULL
	struct __type_traits<char>
	{
		using     has_trivial_default_constructor	= __true_type;
		using     has_trivial_copy_constructor		= __true_type;
		using     has_trivial_assignment_operator	=__true_type;
		using     has_trivial_destructor	= __true_type;
		using     is_POD_type				= __true_type;
	};
	__STL_TEMPLATE_NULL
		struct __type_traits<signed char>
	{
		using     has_trivial_default_constructor = __true_type;
		using     has_trivial_copy_constructor = __true_type;
		using     has_trivial_assignment_operator = __true_type;
		using     has_trivial_destructor = __true_type;
		using     is_POD_type = __true_type;
	};
	__STL_TEMPLATE_NULL
		struct __type_traits<unsigned char>
	{
		using     has_trivial_default_constructor = __true_type;
		using     has_trivial_copy_constructor = __true_type;
		using     has_trivial_assignment_operator = __true_type;
		using     has_trivial_destructor = __true_type;
		using     is_POD_type = __true_type;
	};
	__STL_TEMPLATE_NULL
		struct __type_traits<wchar_t>
	{
		using     has_trivial_default_constructor = __true_type;
		using     has_trivial_copy_constructor = __true_type;
		using     has_trivial_assignment_operator = __true_type;
		using     has_trivial_destructor = __true_type;
		using     is_POD_type = __true_type;
	};
	template <typename Ty, Ty Val>
	struct intergral_constant
	{
		static constexpr Ty value = Val;
		using value_type = Ty;
		using type = intergral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}

		constexpr value_type operator()() const noexcept {
			return value;
		}
	};
	template <bool Val>
	using bool_constant = intergral_constant<bool, Val>;
	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	template <typename Ty>
	struct remove_cv
	{
		using type = Ty;
	};
	template <typename Ty>
	using remove_cv_t =typename remove_cv<Ty>::type;

	template <typename, typename>
	constexpr bool is_same_v = false;
	template <typename Ty>
	constexpr bool is_same_v<Ty, Ty> = true;
	template <typename Ty1, typename Ty2>
	constexpr bool is_same = bool_constant<is_same_v<Ty1, Ty2>>{};

	template <typename Ty>
	constexpr bool is_void_v = is_same_v<remove_cv_t<Ty>, void>;
	template <typename Ty>
	struct is_void : bool_constant<is_void_v<Ty>> {};

	template <typename Ty>
	constexpr bool is_null_pointer_v = is_same_v<remove_cv_t<Ty>, nullptr_t>;
	template <typename Ty>
	struct is_null_pointer : bool_constant<is_null_pointer_v<Ty>> {};

	//array
	template <typename Ty>
	constexpr bool is_array_v = false;
	template <typename Ty, size_t N>
	constexpr bool is_array_v<Ty[N]> = true;
	template <typename Ty>
	constexpr bool is_array_v<Ty[]> = true;
	template <typename Ty>
	struct is_array : bool_constant<is_array_v<Ty>>{};

	//is_enum
	template <typename Ty>
	constexpr bool is_enum_v = bool_constant<__is_enum(Ty)>::value;
	template <typename Ty>
	struct is_enum : bool_constant< __is_enum(Ty)> {};

	//is_union
	template <typename Ty>
	constexpr bool is_union_v = bool_constant<__is_union(Ty)>::value;
	template <typename Ty>
	struct is_union : bool_constant< __is_union(Ty)> {};

	//is_union
	template <typename Ty>
	constexpr bool is_class_v = bool_constant<__is_class(Ty)>::value;
	template <typename Ty>
	struct is_class : bool_constant< __is_class(Ty)> {};

	//is_funtion
	template <class _Ty>
	struct _Is_function { // determine whether _Ty is a function
		using _Bool_type = false_type; // NB: members are user-visible via _Weak_types
	};
	template <typename Ty>
	constexpr bool is_function_v = _Is_function<Ty>::_Bool_type::value;

	//is_pointer
	template <typename T>
	constexpr bool is_pointer_v = false;

	template <typename T>
	constexpr bool is_pointer_v<T*> = true;

	template <typename T>
	constexpr bool is_pointer_v<T* const> = true;

	template <typename T>
	constexpr bool is_pointer_v<T* volatile> = true;

	template <typename T>
	constexpr bool is_pointer_v<T* const volatile> = true;

	template <typename Ty>
	struct is_pointer : bool_constant<is_pointer_v<Ty>> {};

	//is_lvalue_reference
	template <typename Ty>
	constexpr bool is_lvalue_reference_v = false;

	template <typename Ty>
	constexpr bool is_lvalue_reference_v<Ty&> = true;

	template <typename Ty>
	struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<Ty>> {};
	
	//is_rvalue_reference
	template <typename Ty>
	constexpr bool is_rvalue_reference_v = false;

	template <typename Ty>
	constexpr bool is_rvalue_reference_v<Ty&&> = true;

	template <typename Ty>
	struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<Ty>> {};
	
	//is_reference
	template <class>
	constexpr bool is_reference_v = false; // determine whether type argument is a reference

	template <class _Ty>
	constexpr bool is_reference_v<_Ty&> = true;

	template <class _Ty>
	constexpr bool is_reference_v<_Ty&&> = true;

	template <class _Ty>
	struct is_reference : bool_constant<is_reference_v<_Ty>> {};

	//is_fundamental
	template <typename Ty>
	constexpr bool is_arithmetic_v = std::is_integral_v<Ty> || std::is_floating_point_v<Ty>;

	template <class Ty>
	constexpr bool is_fundamental_v = is_arithmetic_v<Ty> || is_void_v<Ty> || is_null_pointer_v<Ty>;

	template <typename Ty>
	struct is_fundamental : bool_constant<is_fundamental_v<Ty>> {};
	
	//is_arithmetic
	template <typename Ty>
	struct is_arithmetic : bool_constant<is_arithmetic_v<Ty>> {};

	//is_scalar
	template <typename Ty>
	constexpr bool is_scalar_v = is_arithmetic_v<Ty> || is_enum_v<Ty> || is_pointer_v<Ty> || std::is_member_pointer_v<Ty> || is_null_pointer_v<Ty>;
	
	template <typename Ty>
	struct is_scalar : bool_constant<is_scalar_v<Ty>> {};

	//is_object
	template <typename Ty>
	constexpr bool is_object_v = !is_function_v<Ty> && !is_reference_v<Ty> && !is_void_v<Ty>;

	template <typename Ty>
	struct is_object : bool_constant<is_object_v<Ty>> {};

	//is_compound
	template <typename Ty>
	constexpr bool is_compound_v = !is_function_v<Ty>;

	template <typename Ty>
	struct is_compound : bool_constant<!is_compound_v<Ty>>{};

	//is_member_pointer
	  

	//类型的性质

	//is_const
	template <typename Ty>
	constexpr bool is_const_v = false;

	template <typename Ty>
	constexpr bool is_const_v<const Ty> = true;

	template <typename Ty>
	struct is_const : bool_constant<is_const_v<Ty>> {};

	//is_volatile
	template <typename Ty>
	constexpr bool is_volatile_v = false;

	template <typename Ty>
	constexpr bool is_volatile_v<volatile Ty> = true;

	template <typename Ty>
	struct is_volatile : bool_constant<is_volatile_v<Ty>> {};

	//is_trivial
	template <typename _Ty>
	constexpr bool is_trivial_v = __is_trivially_constructible(_Ty) && __is_trivially_copyable(_Ty);

	template <typename Ty>
	struct is_trivial : bool_constant<__is_trivially_constructible(Ty) && __is_trivially_copyable(Ty)> {};

	//is_trivially_copyable
	template <typename Ty>
	constexpr bool is_trivially_copyable_v = bool_constant<__is_trivially_copyable(Ty)>::value;
	template <typename Ty>
	struct is_trivially_copyable : public bool_constant<__is_trivially_copyable(Ty)> {};

	//is_empty
	template <typename Ty>
	constexpr bool is_empty_v = bool_constant<__is_empty(Ty)>::value;
	template <typename Ty>
	struct is_empty : public bool_constant<__is_empty(Ty)> {};

	//is_polymorphic
	template <typename Ty>
	constexpr bool is_polymorphic_v = bool_constant<__is_polymorphic(Ty)>::value;
	template <typename Ty>
	struct is_polymorphic : public bool_constant<__is_polymorphic(Ty)> {};








	//杂项变换 https://zh.cppreference.com/w/cpp/header/type_traits
	//enable_if
	template <bool Test, typename Ty = void>
	struct enable_if {};

	template <typename Ty>
	struct enable_if<true, Ty>
	{
		using type = Ty;
	};
};

#endif // !_MYTYPETRAITS_H_
