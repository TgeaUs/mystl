#pragma once
#ifndef _MYITERATORTRAITS_H_
#define _MYITERATORTRAITS_H_
#include <cstddef> //ptrdiff_t
namespace MyIterator_traits
{
    struct output_iterator_tag {};//输入迭代器
    struct input_iterator_tag {} ; //输出迭代器
    struct forward_iterator_tag  :      public input_iterator_tag {}; //勇往直前迭代器
    struct bidirectional_iterator_tag : public forward_iterator_tag {}; //回马枪迭代器
    struct random_access_iterator_tag : public bidirectional_iterator_tag {}; //乱跳迭代器

    template <class _Iterator>
    struct iterator_traits {
        using iterator_category     = typename _Iterator::iterator_category;
        using value_type            = typename _Iterator::value_type       ;
        using difference_type       = typename _Iterator::difference_type  ;
        using pointer               = typename _Iterator::pointer          ;
        using reference             = typename _Iterator::reference        ;
    };
    //raw_pointer的针对
    template <class _Tp>
    struct iterator_traits<_Tp*>
    {
        using iterator_category      = random_access_iterator_tag;
        using value_type             = _Tp;		//表示你这个迭代器的类型
        using difference_type        = ptrdiff_t;	//表示距离 两个迭代器
        using reference              = value_type&;		// 从没有使用过
        using pointer                = value_type*;		// 从没有使用过
    };  
    template <class _Tp>
    struct iterator_traits<const _Tp*>
    {
        using iterator_category      = random_access_iterator_tag;
        using value_type             = _Tp;		//表示你这个迭代器的类型
        using difference_type        = ptrdiff_t;	//表示距离 两个迭代器
        using reference              = value_type&;		// 从没有使用过
        using pointer                = value_type*;		// 从没有使用过
    };

    //直接萃取value_type
    template<typename _iter>
    inline auto
        __value_type(const _iter&)->decltype(static_cast<typename iterator_traits<_iter>::value_type*>(0))
    {
        return  static_cast<typename iterator_traits<_iter>::value_type*>(0);
    }
    template<typename _iter>
    inline auto     //直接萃取__iterator_category
        __iterator_category(const _iter&)->decltype(iterator_traits<_iter>::iterator_category())
    {
        return iterator_traits<_iter>::iterator_category();
    }
    template<typename _iter>
    inline auto     //直接萃取__distance_type
        __distance_type(const _iter&)->decltype(static_cast<typename iterator_traits<_iter>::difference_type*>(0))
    {
        return static_cast<typename iterator_traits<_iter>::difference_type*>(0);
    }
}
#endif // !_MYITERATORTRAITS_H_
