#pragma once
#ifndef _TEST_H_
#define _TEST_H_
#include <iostream>

class base
{
public:	
	base* root=nullptr;
	base* next;

};

template <typename compare>
class MyClass:public base
{

public:
	int value = 1;

	MyClass() = default;
	friend std::ostream& operator<<(std::ostream& os, const MyClass& T)
	{
		return os << T.a<<*T.bbb;
	}

	MyClass operator*()
	{
		int operatorVariable=999;
		int* b = &operatorVariable;
		this->bbb = b;
		this->a = 3;
		return *this;
	}

	void operator[](int n)
	{
		
	}


};



#endif // !_TEST_H_
