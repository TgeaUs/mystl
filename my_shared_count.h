#pragma once
#ifndef MY_SHARED_COUNT_H
#define MY_SHARED_COUNT_H
#include <type_traits>
#include <memory>
namespace ty
{

	class sp_counted_base
	{
		long use_count_;
		long weak_count_;
	public:
		sp_counted_base() : use_count_(1), weak_count_(1){}

		long use_count()const
		{
			return use_count_;
		}
	};
	template<class P, class D> 
	class sp_counted_base_impl : public sp_counted_base
	{
		P ptr; // copy constructor must not throw
		D del; // copy constructor must not throw
	public:
		sp_counted_base_impl(P p, D d) : ptr(p), del(d)
		{

		}
	};
	class shared_count
	{
		sp_counted_base* pi_;
	public:
		template<class P, class D> 
		shared_count(P p, D d) : pi_(0)
		{
			try
			{
				pi_ = new sp_counted_base_impl<P, D>(p, d);
			}
			catch (...)
			{
				d(p); //É¾³ýÆ÷
				throw;
			}
		}
		long use_count() const // nothrow
		{
			return pi_ != 0 ? pi_->use_count() : 0;
		}
		shared_count& operator=(const shared_count&)
		{
			std::cout << "½ð" <<"\n";
			return *this;
		}
	
	};
}
#endif // ! MY_SHARED_PTR_H
