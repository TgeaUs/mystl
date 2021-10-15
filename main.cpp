#include "my_list.h"
#include "my_allocator.h"
#include <list>
#include <memory>
#include <vector>
#include <tuple>
#include <type_traits>
#include <utility>
#include "my_utility.h"
#include "my_type_traits.h"
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "my_shared_ptr.h"
#include <ranges>
#include "safe_stack.h"
struct Algin
{
	char a; //1
	short b; //2
	int c; //4
	double d; //8
	
	
};

template <typename Ty>
void sum(Ty[])
{
	std::cout << "233";
}


template <typename Ty, size_t N>
void sum(Ty[N])
{
	std::cout << "1111";
}

class Test
{
public:
	virtual ~Test()
	{
		std::cout << "D1" << "\n";
	}
	Test()
	{
		std::cout << "C1" << "\n";
	}
	std::shared_ptr<Test> getObj()
	{
		std::shared_ptr<Test> temp(this);
		return temp;
	}

	virtual void print()
	{
		std::cout << "print Test\n";
	}

};
class Test2
{
public:
	virtual ~Test2()
	{
		std::cout << "D2" << "\n";
	}
	Test2()
	{
		std::cout << "C2" << "\n";
	}
	std::shared_ptr<Test2> getObj()
	{
		std::shared_ptr<Test2> temp(this);
		return temp;
	}

	virtual void print()
	{
		std::cout << "print Test2\n";
	}

};

class Driver : public Test
{
public:
	~Driver()
	{
		std::cout << "DriverD" << "\n";
	}
	Driver()
	{
		std::cout << "DriverC" << "\n";
	}
	void print() override
	{
		std::cout << "ÎÒ´òÓ¡ÁËprintf" << "\n";
	}
};
using namespace std;
#include <functional>
class A
{
public:
	using pf = int(*)(int) ;
	static std::function<void(int*)> b;
};

void sum( void(A::* function)(int) )
{

}
template <size_t size>
class xxx
{
	double a;
	int b;
public:
	xxx()
	{
		std::cout << "C";
	}
	template <size_t osize>
	xxx& operator=(const xxx<osize>& other)
	{
		std::cout << size;
		std::cout << osize;
		return *this;
	}
	xxx<15> aaaaa(xxx<15> b)
	{
		return b;
	}
};

void alg(std::input_iterator_tag)
{
	std::cout << "input_iterator_tag";
}
int main()
{
	

	std::string a = "233";

	std::cout << a.substr(2, 2);
	

}