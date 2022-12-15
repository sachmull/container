#ifndef MAP_HPP
#define MAP_HPP

#include <functional> //std::less
#include <queue> // std::queue
#include <iostream>

#include <memory> // std::allocator

#include "pair.hpp"
#include "utility.hpp"

#define EQ(a, b) (comp((a), (b)) == false && comp((b), (a)) == false)
#define LESS(a, b) (comp((a), (b)) == true)
#define GREATER(a, b) (comp((b), (a)) == true)
#define PARENT_POINTER(child) (child->parent->left == child ? child->parent->left : child->parent->right)

namespace ft {
	template <
		class Key,
		class T,
		class Compare = std::less<Key>,
		class Allocator = std::allocator<ft::pair<const Key, T> >
	>
	class map {
		public:
			typedef	Key	key_type;
			typedef	T	mapped_type;
			typedef	ft::pair<const Key, T>	value_type;
			typedef	size_t	size_type;
			typedef	ptrdiff_t	difference_type;
			typedef	Compare	key_compare;
			typedef	Allocator	allocator_type;
			typedef	value_type&	reference;
			typedef	const value_type&	const_reference;
			typedef	typename Allocator::pointer	pointer;
			typedef	typename Allocator::const_pointer	const_pointer;

		public:
			struct Node;

		public:
			Allocator	allocator;
			Node*		root;
			Compare		comp;
			Node		endNode;
			Node*		lastInsert;
			size_type	treeSize;

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
			};

		public:
			struct Node {
				value_type*	data;
				Node*		left;
				Node*		right;
				Node*		parent;
				size_type	height;

				Node(const value_type& value, Allocator& allocator) {
					data = allocator.allocate(1);
					allocator.construct(data, value);

					left = NULL;
					right = NULL;
					parent = NULL;

					height = 0;
				}

				Node() : data(NULL), left(NULL), right(NULL), parent(NULL), height(0) {}
			};

			Node*	pSearch(const key_type& key) {
				return pSearch(key, root);
			}

			Node*	pSearch(const key_type& key, Node* node) {
				if (node == NULL) {
					return NULL;
				}

				if (EQ(key, node->data->first)) {
					return node;
				} else if (LESS(key, node->data->first)) {
					return pSearch(key, node->left);
				} else {
					return pSearch(key, node->right);
				}
			}

			Node*	pInsert(const value_type& val, Node* node) {
				if (node == NULL) {
					lastInsert = new Node(val, allocator);
					++treeSize;
					return lastInsert;
				} else if (LESS(val.first, node->data->first)) {
					node->left = pInsert(val, node->left);
					updateHeight(node);
					node->left = rebalance(node->left);
					node->left->parent = node;
				} else if (GREATER(val.first, node->data->first)) {
					node->right = pInsert(val, node->right);
					updateHeight(node);
					node->right = rebalance(node->right);
					node->right->parent = node;
				} else if (EQ(val.first, node->data->first)) {
					lastInsert = node;
				}

				return node;
			}

			bool	removeRecursive(const key_type& key) {
				size_type	tmp = treeSize;
				root = removeRecursive(key, root);
				if (root)
					root = rebalance(root);
				endNode.left = root;
				if (root)
					root->parent = &endNode;

				// Return true if a element was deleted -> treeSize shrinked
				if (tmp != treeSize) {
					return true;
				}
				return false;
			}

			Node*	removeRecursive(const key_type& key, Node* node) {
				// No node at current position -> go up recursion
				if (node == NULL) {
					return NULL;
				}

				// Traverse the tree to the left or right depending on the key
				if (LESS(key, node->data->first)) {
					node->left = removeRecursive(key, node->left);
					if (node->left) {
						node->left = rebalance(node->left);
					}
				} else if (GREATER(key, node->data->first)) {
					node->right = removeRecursive(key, node->right);
					if (node->right) {
						node->right = rebalance(node->right);
					}
				} else if (node->left == NULL && node->right == NULL) {	// At this point node is the node to be deleted
					std::cout << "Node " << node->data->first << " has no children" << std::endl;
					// Node has no child
					allocator.destroy(node->data);
					allocator.deallocate(node->data, 1);
					delete node;
					node = NULL;
					--treeSize;
				} else if (node->left == NULL) {
					std::cout << "Node " << node->data->first << " has right child" << std::endl;
					// Replace node with node right
					PARENT_POINTER(node) = node->right;
					node->right->parent = node->parent;	// This line should be removeable, since parent's pointers are reset one recursion up
					Node*	tmp = node->right;

					allocator.destroy(node->data);
					allocator.deallocate(node->data, 1);
					delete node;
					node = NULL;
					--treeSize;

					return tmp;
				} else if (node->right == NULL) {
					std::cout << "Node " << node->data->first << " has left child" << std::endl;
					// Replace node with node left
					PARENT_POINTER(node) = node->left;
					node->left->parent = node->parent;	// This line should be removeable, since parent's pointers are reset one recursion up
					Node*	tmp = node->left;

					allocator.destroy(node->data);
					allocator.deallocate(node->data, 1);
					delete node;
					--treeSize;

					return tmp;
				} else { // Node has two children
					std::cout << "Node " << node->data->first << " has two children" << std::endl;
					Node*	inOrderSuccessor = node->right;
					while (inOrderSuccessor->left != NULL) {
						inOrderSuccessor = inOrderSuccessor->left;
					}

					ft::swap(node->data, inOrderSuccessor->data);
					node->right = removeRecursive(inOrderSuccessor->data->first, node->right);
				}

				return node;
			}

