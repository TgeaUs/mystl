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
#include <unordered_map>
#include "my_shared_ptr.h"

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
		std::cout << "D" << "\n";
	}
	Test()
	{
		std::cout << "C" << "\n";
	}
	std::shared_ptr<Test> getObj()
	{
		std::shared_ptr<Test> temp(this);
		return temp;
	}

	virtual void print()
	{
		std::cout << "printA";
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
		std::cout << "我打印了printf" << "\n";
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
	//ty::shared_ptr<Test> myb(new Driver());
	//ty::shared_ptr<Test> myc = myb;

	//std::cout <<"计数器"<< myb.use_count() <<"\n";
	//std::cout << "\n" << "\n" << "\n";

	//std::shared_ptr<Test> stdb(new Driver());
	//std::shared_ptr<Test> stdc = stdb;
	//std::cout << "计数器" << stdb.use_count()<<"\n";

	
	std::vector<int> vc{ 1,2,3,4 };

	auto it = (MyAlgorithm_base::find_if(vc.begin(), vc.end(), [](int a) { return a == 3; }));

	*it = 4;
	for (auto i : vc)
		std::cout << i;
}