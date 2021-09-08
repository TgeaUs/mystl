#pragma once
#ifndef _MY_TREE_H_
#define _MY_TREE_H_
#include "my_allocator.h"
#include "my_pair.h"
#include "my_type_traits.h"
#include "my_construct.h"
#include "my_algorithm_base.h"
#include "my_iterator_traits.h"
namespace MyTree
{

	//��ƪ��Ϊmap set multiset/map�ĵײ�����
	//Start: 2020/8/14

	//End::2020 /8/18
	
		
	 
	
	using rb_tree_Color_type = bool;
	const rb_tree_Color_type rb_tree_red = false; //��ɫ
	const rb_tree_Color_type rb_tree_black = true; //��ɫ




	struct rb_tree_node_base
	{
		using color_type = rb_tree_Color_type;
		using base_ptr = rb_tree_node_base*;

		color_type color;
		base_ptr parent; //����
		base_ptr left;
		base_ptr right;

		static base_ptr minimum(base_ptr x)
		{
			while (x->right != 0)
			{
				x = x->right;
			}
			return x;
		}

		static base_ptr maximum(base_ptr x)
		{
			while (x->left != 0)
			{
				x = x->left;
			}
			return x;
		}
	};
	template<typename value>
	class rb_tree_node :public rb_tree_node_base
	{
	public:
		using like_type = rb_tree_node<value>;
		value value_field;
	};

	//���˸о�������������� ��insert��ʱ�� ������rb_tree_node
	//�����Ƽ̳� ����û�취ת��
	template<typename value>
	class rb_tree_iterator :public rb_tree_node<value>
	{
	public:
		//ǰ��
		void increment()
		{
			if (node->right != 0)
			{
				node = node->right;
				while (node->left != 0)
				{
					node = node->left;
				}
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->right)
				{
					node = y;
					y = y->parent;
				}
				if (node->right != y)
				{
					node = y;
				}
			}
		}
		void decrement()
		{
			if (node->color == rb_tree_red && node->parent->parent == node)
			{
				node = node->right;
			}
			else if (node->left != 0)
			{
				base_ptr y = node->left;
				while (y->right != 0)
					y = y->right;
				node = y;
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->left)
				{
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}
	public:
		using value_type = value;
		using reference = value&;
		using difference_type = ptrdiff_t;
		using pointer = value*;


		using iterator_category = MyIterator_traits::bidirectional_iterator_tag;

		using self = rb_tree_iterator<value>;
		using base_ptr = rb_tree_node_base*;
		using link_type = rb_tree_node<value>*;
		using iterator = rb_tree_iterator<value>;
		base_ptr node;

		rb_tree_iterator(link_type __x) { node = __x; }
		rb_tree_iterator() {}

		reference operator*() const { return static_cast<link_type>(node)->value_field; }

		self& operator++() { increment(); return *this; }
		self& operator--() { decrement(); return *this; }

		self operator++(int)
		{
			self temp = *this;
			increment();
			return temp;
		}

		self operator--(int)
		{
			self temp = *this;
			decrement();
			return temp;
		}
		bool operator!=(const self& x)
		{
			return node != x.node;
		}

		bool operator==(const self& x)
		{
			return node == x.node;
		}
	};



	

	template<
		typename key,
		typename value,
		typename keyOfvalue,
		typename compare,
		typename allocator = MyAllocator::allocator<value>>
		class rb_tree
	{
	public:
		using size_type			= value;
		using size_t			= size_type;
		using reference			= value&;
		using const_reference	= const value&;
		using pointer			= value*;
		using const_pointer		= const value*;
		using difference_type	= ptrdiff_t;
		using self = rb_tree<key, value, keyOfvalue, compare>;

		using base_ptr = rb_tree_node_base*;
		//��ȡ�ڵ�
		using link_type = rb_tree_node<value>*;
		using tree_node_allocator = MyAllocator::allocator<rb_tree_node<value>>;

