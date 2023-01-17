#ifndef MAP_HPP
#define MAP_HPP

#include "AVLTree.hpp"
#include "reverse_iterator.hpp"

namespace ft {
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
			typedef	std::size_t							size_type;
			typedef	std::ptrdiff_t						difference_type;
			typedef	Compare								key_compare;
			typedef	Allocator							allocator_type;
			typedef	value_type&							reference;
			typedef	const value_type&					const_reference;
			typedef	typename Allocator::pointer			pointer;
			typedef	typename Allocator::const_pointer	const_pointer;
			typedef	Iterator<value_type>				iterator;
			typedef	Iterator<const value_type>			const_iterator;
			typedef	reverse_iterator<iterator>			reverse_iterator;
			// typedef	reverse_iterator<const_iterator>	const_reverse_iterator;

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
			AVLTree<Key, T, Compare, Allocator>	_tree;

		public:
			map() : _tree() {}
			explicit map(const Compare& comp, const Allocator& alloc = Allocator()) : _tree(comp, alloc) {}
			template <class InputIt>
			map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) : _tree(comp, alloc) {
				for (; first != last; ++first) {
					_tree.insert(*first);
				}
			}
			map(const map& other) : _tree() { *this = other; }

			map&	operator=(const map& other){
				//
			}
	};
}

#endif
