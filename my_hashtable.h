#pragma once
#ifndef MY_HASHTABLE_H_
#define MY_HASHTABLE_H_
#include <cstddef> //ptrdiff_t
#include "my_uninitialized.h"
#include "my_allocator.h"
#include "my_iterator_traits.h"
#include "my_iterator.h"
#include "my_config.h"
namespace MyHashtable
{

	template<typename val>
	struct hashtable_node //每一个挂的节点 是单链表
	{
		hashtable_node* next;
		val value_feild;
	};

	enum { stl_num_primes = 28 };
	static const unsigned long stl_prime_list[stl_num_primes] =
	{
	  53ul,         97ul,         193ul,       389ul,       769ul,
	  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
	  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
	  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
	  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
	  1610612741ul, 3221225473ul, 4294967291ul
	};

	//找出 上面的stl_prime_list中最接近n的质数当作篮子
	inline unsigned long stl_next_prime(unsigned long n)
	{
		const unsigned long* first = stl_prime_list;
		const unsigned long* last = stl_prime_list + (int)stl_num_primes;
		const unsigned long* pos =MyAlgorithm_base::lower_bound(first, last,n); //找出这个数组>=n的第一个数字 作为哈希表的大小
		return pos == last ? *(last - 1) : *pos;
	}

	template<typename value,  //值
		typename key,			//键
		typename hashfunction,  //函数的类型
		typename extractKey,  //取出key
		typename euqalKey, //检测碰撞
		typename allocator = MyAllocator::allocator<value>>
	class hashtable
	{

	public:
		typedef value			value_type;
		typedef key				key_type;
		typedef hashfunction	hasher;
		typedef euqalKey		key_equal;

		typedef size_t            size_type;
		typedef ptrdiff_t         difference_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		hasher hash_funct() const { return hash; }
		key_equal key_eq() const { return equals; }

		typedef  allocator allocator_type;
		typedef hashtable_node<value> node;
	private:
		hasher                hash;
		key_equal             equals;
		extractKey            get_key;
		MyVector::vector<node*, allocator_type> buckets;
		size_type             num_elements;


	};
}
#endif // !MY_HASHTABLE_H_
