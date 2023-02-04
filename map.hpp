#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <functional>
#include <memory>

#include <vector>
#include "vector.hpp"
#include <iostream>

#include "pair.hpp"
#include "utility.hpp"
#include "lexicographical_compare.hpp"
#include "swap.hpp"
#include "reverse_iterator.hpp"

namespace ft {

	// Node
	template <class V>
	class Node {
		template <class A, class B, class C, class D>
		friend class map;
		
		template <class E>
		friend class Iterator;

		private:
			V*			data;
			Node*		left;
			Node*		right;
			Node*		parent;
			int			height;

		private:
			// Constructors
			Node() : data(nullptr), left(nullptr), right(nullptr), parent(nullptr), height(0) {}

			template <class Allocator>
			Node(const V& value, Allocator& allocator)
			: data(nullptr), left(nullptr), right(nullptr), parent(nullptr), height(0)
			{
				data = allocator.allocate(1);
				allocator.construct(data, value);
			}

			operator	Node<const V>() { return reinterpret_cast<Node<const V>& >(*this); }


		private:
			Node*	left_most() {
				Node*	node = this;

				while (node->left) {
					node = node->left;
				}

				return node;
			}

			Node*	right_most() {
				Node*	node = this;

				while (node->right) {
					node = node->right;
				}

				return node;
			}

			// Red-Black-Tree
			// unused
			Node*	get_sibling() {
				return ((parent->left == this) ? parent->right : parent->left);
			}

			Node*	get_uncle() {
				return parent->get_sibling();
			}
			// ^ unused

			// AVL-Tree
			static int	get_height(Node* node) {
				return node ? node->height : -1;
			}

			static void	update_height(Node* node) {
				int	left_height = get_height(node->left);
				int	right_height = get_height(node->right);

				node->height = ((left_height > right_height) ? left_height : right_height) + 1;
			}

			static int	balance_factor(Node* node) {
				return get_height(node->right) - get_height(node->left);
			}

			static Node*	rotate_right(Node* node) {
				Node*	left = node->left;

				if (node->parent) {
					(node->parent->left == node ? node->parent->left : node->parent->right) = left;
				}
				left->parent = node->parent;

				node->left = left->right;
				if (left->right && left->right->parent) {
					left->right->parent = node;
				}

				left->right = node;
				node->parent = left;

				update_height(node);
				update_height(left);

				return left;
			}

			static Node*	rotate_left(Node* node) {
				Node*	right = node->right;

				if (node->parent) {
					(node->parent->left == node ? node->parent->left : node->parent->right) = right;
				}
				right->parent = node->parent;

				node->right = right->left;
				if (node->right) {
					node->right->parent = node;
				}

				right->left = node;
				node->parent = right;

				update_height(node);
				update_height(right);

				return right;
			}

			static Node*	rebalance(Node* node) {
				update_height(node);

				int	bf = balance_factor(node);
				if (bf < -1) {
					if (balance_factor(node->left) <= 0) {
						node = rotate_right(node);
					} else {
						node->left = rotate_left(node->left);
						node = rotate_right(node);
					}
				}

				if (bf > 1) {
					if (balance_factor(node->right) >= 0) {
						node = rotate_left(node);
					} else {
						node->right = rotate_right(node->right);
						node = rotate_left(node);
					}
				}

				return node;
			}
	};


	// Iterator
	template <class V>
	class Iterator {
		template <class A, class B, class C, class D>
		friend class map;

		public:
			typedef	V	value_type;
			typedef	V*	pointer;
			typedef	V&	reference;
			typedef	std::ptrdiff_t	difference_type;
			typedef	std::bidirectional_iterator_tag	iterator_category;

		private:
			Node<V>*	_base;

		public:
			Iterator() : _base(nullptr) {}
			Iterator(Node<V>* base) : _base(base) {}
			Iterator(const Iterator& other) : _base(nullptr) { *this = other; }

			operator	Iterator<const V>() const { return Iterator<const V>(reinterpret_cast<Node<const V>* >(_base)); }

			Iterator&	operator=(const Iterator& other) {
				if (this != &other) {
					_base = other._base;
				}

				return *this;
			}

			V&	operator*() const {
				return *(_base->data);
			}

