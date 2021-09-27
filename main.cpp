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
		std::cout << "driver" << "\n";
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
int main()
{
	sum(A::b);


	//ty::shared_ptr<Test> myb(new Driver());
	//myb->print();
	//(*myb).print();
	//ty::shared_ptr<Test> myc(new Driver());
	//myc = myb;
	//std::cout <<"计数器"<< myb.use_count() <<"\n";


	//std::shared_ptr<Test> stdb(new Driver());
	//stdb->print();
	//(*stdb).print();
	//std::shared_ptr<Test> stdc = stdb;
	//std::cout << "计数器" << stdb.use_count()<<"\n";

	

}