#pragma once
#ifndef _MYITERATORTRAITS_H_
#define _MYITERATORTRAITS_H_
#include <cstddef> //ptrdiff_t
namespace MyIterator_traits
{
    struct output_iterator_tag {};//���������
    struct input_iterator_tag {} ; //���������
    struct forward_iterator_tag  :      public input_iterator_tag {}; //����ֱǰ������
    struct bidirectional_iterator_tag : public forward_iterator_tag {}; //����ǹ������
    struct random_access_iterator_tag : public bidirectional_iterator_tag {}; //����������

    template <class _Iterator>
    struct iterator_traits {
        using iterator_category     = typename _Iterator::iterator_category;
        using value_type            = typename _Iterator::value_type       ;
        using difference_type       = typename _Iterator::difference_type  ;
        using pointer               = typename _Iterator::pointer          ;
        using reference             = typename _Iterator::reference        ;
    };
    //raw_pointer�����
    template <class _Tp>
    struct iterator_traits<_Tp*>
    {
        using iterator_category      = random_access_iterator_tag;
        using value_type             = _Tp;		//��ʾ�����������������
        using difference_type        = ptrdiff_t;	//��ʾ���� ����������
        using reference              = value_type&;		// ��û��ʹ�ù�
        using pointer                = value_type*;		// ��û��ʹ�ù�
    };  
    template <class _Tp>
    struct iterator_traits<const _Tp*>
    {
        using iterator_category      = random_access_iterator_tag;
        using value_type             = _Tp;		//��ʾ�����������������
        using difference_type        = ptrdiff_t;	//��ʾ���� ����������
        using reference              = value_type&;		// ��û��ʹ�ù�
        using pointer                = value_type*;		// ��û��ʹ�ù�
    };

    //ֱ����ȡvalue_type
    template<typename _iter>
    inline auto
        __value_type(const _iter&)->decltype(static_cast<typename iterator_traits<_iter>::value_type*>(0))
    {
        return  static_cast<typename iterator_traits<_iter>::value_type*>(0);
    }
    template<typename _iter>
    inline auto     //ֱ����ȡ__iterator_category
        __iterator_category(const _iter&)->decltype(iterator_traits<_iter>::iterator_category())
    {
        return iterator_traits<_iter>::iterator_category();
    }
    template<typename _iter>
    inline auto     //ֱ����ȡ__distance_type
        __distance_type(const _iter&)->decltype(static_cast<typename iterator_traits<_iter>::difference_type*>(0))
    {
        return static_cast<typename iterator_traits<_iter>::difference_type*>(0);
    }
}
#endif // !_MYITERATORTRAITS_H_
