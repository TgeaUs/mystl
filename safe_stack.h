//#pragma once
//#ifndef SAFE_STACK_H_
//#define SAFE_STACK_H_
//#include <memory>
//#include "my_construct.h"
//namespace ty
//{
//
//	template <typename T>
//	class stack_impl
//	{
//	protected:
//		T* v_;
//		size_t vsize_;
//		size_t vused_;
//
//		stack_impl<T> (const stack_impl<T>&) = delete;
//		stack_impl<T>& operator = (const stack_impl<T>&) = delete;
//
//		stack_impl<T>(size_t size = 0): v_(static_cast<T*>(size == 0 ? 0 : ::operator new(sizeof(T) * size))), vsize_(size), vused_(0){}
//		~stack_impl()
//		{
//			MyConstruct::destroy(v_, v_ + vused_);
//			::operator delete(v_);
//		}
//		void Swap(stack_impl& other) noexcept
//		{
//			std::swap(v_, other.v_);
//		}
//
//		
//	};
//	template <typename T>
//	class safe_stack : private stack_impl<T>
//	{
//	public:
//		safe_stack(size_t size = 0) :stack_impl<T>(size){}
//		safe_stack(const safe_stack& other) : stack_impl<T>(other.vused_)
//		{
//			while (vused_ < other.vused_)
//			{
//				MyConstruct::construct(v_ + vused_, other.v_[vused_]);
//				++vused_;
//			}
//		}
//		safe_stack& operator = (const safe_stack& other)
//		{
//			safe_stack temp(other);
//			/*std::swap(this->v_, temp.v_);
//			std::swap(this->vused_, temp.vused_);
//			std::swap(this->vsize_, temp.vsize_);*/
//			return *this;
//		}
//
//	};
//}
//
//#endif // !SAFE_STACK_H_
