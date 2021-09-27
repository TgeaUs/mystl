#pragma once
#ifndef _MYALGORITHMBASE_H_
#define _MYALGORITHMBASE_H_
#include <algorithm>
namespace MyAlgorithm_base
{

	template <class _ForwardIter, class _Tp>
	void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __value) {
		for (; __first != __last; ++__first)
			*__first = __value;
	}
	template<typename _OutputIter,typename _Size,typename _Tp>
	inline _OutputIter  //True进来的
		fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
	{
		for (; __n > 0; --__n, ++__first)
			*__first = __value;
		return __first;
	}
	//就先暂时第二个辅助函数了 因为还没萃取 
	template<typename _OutputIter, typename _Tp>
	inline _OutputIter  //True进来的
		fill_n_initializer_list(_OutputIter __first, const _Tp& value)
	{
		*__first = value;
		return __first;
	}

	template<typename __RandomAccessIter,typename __OutputIter,typename __Distance>
	inline __OutputIter
		__copy(__RandomAccessIter __first, __RandomAccessIter __last,__OutputIter __result, 
			MyIterator_traits::random_access_iterator_tag, __Distance*)
	{	
		for (__Distance __n = __last - __first; __n > 0; --__n)
		{
			*__result = *__first;
			++__result; ++__first;
		}
		return __result;
	}
	//copy -- generalization
	template <typename _InputIter, typename _OutputIter>
	inline _OutputIter 
		copy(_InputIter __first, _InputIter __last,_OutputIter __result)
	{
		return __copy(__first, __last, __result,
			MyIterator_traits::__iterator_category(__first),
			MyIterator_traits::__distance_type(__first));
	}
	//copy -- specialization
	//inline char* 
	//	copy(const char* __first, const char* __last, char* __result) 
	//{
	//	//以后写
	//	return (char*)0;
	//}
	////copy -- refinement
	//inline wchar_t* 
	//	copy(const wchar_t* __first, const wchar_t* __last, wchar_t* __result)
	//{
	//	//以后写
	//	return (wchar_t*)0;
	//}
	template <typename _BidirectionalIter1, typename _BidirectionalIter2,
		typename _Distance>
		inline _BidirectionalIter2 
		__copy_backward(
			_BidirectionalIter1 __first,
			_BidirectionalIter1 __last,
			_BidirectionalIter2 __result,
			MyIterator_traits::bidirectional_iterator_tag,
			_Distance*)
	{
		while (__first != __last)
			*--__result = *--__last;
		return __result;
	}
	template <typename _RandomAccessIter, typename _BidirectionalIter, typename _Distance>
	inline _BidirectionalIter 
		__copy_backward(
		_RandomAccessIter __first,
		_RandomAccessIter __last,
		_BidirectionalIter __result,
		MyIterator_traits::random_access_iterator_tag,
		_Distance*
		)
	{
		for (_Distance __n = __last - __first; __n > 0; --__n)
			*--__result = *--__last;
		return __result;
	}
	template <typename _BI1, typename _BI2>
	inline _BI2 
		copy_backward(_BI1 first, _BI1 last, _BI2 result) 
	{
		return __copy_backward(
			first,
			last,
			result,
			MyIterator_traits::__iterator_category(first),
			MyIterator_traits::__distance_type(first)
		);
	}
	template<typename InputIterator, typename Distance>
	inline void
		_distance(
			InputIterator first,
			InputIterator last,
			Distance& result,
			MyIterator_traits::bidirectional_iterator_tag)
	{
		while (first!=last)
		{
			first++;
			result++;
		}
	}
	template<typename InputIterator,typename Distance>
	inline void 
		distance(InputIterator first, InputIterator last, Distance& result)
	{
		_distance(first, last, result, MyIterator_traits::__iterator_category(first));
	}
	////针对偏特化
	//template <class _BidirectionalIter1, class _BidirectionalIter2,class _BoolType>
	//struct __copy_backward_dispatch
	//{
	//	typedef typename MyIterator_traits::iterator_traits<_BidirectionalIter1>::iterator_category	_Cat;
	//	typedef typename MyIterator_traits::iterator_traits<_BidirectionalIter1>::difference_type		_Distance;

	//	static _BidirectionalIter2 copy(_BidirectionalIter1 __first,_BidirectionalIter1 __last,_BidirectionalIter2 __result) 
	//	{
	//		return __copy_backward(__first, __last, __result, _Cat(), (_Distance*)0);
	//	}
	//};
	//template <class _Tp> //non-const
	//struct __copy_backward_dispatch<_Tp*, _Tp*, MyType_traits::__true_type>
	//{
	//	static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
	//		const ptrdiff_t _Num = __last - __first;
	//		memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
	//		return __result - _Num;
	//	}
	//};
	//template <class _Tp> //const
	//struct __copy_backward_dispatch<const _Tp*, _Tp*, MyType_traits::__true_type>
	//{
	//	static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) 
	//	{
	//		return  __copy_backward_dispatch<_Tp*, _Tp*, __true_type>::copy(__first, __last, __result);
	//	}
	//};
	//template <class _BI1, class _BI2>
	//inline _BI2 
	//	copy_backward(_BI1 first, _BI1 last, _BI2 result) 
	//{
	//	typedef typename  MyType_traits::__type_traits<typename MyIterator_traits::iterator_traits<_BI2>::value_type>::has_trivial_assignment_operator _Trivial;
	//	return __copy_backward_dispatch<_BI1, _BI2, _Trivial>::copy(first, last, result);
	//}





	template<typename _Tp>
	inline const _Tp&
		max(const _Tp& a, const _Tp& b)
	{
		return  a < b ? b : a;

	}
	template <class _Tp>
	inline void 
		swap(_Tp& a, _Tp& b) {
		_Tp tmpe = a;
		a = b;
		b = tmpe;
	}



	template <class _InputIter, class _Distance>
	inline void __advance(_InputIter& __i, _Distance __n, MyIterator_traits::input_iterator_tag) {
		while (__n--) ++__i;
	}

	template <class _BidirectionalIterator, class _Distance>
	inline void __advance(_BidirectionalIterator& __i, _Distance __n,
		MyIterator_traits::bidirectional_iterator_tag) {
		if (__n >= 0)
			while (__n--) ++__i;
		else
			while (__n++) --__i;
	}

	template <class _RandomAccessIterator, class _Distance>
	inline void __advance(_RandomAccessIterator& __i, _Distance __n,
		MyIterator_traits::random_access_iterator_tag) {
		__i += __n;
	}

	template <class _InputIterator, class _Distance>
	inline void advance(_InputIterator& __i, _Distance __n) {
		__advance(__i, __n, MyIterator_traits::__iterator_category(__i));
	}

	template <typename _ForwardIter, typename _Tp, typename _Distance>
	_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Distance*)
	{
		_Distance __len = 0;
		distance(__first, __last, __len);
		_Distance __half;
		_ForwardIter __middle;

		while (__len > 0) {
			__half = __len >> 1;
			__middle = __first;
			advance(__middle, __half);
			if (*__middle < __val) {
				__first = __middle;
				++__first;
				__len = __len - __half - 1;
			}
			else
				__len = __half;
		}
		return __first;
	}

	template <typename _ForwardIter, typename _Tp>
	inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) 
	{
		return __lower_bound(__first, __last, __val,MyIterator_traits::__distance_type(__first));
	}


}
#endif // !_MYALGORITHMBASE_H_