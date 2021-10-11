#pragma once
#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H
#include <type_traits>
#include <memory>

#include "my_shared_count.h"
namespace ty
{
	template<class T> inline void checked_delete(T* x)
	{
		// intentionally complex - simplification causes regressions
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		(void)sizeof(type_must_be_complete);
		delete x;
		std::cout << "我调用了删除器"<<typeid(x).name() <<"\n";
	}

	template<class T> inline void checked_array_delete(T* x)
	{
		typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
		(void)sizeof(type_must_be_complete);
		delete[] x;
	}

	template<class T> struct checked_deleter
	{
		typedef void result_type;
		typedef T* argument_type;

		void operator()(T* x) const
		{
			// boost:: disables ADL
			checked_delete(x);
		}
	};

	template<class T> struct checked_array_deleter
	{
		typedef void result_type;
		typedef T* argument_type;

		void operator()(T* x) const
		{
			checked_array_delete(x);
		}
	};


	template<class T> class enable_shared_from_this
	{
	public:

		std::shared_ptr<T> shared_from_this()
		{
			std::shared_ptr<T> p(_internal_weak_this);

			return p;
		}

		std::shared_ptr<T const> shared_from_this() const
		{
			std::shared_ptr<T const> p(_internal_weak_this);

			return p;
		}

		typedef T _internal_element_type; // for bcc 5.5.1
		std::weak_ptr<_internal_element_type> _internal_weak_this;
	};

	template<class T, class Y>
	void sp_enable_shared_from_this(enable_shared_from_this<T>* pe, Y* px, shared_count const& pn)
	{
		//if (pe != 0) pe->_internal_weak_this._internal_assign(px, pn);
		std::cout << "Yes";
	}
	inline void sp_enable_shared_from_this(void const*, void const*, shared_count const&){}
	
	template <typename T>
	struct shared_ptr_traits
	{
		using reference = T&;
	};
	template <typename T>
	class shared_ptr
	{
		T* px;
		shared_count pn;
		typedef typename shared_ptr_traits<T>::reference this_reference;
	public:
		shared_ptr() :px(std::nullptr_t), pn() {}


		template<class Y>
		explicit shared_ptr(Y* p) : px(p), pn(p, checked_deleter<Y>())
		{
			sp_enable_shared_from_this(p, p, pn);
		}

		long use_count() const
		{
			return pn.use_count();
		}

		T* get() const
		{
			return px;
		}

		this_reference operator*()const
		{
			std::cout << "进来了operator*()"<<"\n";

			return *px;
		}
		T* operator-> () const // never throws
		{
			std::cout << "进来了operator->()" << "\n";

			return px;
		}
		template <typename T>
		shared_ptr& operator=(shared_ptr<T> const& r)
		{
			this->px = r.px;
			this->pn = r.pn;
			return *this;
		}

	};
}
#endif // ! MY_SHARED_PTR_H
