#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <functional>
#include <memory>

#include "pair.hpp"
#include "utility.hpp"
#include "lexicographical_compare.hpp"

// Debugging
#define PRINTLN(x) std::cout << (x) << std::endl
#define PRINT(x) std::cout << (x)
#define LINE PRINTLN(__LINE__)
#define POS std::cout << __FILE__ << ": " << __LINE__ << std::endl

namespace ft {

	template <class E>
	class Iterator;

	// Node
	template <class V>
	class Node {
		template <class A, class B, class C, class D>
		friend class AVLTree;
		
		template <class E>
		friend class Iterator;

		private:
			V*			data;
			Node*		left;
			Node*		right;
			Node*		parent;
			size_t		height;

		public:
			// Constructors
			template <class Allocator>
			Node(const V& value, Allocator& allocator)
			: data(nullptr), left(nullptr), right(nullptr), parent(nullptr), height(0)
			{
				data = allocator.allocate(1);
				allocator.construct(data, value);
			}

			// Node() : data(nullptr), left(nullptr), right(nullptr), parent(nullptr), height(0) {}

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
	};


	// Iterator
	template <class V>
	class Iterator {
		template <class A, class B, class C, class D>
		friend class AVLTree;

		private:
			Node<V>*	_base;

		public:
			Iterator() : _base(nullptr) {}
			Iterator(Node<V>* base) : _base(base) {}
			Iterator(const Iterator& other) : _base(nullptr) { *this = other; }

			operator	Iterator<const V>() { return reinterpret_cast<Iterator<const V>& >(*this); }

			Iterator&	operator=(const Iterator& other) {
				if (this != &other) {
					_base = other._base;
				}

				return *this;
			}

			V&	operator*() {
				return *(_base->data);
			}

			V*	operator->() {
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
				++*this;
				return it;
			}

			friend bool	operator==(const Iterator& lhs, const Iterator& rhs) {
				return lhs._base == rhs._base;
			}

			friend bool	operator!=(const Iterator& lhs, const Iterator& rhs) {
				return !(lhs == rhs);
			}
	};


	// AVLTree
	template <
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> >
	>
	class AVLTree {
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
						comp(lhs.first, rhs.first);
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

