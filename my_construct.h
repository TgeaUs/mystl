#pragma once
#ifndef _MYCONSTRUCT_H_
#define _MYCONSTRUCT_H_
#include "my_iterator_traits.h"
#include "my_type_traits.h"
#include "my_config.h"
//构造构造！
namespace MyConstruct
{
	template <typename _Tp>
	inline void destroy(_Tp* _pointer)
	{
#if ISDIALOGUE

		std::cout << "destroy()的对象是"<<typeid(_Tp).name() << "\t";
		_pointer->~_Tp();
		std::cout << "destroy()的数值是" << *_pointer << "\n";
#else
		_pointer->~_Tp();
#endif
	}
	template<typename _ForwardIterator> //1
	inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, MyType_traits::__true_type) {}
	template<typename _ForwardIterator> //0
	inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, MyType_traits::__false_type)
	{
		for (; __first != __last; ++__first)
			destroy(&*__first);
	}
	template <typename _ForwardIterator, typename _Tp>
	inline void
		_destroy(_ForwardIterator _first, _ForwardIterator _last, _Tp*)
	{

		typedef typename MyType_traits::__type_traits<_Tp>::has_trivial_destructor Trivial_destructor;
		__destroy_aux(_first, _last, Trivial_destructor());
	}

	//析构函数
	template<typename _ForwardIterator>
	inline void destroy(_ForwardIterator first, _ForwardIterator last)
	{
		_destroy(first, last, MyIterator_traits::__value_type(first));
	}





	/////////////////////////////////////////////////////
	template <typename _T1, typename _T2>
	inline void construct(_T1* __p, const _T2& __value)  //针对构造函数的
	{
#if	ISDIALOGUE
		std::cout << "分配内存的类型是:" << typeid(_T1).name()<<'\t';
		std::cout << "构造函数__P的类型是" << typeid(_T1).name()<<'\t';
		new(__p)_T1( __value);
		std::cout << "构造的内容数值:"<<__value<<'\n';
#else
		new(__p)_T1(__value);
#endif
	}


}
#endif // !_MYCONSTRUCT_H_