			bool	pRemove(const key_type& key) {
				Node*	node = root;
				Node*	parent = NULL;

				// Find the node to be deleted
				while (node != NULL && !EQ(node->data->first, key)) {
					// Traverse the tree to the left or right depending on key
					parent = node;
					if (LESS(key, node->data->first)) {
						node = node->left;
					} else {
						node = node->right;
					}
				}

				// Node not found
				if (node == NULL) {
					return false;
				}

				// At this point, node is the node to be deleted
				--treeSize;

				// Node has at most one child -> replace node by its only child
				if (node->left == NULL || node->right == NULL) {
					Node*	child = (node->left != NULL) ? node->left : node->right;

					if (node == root) {
						root = child;
					} else if (LESS(key, parent->data->first)) {
						parent->left = child;
					} else {
						parent->right = child;
					}

					allocator.deallocate(node->data, 1);
					delete node;
					return true;
				} else {
					Node*	inOrderSuccessor = node->right;
					Node*	inOrderSuccessorParent = node;

					while (inOrderSuccessor->left != NULL) {
						inOrderSuccessorParent = inOrderSuccessor;
						inOrderSuccessor = inOrderSuccessor->left;
					}

					// Copy inorder successors data to current node
					allocator.destroy(node->data);
					node->data = inOrderSuccessor->data;

					// Delete inorder successor

					// Inorder successor is the deleted nodes right child
					if (inOrderSuccessor == node->right) {
						node->right = inOrderSuccessor->right;
						inOrderSuccessor->right->parent = node;
						delete inOrderSuccessor;
					} else {
						inOrderSuccessorParent->left = inOrderSuccessor->right;
						inOrderSuccessorParent->left->parent = inOrderSuccessorParent;
						delete inOrderSuccessor;
					}
					return true;
				}
			}

			int	height(Node* node) {
				return (node != NULL) ? node->height : -1;
			}

			void	updateHeight(Node* node) {
				int	left = height(node->left);
				int	right = height(node->right);
				node->height = ((left > right) ? left : right) + 1;
			}

			int	balanceFactor(Node* node) {
				return height(node->right) - height(node->left);
			}

			Node*	rotateRight(Node *node) {
				// Do I need to update node->parent->left/->right
				Node*	left = node->left;

				left->parent = node->parent;

				node->left = left->right;
				if (left->right->parent)
					left->right->parent = node;

				left->right = node;
				node->parent = left;

				updateHeight(node);
				updateHeight(left);

				return left;
			}

			Node*	rotateLeft(Node* node) {
				Node*	right = node->right;

				right->parent = node->parent;

				node->right = right->left;
				if (node->right)
					node->right->parent = node;

				right->left = node;
				node->parent = right;

				updateHeight(node);
				updateHeight(right);

				return right;
			}

			Node*	rebalance(Node* node) {
				updateHeight(node);
				// std::cout << "rebalance\n";
				int	bf = balanceFactor(node);
				// std::cout << "balance factor " << bf << std::endl;

				// Left-heavy
				if (bf < -1) {
					// std::cout << "-1\n";
					if (balanceFactor(node->left) <= 0) {
						// std::cout << "r\n";
						// Rotate right
						node = rotateRight(node);
					} else {
						// std::cout << "lr\n";
						// Rotate left-right
						node->left = rotateLeft(node->left);
						node = rotateRight(node);
					}
				}

				// Right-heavy
				if (bf > 1) {
					// std::cout << "1\n";
					if (balanceFactor(node->right) >= 0) {
						// std::cout << "l\n";
						// Rotate left
						node = rotateLeft(node);
					} else {
						// std::cout << "rl\n";
						// Rotate right-left
						node->right = rotateRight(node->right);
						node = rotateLeft(node);
					}
				}

				return node;
			}