			V*	operator->() const {
				return _base->data;
			}

			Iterator&	operator++() {
				if (!_base) {
					return *this;
				}

				if (_base->right) {
					_base = (_base->right)->left_most();
					return *this;
				}

				Node<V>*	child = _base;
				Node<V>*	parent = _base->parent;
				while (parent && child == parent->right) {
					child = parent;
					parent = parent->parent;
				}
				_base = parent;

				return *this;
			}

			Iterator	operator++(int) {
				Iterator	it = *this;
				++*this;
				return it;
			}

			Iterator&	operator--() {
				if (!_base) {
					return *this;
				}

				if (_base->left) {
					_base = _base->left->right_most();
					return *this;
				}

				Node<V>*	child = _base;
				Node<V>*	parent = _base->parent;
				while (parent && child == parent->left) {
					child = parent;
					parent = parent->parent;
				}
				_base = parent;

				return *this;
			}

			Iterator	operator--(int) {
				Iterator	it = *this;
				--*this;
				return it;
			}

			friend bool	operator==(const Iterator& lhs, const Iterator& rhs) {
				return lhs._base == rhs._base;
			}

			friend bool	operator!=(const Iterator& lhs, const Iterator& rhs) {
				return !(lhs == rhs);
			}
	};


	// map
	template <
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> >
	>
	class map {
		public:
			typedef	Key									key_type;
			typedef	T									mapped_type;
			typedef	ft::pair<const Key, T>				value_type;
			typedef	size_t								size_type;
			typedef	ptrdiff_t							difference_type;
			typedef	Compare								key_compare;
			typedef	Allocator							allocator_type;
			typedef	value_type&							reference;
			typedef	const value_type&					const_reference;
			typedef	typename Allocator::pointer			pointer;
			typedef	typename Allocator::const_pointer	const_pointer;
			typedef	Iterator<value_type>				iterator;
			typedef	Iterator<const value_type>			const_iterator;
			typedef	reverse_iterator<iterator>			reverse_iterator;
			typedef	ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		public:
			class value_compare {
				public:
					typedef	bool		result_type;
					typedef	value_type	first_argument_type;
					typedef	value_type	second_argument_type;

				protected:
					Compare	comp;

				public:
					value_compare() : comp() {}
					bool	operator()(const value_type& lhs, const value_type& rhs) const {
						return comp(lhs.first, rhs.first);
					}

				protected:
					value_compare(Compare c) : comp(c) {}
			};

		private:
			Node<value_type>*					_root;
			size_t								_size;
			Compare								_compare;
			Allocator							_allocator;
			std::allocator<Node<value_type> >	_node_allocator;
			Node<value_type>					_end;

		public:
			// Constructor
			map() : _root(nullptr), _size(0), _compare(), _allocator(), _node_allocator(), _end() {}
			map(const Compare& comp, const Allocator& alloc) : _root(nullptr), _size(0), _compare(comp), _allocator(alloc), _node_allocator(), _end() {}
			template <class InputIt>
			map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : _root(nullptr), _size(0), _compare(comp), _allocator(alloc), _node_allocator(), _end() {
				while (first != last) {
					insert(*first);
					++first;
				}
			}
			map(const map& other) : _root(nullptr), _size(0), _compare(), _allocator(), _node_allocator(), _end() { *this = other; }


			// Destructor
			~map() {
				clear();
			}


			map&	operator=(const map& other) {
				if (this != &other) {
					clear();
					_compare = other._compare;
					_allocator = other._allocator;
					_node_allocator = other._node_allocator;
					_end = other._end;
					for (const_iterator it = other.begin(); it != other.end(); ++it) {
						insert(*it);
					}
				}

				return *this;
			}


			allocator_type	get_allocator() const {
				return _allocator;
			}


			// Element access
			T&	at(const Key& key) {
				iterator pos = find(key);

				if (pos == end()) {
					throw std::out_of_range("ft::map at");
				}

				return pos->second;
			}

			const T&	at(const Key& key) const {
				iterator pos = find(key);

				if (pos == end()) {
					throw std::out_of_range("ft::map const at");
				}

				return pos->second;
			}


			T&	operator[](const Key& key) {
				return (insert(ft::make_pair(key, T())).first->second);
			}


