#include "my_list.h"
#include "my_allocator.h"
#include <list>
#include <memory>
#include <vector>
#include <tuple>
#include <type_traits>
#include "my_type_traits.h"
#include <sstream>
#include <unordered_map>
#include <memory>

class simpleClass
{
	char* m_buf;

public:
	int* m_count;

	simpleClass(size_t n = 1)
	{
		m_buf = new char[n];
		m_count = new int();
		*m_count = 1;
		std::cout << "Ctor" << "\n";
	}
	simpleClass(const simpleClass& T)
	{
		m_buf = T.m_buf;
		m_count = T.m_count;

		(*m_count)++;
		std::cout << "C:" << m_count << "\n";
	}
	~simpleClass()
	{
		(*m_count)--;
		if (*m_count == 0)
		{
			std::cout << "Dctor" << "\n";
			delete []m_buf;
			delete m_count;
		}
	}
};
class a {};
struct func
{
	int& i;
	func(int& i_) : i(i_) {}
};

void oops()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	some_local_state = 2;
	
}                              


template <typename T>
void sumasb(T a,  typename std::enable_if<std::is_integral<T>::value>::type*  = nullptr)
{
	std::cout << "int";
}

template <typename T>
void sumasb(T a, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr)
{
	std::cout << "float";
}
int main() {
	char i = '9';
	char b = '8';
	std::cout << i - b;
}
