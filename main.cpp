#include "my_list.h"
#include "my_allocator.h"
#include <list>
#include <memory>
#include <vector>
#include <tuple>
#include <type_traits>
#include "my_type_traits.h"
#include <sstream>

class simpleClass
{
	char* m_buf;
	int* m_count;

public:
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
int main()
{

	std::cout << std::is_lvalue_reference_v<int&&>;
	std::cout << MyType_traits::is_lvalue_reference_v<int&&>;

	return 0;
	
}