		//������
		using iterator		 = rb_tree_iterator<value>;
		using const_iterator =const rb_tree_iterator<value>;
		using allocator_type = allocator;

	protected:
		size_type node_count;
		compare key_compare;
		link_type header;

		using color_type = rb_tree_Color_type;

		static color_type& color(link_type x) { return x->color; } //ȡ����ɫ
		link_type get_node() const { return tree_node_allocator::allocate(1); } //ȡ���ڴ�
		link_type& root()const { return (link_type&)header->parent; }                   //ȡ�ø����c
		link_type& leftmost()const { return (link_type&)header->left; }
		link_type& rightmost()const { return (link_type&)header->right; }

		//��ȡ�ڵ�Ĳ���
		static reference get_value(link_type x) { return x->value_field; }
		static const key& get_key(link_type x) { return keyOfvalue()(get_value(x)); }
		static link_type& get_left(link_type x) { return (link_type&)x->left; }
		static link_type& get_right(link_type x) { return (link_type&)x->right; }
		static link_type& get_parent(link_type x) { return (link_type&)x->parent; }

		void destroy_node(link_type p)
		{
			MyConstruct::destroy(&p->value_field);
			tree_node_allocator::deallocate(p);
		}
		static const key& get_key(base_ptr __x)
		{
			return keyOfvalue()(get_value(link_type(__x)));
		}
		//���ݵĲ���
		//��ȡ�ڵ� ���ҷ����ڴ�+ֵ
		link_type create_node(const value& x)
		{
			link_type temp = get_node();
			try
			{
				MyConstruct::construct(&temp->value_field, x);

			}
			catch (...)
			{
				throw std::bad_alloc();
			}
			return temp;
		}
	private:
		void empty_initialize()
		{
			//header�ڵ�����Ϊ��ɫ
			color(header) = rb_tree_red;
			root() = 0; //���ڵ����ó� ��
			leftmost() = header;
			rightmost() = header;
		}
	public:
		//base=rb_tree_base����װ��header ����һ���ڴ沢ָ���� ��Ϊһ����Ԫ��
		rb_tree() :node_count(), key_compare()
		{
			header = get_node();
			empty_initialize();
		}
		rb_tree(const compare& comp, const allocator_type& a) :node_count(0), key_compare(comp)
		{
			header = get_node();
			empty_initialize();
		}
		iterator begin()				{ return static_cast<iterator*>(leftmost()); }
		const_iterator begin() const	{ return leftmost(); }
		iterator end()					{ return header; }
		const_iterator end() const		{ return header; }
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }
		//1=��ת�� 2=root
		void rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root)
		{
			rb_tree_node_base* y = x->right;
			x->right = y->left;
			if (y->left != 0)
				y->left->parent = x;
			y->parent = x->parent;
			if (x == root)
				root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}
		//1=��ת�� 2=root
		void rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root)
		{
			rb_tree_node_base* y = x->left;
			x->left = y->right;
			if (y->right != 0)
				y->right->parent = x;
			y->parent = x->parent;
			if (x == root)
				root = y;
			else if (x == x->parent->left)
				x->parent->right = y;
			else
				x->parent->left = y;
			y->right = x;
			x->parent = y;
		}

		//�˺��� ������ɫ������ƽ��
		//1=�����ڵ� 2=���ڵ�
		void rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root)
		{
			x->color = rb_tree_red; //�����Ľڵ�һ���Ǻ�ɫ
			while (x != root && x->parent->color == rb_tree_red)
			{
				//���
				if (x->parent == x->parent->parent->left)
				{
					rb_tree_node_base* y = x->parent->parent->right;
					if (y && y->color == rb_tree_red)
					{
						x->parent->color = rb_tree_black;
						y->color = rb_tree_black;
						x->parent->parent->color = rb_tree_red;
						x = x->parent->parent;
					}
					else//�޲����ڵ�
					{
						//�����LR ��������
						if (x == x->parent->right)
						{
							x = x->parent;
							rb_tree_rotate_left(x, root);
						}
						//�������LR ֱ�Ӹ���ɫ LL����
						x->parent->color = rb_tree_black;
						x->parent->parent->color = rb_tree_red;
						rb_tree_rotate_right(x->parent->parent, root);
					}
				}
				else //�ұ�
				{
					base_ptr y = x->parent->parent->left; //save
					if (y && y->color == rb_tree_red) //x�ĸ��ڵ��Ƿ�Ϊ��ɫ
					{
						x->parent->color = rb_tree_black; //�Ѹ��ڵ��Ϊ��ɫ
						y->color = rb_tree_black;        //�������óɺ�ɫ
						x->parent->parent->color = rb_tree_red; //�游���
						x = x->parent->parent;//������
					}
					else//�޲����ڵ�
					{
						//�����RL ��������
						if (x == x->parent->left)
						{
							x = x->parent;
							rb_tree_rotate_right(x, root);
						}
						//�������RR ֱ�Ӹ���ɫ RR����
						x->parent->color = rb_tree_black;
						x->parent->parent->color = rb_tree_red;
						rb_tree_rotate_left(x->parent->parent, root);
					}
				}

			}
			root->color = rb_tree_black;

		}


		// 1=�����Ĳ���λ��    2=���ڵ�           3=�����Ԫ��
		iterator _insert(base_ptr _x_root, base_ptr _y_header, const value& _x)
		{
			link_type _x_new_root = (link_type)_x_root;
			link_type _y_new_header = (link_type)_y_header;
			link_type _z_new_nodeData;

			//�ж��ǲ��Ǹ��ڵ� || xrootnew�������һ�� || compare�Ĵ�С����ô���� С=1=true ��=0=false
			if (_y_new_header == header || _x_new_root != 0 || compare()(_x, get_key(_y_new_header)))
			{
				_z_new_nodeData = create_node(_x);
				get_left(_y_new_header) = _z_new_nodeData; //���ص���ڵ�

				//���һ��Ԫ�ض�û�� ��ô�����Ԫ�ؾ��Ǹ��ڵ�
				if (_y_new_header == header)
				{
					root() = _z_new_nodeData;
					rightmost() = _z_new_nodeData;
				}
				else if (_y_new_header == leftmost())
				{
					leftmost() = _z_new_nodeData; //����header->leftmostָ����С�Ľڵ�
				}
			}
			else
			{
				_z_new_nodeData = create_node(_x);
				get_right(_y_new_header) = _z_new_nodeData;
				if (_y_new_header == rightmost())
					rightmost() = _z_new_nodeData; //���� header->leftmostָ�����Ľڵ�
			}
			get_parent(_z_new_nodeData) = _y_new_header;
			get_left(_z_new_nodeData) = 0;
			get_right(_z_new_nodeData) = 0;

			//����������һ��ƽ�� �Ȳ�д
			rb_tree_rebalance(_z_new_nodeData, header->parent);
			++node_count; //���� �м����ڵ�
			return static_cast<iterator*>(_z_new_nodeData);
		}


		iterator insert_equal(const value& x)
		{
			link_type y_header = header;
			link_type x_root = root();
			while (x_root != 0)
			{
				y_header = x_root;

				if (compare()(keyOfvalue()(x), get_key(x_root))) //Ĭ������
				{
					x_root = get_left(x_root);
				} //С�������
				else
				{
					x_root = get_right(x_root);
				} //������ұ�

			}
			return _insert(x_root, y_header, x); //Ȼ�����
		}

		MyPair::pair<iterator,bool>
			insert_unique(const value& v)
		{
			link_type y = header;
			link_type  x = root();
			bool comp = true;
			while (x != 0)
			{
				y = x;
				comp = key_compare(keyOfvalue()(v), get_key(x));
				x = comp ? get_left(x) : get_right(x);
			}
			iterator j = iterator(y);
			if (comp)
				if (j == begin())
					return MyPair::pair<iterator, bool>(_insert(x, y, v), true);
				else
					--j;
			if (key_compare(get_key(j.node), keyOfvalue()(v)))
				return MyPair::pair<iterator, bool>(_insert(x, y, v), true);
			return MyPair::pair<iterator, bool>(j, false);
		}


		iterator lower_bound(const key& k)
		{
			link_type y = header; /* Last node which is not less than k. */
			link_type x = root(); /* Current node. */

			while (x != 0)
				if (!key_compare(get_key(x), k))
					y = x, x = get_left(x);
				else
					x = get_right(x);

			return iterator(y);
		}
		const_iterator lower_bound(const key& k) const
		{
			link_type y = header;
			link_type x = root();

			while (x != 0)
				if (!key_compare(get_key(x), k))
					y = x, x = get_left(x);
				else
					x = get_right(x);

			return const_iterator(y);
		}
		iterator upper_bound(const key& k)
		{
			link_type y = header;
			link_type x = root();

			while (x != 0)
				if (key_compare(k, get_key(x)))
					y = x, x = get_left(x);
				else
					x = get_right(x);

			return iterator(y);
		}
		const_iterator upper_bound(const key& k) const
		{
			link_type y = header;
			link_type x = root();

			while (x != 0)
				if (key_compare(k, get_key(x)))
					y = x, x = get_left(x);
				else
					x = get_right(x);

			return const_iterator(y);
		}
		MyPair::pair<iterator, iterator> equal_range(const key& k)
		{
			return MyPair::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
		}

		MyPair::pair<const_iterator, const_iterator> equal_range(const key& k) const
		{
			return MyPair::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
		}

		size_type count(const key& k)const
		{
			MyPair::pair<const_iterator, const_iterator> p = equal_range(k);
			size_type n = 0;
			MyAlgorithm_base::distance(p.first, p.second, n);
			return n;
		}

		iterator find(const key& k)
		{
			link_type y = header;
			link_type x = root();
			
			while (x != 0)
				if (!key_compare(get_key(x), k))
					y = x, x = get_left(x);
				else
					x = get_right(x);

			iterator __j = iterator(y);
			//���ɹ� ����end() �ɹ��˷����ҵ��ĵ�����
			return (__j == end() || key_compare(k, get_key(__j.node))) ? end() : __j;
		}

		//1=ɾ����Ԫ�� 2=ɾ��Ԫ�صĸ� 3=��� 4=��С
		inline base_ptr
			rb_tree_rebalance_for_erase(
				base_ptr __z,
				base_ptr& __root,
				base_ptr& __leftmost,
				base_ptr& __rightmost)
		{
			rb_tree_node_base* __y = __z;
			rb_tree_node_base* __x = 0;
			rb_tree_node_base* __x_parent = 0;
			if (__y->left == 0)     // __z has at most one non-null child. y == z.
				__x = __y->right;     // __x might be null.
			else
				if (__y->right == 0)  // __z has exactly one non-null child. y == z.
					__x = __y->left;    // __x is not null.
				else {                   // __z has two non-null children.  Set __y to
					__y = __y->right;   //   __z's successor.  __x might be null.
					while (__y->left != 0)
						__y = __y->left;
					__x = __y->right;
				}
			if (__y != __z) {          // relink y in place of z.  y is z's successor
				__z->left->parent = __y;
				__y->left = __z->left;
				if (__y != __z->right) {
					__x_parent = __y->parent;
					if (__x) __x->parent = __y->parent;
					__y->parent->left = __x;      // __y must be a child of left
					__y->right = __z->right;
					__z->right->parent = __y;
				}
				else
					__x_parent = __y;
				if (__root == __z)
					__root = __y;
				else if (__z->parent->left == __z)
					__z->parent->left = __y;
				else
					__z->parent->right = __y;
				__y->parent = __z->parent;
				_STD swap(__y->color, __z->color);
				__y = __z;
				// __y now points to node to be actually deleted
			}
			else {                        // __y == __z
				__x_parent = __y->parent;
				if (__x) __x->parent = __y->parent;
				if (__root == __z)
					__root = __x;
				else
					if (__z->parent->left == __z)
						__z->parent->left = __x;
					else
						__z->parent->right = __x;
				if (__leftmost == __z)
					if (__z->right == 0)        // __z->left must be null also
						__leftmost = __z->parent;
				// makes __leftmost == header if __z == __root
					else
						__leftmost = rb_tree_node_base::minimum(__x);
				if (__rightmost == __z)
					if (__z->left == 0)         // __z->right must be null also
						__rightmost = __z->parent;
				// makes __rightmost == header if __z == __root
					else                      // __x == __z->left
						__rightmost = rb_tree_node_base::maximum(__x);
			}
			if (__y->color != rb_tree_red) {
				while (__x != __root && (__x == 0 || __x->color == rb_tree_black))
					if (__x == __x_parent->left) {
						rb_tree_node_base* __w = __x_parent->right;
						if (__w->color == rb_tree_red) {
							__w->color = rb_tree_black;
							__x_parent->color = rb_tree_red;
							rb_tree_rotate_left(__x_parent, __root);
							__w = __x_parent->right;
						}
						if ((__w->left == 0 ||
							__w->left->color == rb_tree_black) &&
							(__w->right == 0 ||
								__w->right->color == rb_tree_black)) {
							__w->color = rb_tree_red;
							__x = __x_parent;
							__x_parent = __x_parent->parent;
						}
						else {
							if (__w->right == 0 ||
								__w->right->color == rb_tree_black) {
								if (__w->left) __w->left->color = rb_tree_black;
								__w->color = rb_tree_red;
								rb_tree_rotate_right(__w, __root);
								__w = __x_parent->right;
							}
							__w->color = __x_parent->color;
							__x_parent->color = rb_tree_black;
							if (__w->right) __w->right->color = rb_tree_black;
							rb_tree_rotate_left(__x_parent, __root);
							break;
						}
					}
					else {                  // same as above, with right <-> left.
						rb_tree_node_base* __w = __x_parent->left;
						if (__w->color == rb_tree_red) {
							__w->color = rb_tree_black;
							__x_parent->color = rb_tree_red;
							rb_tree_rotate_right(__x_parent, __root);
							__w = __x_parent->left;
						}
						if ((__w->right == 0 ||
							__w->right->color == rb_tree_black) &&
							(__w->left == 0 ||
								__w->left->color == rb_tree_black)) {
							__w->color = rb_tree_red;
							__x = __x_parent;
							__x_parent = __x_parent->parent;
						}
						else {
							if (__w->left == 0 ||
								__w->left->color == rb_tree_black) {
								if (__w->right) __w->right->color = rb_tree_black;
								__w->color = rb_tree_red;
								rb_tree_rotate_left(__w, __root);
								__w = __x_parent->left;
							}
							__w->color = __x_parent->color;
							__x_parent->color = rb_tree_black;
							if (__w->left) __w->left->color = rb_tree_black;
							rb_tree_rotate_right(__x_parent, __root);
							break;
						}
					}
				if (__x) __x->color = rb_tree_black;
			}
			return __y;
		}

		void erase(iterator pos)
		{
			link_type __y = (link_type)rb_tree_rebalance_for_erase( //ɾ����Ҫ����ƽ��
				pos.node,
				header->parent, 
				header->left,
				header->right);

			destroy_node(__y);
			--node_count;

		}

		void erase(iterator first, iterator last)
		{
			/*if (first == begin() && last == end())
				clear();
			else*/
				while (first != last) 
					erase(first++);
		}

		size_type erase(const key& x)
		{
			MyPair::pair<iterator, iterator> p = equal_range(x);
			size_type n = 0;
			MyAlgorithm_base::distance(p.first, p.second, n); //����ɾ�˶��ٸ�
			erase(p.first, p.second);
			return n;
		}

	};
}
#endif // ! _MY_TREE_H_