			Node*	leftMost(Node* node) {
				if (node) {
					while (node->left) {
						node = node->left;
					}
				}
				return node;
			}

			Node*	rightMost(Node* node) {
				if (node) {
					while (node->right) {
						node = node->right;
					}
				}
				return node;	
			}


		public:
			template <class U>
			class Iterator {
				public:
					typedef	U								value_type;
					typedef	U*								pointer;
					typedef	U&								reference;
					typedef	ptrdiff_t						difference_type;
					typedef	std::bidirectional_iterator_tag	iterator_category;

				private:
					Node*	base;

				public:
					Iterator(Node* base) : base(base) {}

					Iterator&	operator++() {
						if (!base) {
							return *this;
						}

						if (base && base->right) {
							base = leftMost(base->right);
							return *this;
						}

						Node*	child = base;
						Node*	parent = base->parent;
						while (parent && child == parent->right) {
							child = parent;
							parent = parent->parent;
						}
						base = parent;

						return *this;
					}

					Iterator&	operator--() {
						if (!base) {
							return *this;
						}

						if (base && base->left) {
							base = rightMost(base->left);
							return *this;
						}

						Node*	child = base;
						Node*	parent = base->parent;
						while (parent && child == parent->left) {
							child = parent;
							parent = parent->parent;
						}
						base = parent;

						return *this;
					}

					value_type&	operator*() {
						return *(base->data);
					}

					value_type*	operator->() {
						return (base->data);
					}

					friend bool	operator==(const Iterator& lhs, const Iterator& rhs) {
						return lhs.base == rhs.base;
					}

					friend bool	operator!=(const Iterator& lhs, const Iterator& rhs) {
						return !(lhs == rhs);
					}

				private:
					Node*	leftMost(Node* node) {
						if (node) {
						while (node->left) {
							node = node->left;
						}
						}
						return node;
					}

					Node*	rightMost(Node* node) {
						if (node) {
							while (node->right) {
								node = node->right;
							}
						}
						return node;	
					}
			};

		public:
			explicit map() : comp(), root(NULL), allocator(), endNode(), treeSize(0) {}
			explicit map(const Compare& comp, const Allocator& alloc = Allocator()) : comp(comp), allocator(alloc), root(NULL) {}
			template <class InputIt>
			map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : comp(comp), allocator(alloc), root(NULL), treeSize(0) {
				while (first != last) {
					insert(*first);
					**first;
				}
			}
			map(const map& other) { *this = other; }

			~map() {
				while (treeSize) {
					erase(root);
				}
			}

			map&	operator=(const map& other) {
				if (this != &other) {
					// Erase old content, insert new, reset size, copy comp and allocator
				}

				return *this;
			}

			allocator_type	get_allocator() const {
				return allocator;
			}


			// Iterators
			public:
				typedef	Iterator<value_type>	iterator;

			public:
				iterator	begin() {
					if (root) {
						return iterator(leftMost(root));
					}

					return iterator(NULL);
				}

				iterator	end() {
					if (root) {
						return iterator(&endNode);
					}

					return iterator(NULL);
				}

			// Capacity
			bool	empty() const {
				return treeSize == 0;
			}

			size_type	size() const {
				return treeSize;
			}

			size_type	max_size() const {
				return allocator.max_size();
			}

			// Modifiers
			ft::pair<iterator, bool>	insert(const value_type& value) {
				size_type	tmp = treeSize;

				root = pInsert(value, root);

				updateHeight(root);
				root = rebalance(root);

				root->parent = &endNode;
				endNode.left = root;

				if (tmp == treeSize) {
					return (ft::make_pair(iterator(lastInsert), false));
				}
				return (ft::make_pair(iterator(lastInsert), true));
			}

			void	erase(iterator pos) {
				// pRemove(pos->first);
				removeRecursive(pos->first);
			}


			// Lookup
			size_type	count(const Key& key) const {
				if (pSearch(key)) {
					return 1;
				}
				return 0;
			}

			iterator	find(const Key& key) {
				Node*	node = pSearch(key);

				if (node) {
					return iterator(node);
				}

				return end();
			}

	};

}

#undef EQ
#undef LESS
#undef GREATER
#undef PARENT_POINTER

#endif
