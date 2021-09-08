#pragma once
#ifndef _MYTYPETRAITS_H_
#define _MYTYPETRAITS_H_
#define __STL_TEMPLATE_NULL template<>
namespace MyType_traits
{
	//��Щ���Ǳ�׼���Դ���� ����������ȡ ������ȡ��Ļ��������ǲ�����Ҫ�� �ر���ָ��
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

	//����ָ��
	template <class _Tp>
	struct __type_traits<_Tp*>
	{
		using     has_trivial_default_constructor	= __true_type;
		using     has_trivial_copy_constructor		= __true_type;
		using     has_trivial_assignment_operator	=__true_type;
		using     has_trivial_destructor	= __true_type;
		using     is_POD_type				= __true_type;
	};	
	//���ڿ�ʼ��ָ��� �ػ�����
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

	template <typename Ty>
	constexpr bool is_void_aux = is_same_v<remove_cv_t<Ty>, void>;
	template <typename Ty>
	struct is_void : bool_constant<is_void_aux<Ty>> {};
	template< class T >
	constexpr bool is_void_v = is_void<T>::value;
};

#endif // !_MYTYPETRAITS_H_
