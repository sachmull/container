#ifndef MAP_HPP
#define MAP_HPP

#include <functional> //std::less
#include <queue> // std::queue
#include <iostream>

#include <memory> // std::allocator

#include "pair.hpp"

#define EQ(a, b) (comp((a), (b)) == false && comp((b), (a)) == false)
#define LESS(a, b) (comp((a), (b)) == true)
#define GREATER(a, b) (comp((b), (a)) == true)

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

		private:
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

			Node*	pInsert(const value_type& val) {
				Node*	newNode = new Node(val, allocator);

				if (root == NULL) {
					root = newNode;
					return newNode;
				}

				Node*	node = root;
				while (true) {
					// Traverse the tree to the left or right depending on the key
					if (LESS(val.first, node->data->first)) {
						if (node->left != NULL) {
							// Left subtree exists -> follow
							node = node->left;
						} else {
							// Left subtree does not exist -> insert
							node->left = newNode;
							newNode->parent = node;
							return newNode;
						}
					} else if (GREATER(val.first, node->data->first)) {
						if (node->right != NULL) {
							// Right subtree exists -> follow
							node = node->right;
						} else {
							// Right subtree does not exist -> insert
							node->right = newNode;
							newNode->parent = node;
							return newNode;
						}
					} else {
						// Key already exists
						allocator.deallocate(newNode->data, 1);
						delete newNode;
						return NULL;
					}
				}
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

			size_type	height(Node* node) {
				return node != NULL ? node->height : -1;
			}

			void	updateHeight(Node* node) {
				size_type	left = height(node->left);
				size_type	right = height(node->right);
				node->height = ((left > right) ? left : right) + 1;
			}

			size_type	balanceFactor(Node* node) {
				return height(node->right) - height(node->left);
			}

			Node*	rotateRight(Node *node) {
				// Do I need to update node->parent->left/->right
				Node*	left = node->left;

				left->parent = node->parent;

				node->left = left->right;
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
				node->right->parent = node;

				right->left = node;
				node->parent = right;

				updateHeight(node);
				updateHeight(right);

				return right;
			}

			Node*	rebalance(Node* node) {
				size_type	bf = balanceFactor(node);

				// Left-heavy
				if (bf < -1) {
					if (balanceFactor(node->left) <= 0) {
						// Rotate right
						node = rotateRight(node);
					} else {
						// Rotate left-right
						node->left = rotateLeft(node->left);
						node = rotateRight(node);
					}
				}

				// Right-heavy
				if (bf > 1) {
					if (balanceFactor(node->right) >= 0) {
						// Rotate left
						node = rotateLeft(node);
					} else {
						// Rotate right-left
						node->right = rotateRight(node->right);
						node = rotateLeft(node);
					}
				}

				return node;
			}

		public:
			explicit map() : comp(), root(NULL), allocator() {}
			explicit map(const Compare& comp, const Allocator& alloc = Allocator()) : comp(comp), allocator(alloc), root(NULL) {}
			template <class InputIt>
			map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : comp(comp), allocator(alloc), root(NULL) {
				while (first != last) {
					insert(*first);
					**first;
				}
			}
			map(const map& other) { *this = other; }

			~map() {
				//
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
	};
}

#endif