			// Iterators
			iterator	begin() {
				if (_root) {
					return iterator(_root->left_most());
				}

				return end();
			}

			const_iterator	begin() const {
				if (_root) {
					return iterator(_root->left_most());
				}

				return end();
			}

			iterator	end() {
				return iterator(&_end);
			}

			const_iterator	end() const {
				return iterator((Node<value_type>*)&_end);
			}

			reverse_iterator	rbegin() {
				if (size()) {
					return reverse_iterator(end());
				}

				return rend();
			}

			const_reverse_iterator	rbegin() const {
				if (size()) {
					return const_reverse_iterator(end());
				}

				return rend();
			}

			reverse_iterator	rend() {
				return reverse_iterator(begin());
			}

			const_reverse_iterator	rend() const {
				return const_reverse_iterator(begin());
			}


			// Capacity
			bool	empty() const {
				return _size == 0;
			}

			size_type	size() const {
				return _size;
			}

			size_type	max_size() const {
				return _allocator.max_size();
			}


			// Modifiers
			void	clear() {
				while (_size) {
					erase(_root);
				}
			}

			
			iterator	insert(iterator pos, const value_type& value) {
				(void)pos;
				return insert(value).first;
			}

			template <class InputIt>
			void	insert(InputIt first, InputIt last) {
				while (first != last) {
					insert(*first);
					++first;
				}
			}


			void	erase(iterator first, iterator last) {
				ft::vector<ft::pair<Key, T> >	buf(first, last);
				for (typename ft::vector<ft::pair<Key, T> >::iterator it = buf.begin(); it != buf.end(); ++it) {
					erase(it->first);
				}
			}

			size_type	erase(const Key& key) {
				size_type	size_tmp = size();
				iterator	it = find(key);
				if (it != end() && it._base)
					erase(it);
				return size_tmp == size() ? 0 : 1;
			}


			void	swap(map& other) {
				ft::swap(_root, other._root);
				ft::swap(_size, other._size);
				ft::swap(_compare, other._compare);
				ft::swap(_allocator, other._allocator);
				ft::swap(_node_allocator, other._node_allocator);
				ft::swap(_end, other._end);

				if (_root)
				_root->parent = &_end;
				if (other._root)
					other._root->parent = &other._end;
			}


			// Lookup
			size_type	count(const Key& key) const {
				return find(key) == end() ? 0 : 1;
			}


			ft::pair<iterator, iterator>	equal_range(const Key& key) {
				return (ft::make_pair(lower_bound(key), upper_bound(key)));
			}

			ft::pair<const_iterator, const_iterator>	equal_range(const Key& key) const {
				return (ft::make_pair(lower_bound(key), upper_bound(key)));
			}


			iterator	lower_bound(const Key& key) {
				for (iterator it = begin(); it != end(); ++it) {
					if (!_compare(it->first, key)) {
						return it;
					}
				}

				return end();
			}

			const_iterator	lower_bound(const Key& key) const {
				for (const_iterator it = begin(); it != end(); ++it) {
					if (!_compare(it->first, key)) {
						return it;
					}
				}

				return end();
			}


			iterator	upper_bound(const Key& key) {
				for (iterator it = begin(); it != end(); ++it) {
					if (_compare(key, it->first)) {
						return it;
					}
				}

				return end();
			}

			const_iterator	upper_bound(const Key& key) const {
				for (const_iterator it = begin(); it != end(); ++it) {
					if (_compare(key, it->first)) {
						return it;
					}
				}

				return end();
			}


			// Observers
			key_compare	key_comp() const {
				return _compare;
			}

			value_compare	value_comp() const {
				return value_compare();
			}


			iterator	find(const Key& key) const {
				Node<value_type>* node = _root;

				while (node) {
					if (less(key, node->data->first)) {
						node = node->left;
					} else if (greater(key, node->data->first)) {
						node = node->right;
					} else {
						return iterator(node);
					}
				}

				return iterator((Node<value_type>*)&_end);
			}

