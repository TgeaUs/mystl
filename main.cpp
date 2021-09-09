#include "my_list.h"
#include "my_allocator.h"
#include <list>
#include <memory>
#include <type_traits>
#include "my_type_traits.h"
#include <sstream>
struct obj
{
	obj* next;
};
template <typename T>
void sum(T res,T aa)
{
	std::cout << "hello";
}

template <typename T, typename... args>
void sum(T res, const args&... a)
{
	std::cout << sizeof...(a)<<"\n";
	std::cout << res << "\n";
	sum(a...);

}

template <typename, typename>
constexpr bool is_same_x = false;
template <typename Ty>
constexpr bool is_same_x<Ty, Ty> = true;

template <class _Ty>
void remove_cvv(volatile _Ty& a)
{
	using typo = _Ty;
	std::cout << typeid(_Ty).name();

int main()
{
	
    /*int	*a =(int*)MyAllocator::__default_alloc_template::allocate(1);
	int *b =(int*)MyAllocator::__default_alloc_template::allocate(1);
	int *c =(int*)MyAllocator::__default_alloc_template::allocate(1);
	int *d =(int*)MyAllocator::__default_alloc_template::allocate(1);
	*a = 20;
	*b = 20;
	*c = 20;
	*d = 20;
	std::cout << *a<<"\n"; 
	std::cout << *b<<"\n";
	std::cout << *c<<"\n";
	std::cout << *d<<"\n";*/


	
	std::cout << MyType_traits::is_enum< E>::value;
	return 0;
	
}