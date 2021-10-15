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
		virtual void dispose() = 0; // nothrow
		virtual void destruct() // nothrow
		{
			delete this;
		}
		long use_count()const
		{
			return use_count_;
		}
		void add_ref()
		{
			++use_count_;
			++weak_count_;
		}		
		void weak_add_ref() // nothrow
		{
			++weak_count_;
		}
		void release()
		{
			{
				long new_use_count = --use_count_;

				if (new_use_count != 0)
				{
					--weak_count_;
					return;
				}
			}
			dispose();
			weak_release();
		}

		void weak_release() // nothrow
		{
			long new_weak_count;
			{
				new_weak_count = --weak_count_;
			}

			if (new_weak_count == 0)
			{
				destruct();
			}
		}
	};


	template<class P, class D> 
	class sp_counted_base_impl : public sp_counted_base
	{
		P ptr; // copy constructor must not throw
		D del; // copy constructor must not throw
	public:
		sp_counted_base_impl(P p, D d) : ptr(p), del(d){}
		virtual void dispose()
		{
			del(ptr);
		}
	};


	class shared_count
	{
		sp_counted_base* pi_;
	public:
		shared_count() : pi_(0) {}
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
		~shared_count()
		{
			if (pi_ != 0) pi_->release();
		}
		long use_count() const // nothrow
		{
			return pi_ != 0 ? pi_->use_count() : 0;
		}
		shared_count& operator=(const shared_count& r)
		{
			std::cout << "¼ÆÊýÆ÷µÄoperator=" <<"\n";
			sp_counted_base* temp = r.pi_;
			if (temp != nullptr)
				temp->add_ref();
			if (pi_ != nullptr)
				pi_->release();
			pi_ = temp;
			return *this;
		}


		void swap(shared_count& r) // nothrow
		{
			sp_counted_base* tmp = r.pi_;
			r.pi_ = pi_;
			pi_ = tmp;
		}
	
	};
}
#endif // ! MY_SHARED_PTR_H