			ft::pair<iterator, bool>	insert(const ft::pair<const Key, T>& value) {
				Node<value_type>*	node = _root;
				Node<value_type>**	next;

				if (_root == nullptr) {
					_root = _node_allocator.allocate(1);
					_node_allocator.construct(_root, Node<value_type>(value, _allocator));
					++_size;
					Node<value_type>::rebalance(_root);
					_root->parent = &_end;
					_end.left = _root;
					return ft::make_pair(_root, true);
				}

				while (true) {
					if (less(value.first, node->data->first)) {
						next = &node->left;
					} else if (greater(value.first, node->data->first)) {
						next = &node->right;
					} else {
						return ft::make_pair(node, false);
					}

					if (*next) {
						node = *next;
					} else {
						*next = _node_allocator.allocate(1);
						_node_allocator.construct(*next, Node<value_type>(value, _allocator));
						(*next)->parent = node;
						(less(value.first, node->data->first) ? node->left : node->right) = *next;
						++_size;

						Node<value_type>*	tmp = *next;
						// wander from the inserted node back to the root and rebalance
						do {
							node = Node<value_type>::rebalance(node);
							if (node->parent == &_end) {
								_root = node;
								_root->parent = &_end;
								_end.left = _root;
								break ;
							}
							node = node->parent;
						} while (true);

						return ft::make_pair(tmp, true);
					}
				}
			}

			void	erase(iterator pos) {
				Node<value_type>*	node = pos._base;
				Node<value_type>*	parent = node->parent;

				if (!node->left && !node->right) {	// no children
					parent = node->parent;
					if (node->parent)
						((node->parent->left == node) ? node->parent->left : node->parent->right) = nullptr;
					
					_allocator.destroy(node->data);
					_allocator.deallocate(node->data, 1);
					_node_allocator.destroy(node);
					_node_allocator.deallocate(node, 1);

					--_size;
				} else if (!node->left) {	// only a right child
					parent = node->parent;
					if (node->parent)
						((node->parent->left == node) ? node->parent->left : node->parent->right) = node->right;
					if (node->right) { node->right->parent = node->parent; }

					if (node == _root) { _root = node->right; }

					_allocator.destroy(node->data);
					_allocator.deallocate(node->data, 1);

					_node_allocator.destroy(node);
					_node_allocator.deallocate(node, 1);

					--_size;
				} else if (!node->right) {	// only a left child
					parent = node->parent;
					if (node->parent)
						((node->parent->left == node) ? node->parent->left : node->parent->right) = node->left;
					if (node->left) { node->left->parent = node->parent; }

					if (node == _root) { _root = node->left; }

					_allocator.destroy(node->data);
					_allocator.deallocate(node->data, 1);

					_node_allocator.destroy(node);
					_node_allocator.deallocate(node, 1);

					--_size;
				} else {	// two children
					Node<value_type>*	in_order_successor = node->right;

					while (in_order_successor->left) {
						in_order_successor = in_order_successor->left;
					}

					ft::swap(node->data, in_order_successor->data);
					erase(iterator(in_order_successor));
				}

				// rebalance
				if (parent != &_end) {
					do {
						Node<value_type>::rebalance(parent);
						if (parent->parent == &_end) {
							_root = parent;
							_root->parent = &_end;
							_end.left = _root;
							break ;
						}
						parent = parent->parent;
					} while (true);
				}

				if (!_size) { _root = nullptr; }
			}

		private:
			bool	equal(const Key& lhs, const Key& rhs) const {
				return !_compare(lhs, rhs) && !_compare(rhs, lhs);
			}

			bool	less(const Key& lhs, const Key& rhs) const {
				return _compare(lhs, rhs);
			}

			bool	greater(const Key& lhs, const Key& rhs) const {
				return _compare(rhs, lhs);
			}
	};

	template <class Key, class T, class Compare, class Alloc>
	bool	operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			typename map<Key, T, Compare, Alloc>::const_iterator	lhs_current = lhs.begin();
			typename map<Key, T, Compare, Alloc>::const_iterator	lhs_end = lhs.end();
			typename map<Key, T, Compare, Alloc>::const_iterator	rhs_current = rhs.begin();

			while (lhs_current != lhs_end) {
				if (lhs_current->first != rhs_current->first || lhs_current->second != rhs_current->second) {
					return false;
				}
				++lhs_current;
				++rhs_current;
			}

			return true;
		}

		return false;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return !(lhs > rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	void	swap(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}

#endif