		public:
			// Constructor
			AVLTree() : _root(nullptr), _size(0), _compare(), _allocator() {}
			AVLTree(const Compare& comp, const Allocator& alloc) : _root(nullptr), _size(0), _compare(comp), _allocator(alloc) {}
			template <class InputIt>
			AVLTree(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : _root(nullptr), _size(0), _compare(comp), _allocator(alloc) {
				while (first != last) {
					insert(*first);
					++first;
				}
			}
			AVLTree(const AVLTree& other) : _root(nullptr), _size(0), _compare(), _allocator() { *this = other; }


			// Destructor
			~AVLTree() {
				//
			}


			AVLTree&	operator=(const AVLTree& other) {
				if (this != &other) {
					//
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
					// return const_iterator(_root->left_most());	// why does this not work
					return iterator(_root->left_most());
				}

				return end();
			}

			iterator	end() {
				if (_root) {
					return iterator(_root->parent);
				}

				return iterator(nullptr);
			}

			const_iterator	end() const {
				if (_root) {
					// return const_iterator(&reinterpret_cast<Node<const ft::pair<const Key, T> >& >(_root->parent));	// works fine
					// return const_iterator(_root->parent);	// why does this not work
					return iterator(_root->parent);
				}

				return const_iterator(nullptr);
			}


			// Capacity
			bool	empty() const {
				return _size == 0;
			}

			size_type	size() const {
				return _size;
			}

			size_type	max_size() const {
				_allocator.max_size();
			}


			// Modifiers
			void	clear() {
				while (_size) {
					erase(_root);
				}
			}

			void	swap(AVLTree& other) {
				ft::swap(_root, other._root);
				ft::swap(_size, other._size);
				ft::swap(_compare, other._compare);
				ft::swap(_allocator, other._allocator);
				ft::swap(_node_allocator, other._node_allocator);
			}


			// Lookup
			size_type	count(const Key& key) const {
				return !(find(key) == end());
			}


			ft::pair<iterator, iterator>	equal_range(const Key& key) {
				iterator first = lower_bound(key);
				if (first == end()) {
					return ft::make_pair(end(), end());
				} else if (_compare(key, first->first)) {
					return ft::make_pair(first, first);
				} else {
					return make_pair(first, first._base->right);
				}
			}

			ft::pair<const_iterator, const_iterator>	equal_range(const Key& key) const {
				const_iterator first = lower_bound(key);
				if (first == end()) {
					return ft::make_pair(end(), end());
				} else if (_compare(key, first->first)) {
					return ft::make_pair(first, first);
				} else {
					return make_pair(first, first._base->right);
				}
			}


			iterator	lower_bound(const Key& key) {
				Node<value_type>* node = _root;

				while (node) {
					if (_compare(node->data->first, key)) {
						node = node->right;
					} else {
						return iterator(node);
					}
				}

				return end();
			}

			const_iterator	lower_bound(const Key& key) const {
				Node<value_type>* node = _root;

				while (node) {
					if (_compare(node->data->first, key)) {
						node = node->right;
					} else {
						return iterator(node);
					}
				}

				return end();
			}


			iterator	upper_bound(const Key& key) {
				Node<value_type>* node = _root;

				while (node) {
					if (_compare(key, node->data->first)) {
						return iterator(node);
					} else {
						node = node->right;
					}
				}

				return end();
			}

			const_iterator	upper_bound(const Key& key) const {
				Node<value_type>* node = _root;

				while (node) {
					if (_compare(key, node->data->first)) {
						return iterator(node);
					} else {
						node = node->right;
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

				return iterator(nullptr);
			}

			ft::pair<iterator, bool>	insert(const ft::pair<const Key, T>& value) {
				Node<value_type>*	node = _root;
				Node<value_type>**	next;

				if (_root == nullptr) {
					_root = _node_allocator.allocate(1);
					_node_allocator.construct(_root, Node<value_type>(value, _allocator));
					++_size;
					// handle end node
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

						// rebalance
						// while (node) { // change to while (node != &end_node)
						// 	((node->parent->left == node) ? node->parent->left : node->parent->right) = rebalance(node);
						// 	node = node->parent;
						// }

						// handle end node
						return ft::make_pair(*next, true);
					}
				}
			}

			void	erase(iterator pos) {
				Node<value_type>*	node = pos._base;

				if (!node->left && !node->right) {	// no children
					if (node->parent)
						((node->parent->left == node) ? node->parent->left : node->parent->right) = nullptr;
					
					_allocator.destroy(node->data);
					_allocator.deallocate(node->data, 1);
					_node_allocator.destroy(node);
					_node_allocator.deallocate(node, 1);

					--_size;
				} else if (!node->left) {	// only a right child
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

				// still need to rebalance
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
	bool	operator==(const AVLTree<Key, T, Compare, Alloc>& lhs, const AVLTree<Key, T, Compare, Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			typename AVLTree<Key, T, Compare, Alloc>::const_iterator	lhs_current = lhs.begin();
			typename AVLTree<Key, T, Compare, Alloc>::const_iterator	lhs_end = lhs.end();
			typename AVLTree<Key, T, Compare, Alloc>::const_iterator	rhs_current = rhs.begin();

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
	bool	operator!=(const AVLTree<Key, T, Compare, Alloc>& lhs, const AVLTree<Key, T, Compare, Alloc>& rhs) {
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<(const AVLTree<Key, T, Compare, Alloc>& lhs, const AVLTree<Key, T, Compare, Alloc>& rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<=(const AVLTree<Key, T, Compare, Alloc>& lhs, const AVLTree<Key, T, Compare, Alloc>& rhs) {
		return !(lhs > rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>(const AVLTree<Key, T, Compare, Alloc>& lhs, const AVLTree<Key, T, Compare, Alloc>& rhs) {
		return rhs < lhs;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>=(const AVLTree<Key, T, Compare, Alloc>& lhs, const AVLTree<Key, T, Compare, Alloc>& rhs) {
		return !(lhs < rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	void	swap(AVLTree<Key, T, Compare, Alloc>& lhs, AVLTree<Key, T, Compare, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}

#endif
