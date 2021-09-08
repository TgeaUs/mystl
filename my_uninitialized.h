#pragma once
#ifndef _MYUNINITIALIZED_H_
#define _MYUNINITIALIZED_H_
#include "my_construct.h"
#include "my_iterator_traits.h"
#include "my_vector.h"
#include "my_type_traits.h"
#include "my_algorithm_base.h"
#include "my_config.h"
//模仿标准库写的初始化
//目前进度 0%

//标准库需要有5个初始化的
//construct和destroy在construct里面

//uninitialized_fill_n ----->fill_n
//uninitialized_fill   ----->fill
//uninitialized_copy   ----->copy
namespace MyUninitialized
{

	//真正干的事情 这是is_trivial FALUE!!
	template<typename __ForwardIterator, typename __Size, typename __T>
	inline __ForwardIterator
		__uninitialized_fill_n_aux(__ForwardIterator __first, __Size __count,const __T& __value, MyType_traits::__false_type)
	{
		__ForwardIterator __cur = __first; 
		for (; __count > 0; --__count, ++__cur)
			MyConstruct::construct(&*__cur, __value);
		return __cur;
	}
	//真正干的事情 这是is_trivial TRUE!!
	template<typename __ForwardIterator, typename __Size, typename __T>
	inline __ForwardIterator
		__uninitialized_fill_n_aux(__ForwardIterator __first, __Size __count, const __T& __value, MyType_traits::__true_type)
	{
		return MyAlgorithm_base::fill_n(__first, __count, __value);
	}
	template<typename _ForwardIterator, typename _Size, typename _T,typename _Tp1>
	inline _ForwardIterator
		__uninitialized_fill_n(_ForwardIterator _first, _Size _count, const _T& _value,_Tp1)
	{
#if ISDIALOGUE
		std::cout <<"__uninitialized_fill_n()的value_type是"<<typeid(_Tp1).name()<<"\n""\n";
#endif // 

		typedef typename MyType_traits::__type_traits<int*>::is_POD_type _is_trivial;
		return __uninitialized_fill_n_aux(_first, _count, _value, _is_trivial());

	}
	//first初始化的起始位置 把多少个count初始化为value
	template<typename ForwardIterator,typename Size,typename T>
	inline ForwardIterator 
		uninitialized_fill_n(ForwardIterator first, Size count, const T& value)
	{
		return __uninitialized_fill_n(first, count, value,MyIterator_traits::__value_type(first));
	}

	//C++11
	template<typename ForwardIterator, typename Size,typename _Tp>
	inline ForwardIterator
		uninitialized_fill_n_initializer_list(ForwardIterator first, Size init_size, std::initializer_list<_Tp> init)
	{
		for (auto i : init)
		{
			MyAlgorithm_base::fill_n_initializer_list(first, i);
			++first;
		}
		return first;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
	template <class _ForwardIter, class _Tp>
	inline void
		__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
			const _Tp& __x, MyType_traits::__true_type)
	{
		MyAlgorithm_base::fill(__first, __last, __x);
	}

	template <class _ForwardIter, class _Tp>
	inline void
		__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
			const _Tp& __x, MyType_traits::__false_type)
	{
		_ForwardIter __cur = __first;
		STL_TRY{
		  for (; __cur != __last; ++__cur)
			MyConstruct::construct(&*__cur, __x);
		}
		STL_UNWIND(MyConstruct::destroy(__first, __cur));
	}

	template <class _ForwardIter, class _Tp, class _Tp1>
	inline void __uninitialized_fill(_ForwardIter __first,
		_ForwardIter __last, const _Tp& __x, _Tp1*)
	{
		
		typedef typename MyType_traits::__type_traits<_Tp1>::is_POD_type _is_trivial;
		__uninitialized_fill_aux(__first, __last, __x, _is_trivial());

	}
	template <class _ForwardIter, class _Tp>
	inline void
		uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x)
	{
		__uninitialized_fill(__first, __last, __x, MyIterator_traits::__value_type(__first));
	}

	

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class __InputIter, class __ForwardIter>
	__ForwardIter //1
		__uninitialized_copy_aux(__InputIter __first, __InputIter __last, __ForwardIter __result, MyType_traits::__true_type)
	{
		return MyAlgorithm_base::copy(__first, __last, __result);
	}
	template <class __InputIter, class __ForwardIter>
	__ForwardIter //0
		__uninitialized_copy_aux(__InputIter __first, __InputIter __last, __ForwardIter __result, MyType_traits::__false_type)
	{
		__ForwardIter __cur = __result;
		STL_TRY
		{
			for (; __first != __last; __first++, __cur++)
				MyConstruct::construct(&*__cur, *__first);
			return __cur;
		}
		STL_UNWIND(MyConstruct::destroy(__result, __cur));
	}
	template <class _InputIter, class _ForwardIter, class _Tp>
	inline _ForwardIter
		__uninitialized_copy(_InputIter _first, _InputIter _last, _ForwardIter _result, _Tp*)
	{
		typedef typename MyType_traits::__type_traits<_Tp>::is_POD_type _Is_POD;
		return __uninitialized_copy_aux(_first, _last, _result, _Is_POD());

	}
	template <class InputIter, class ForwardIter>
	inline ForwardIter
		uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
	{
		return __uninitialized_copy(first, last, result, MyIterator_traits::__value_type(result));
	}
}
#endif // _MYCONSTRUCT_H_
