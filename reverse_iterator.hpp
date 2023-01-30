#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP

#include "iterator_traits.hpp"

namespace ft {
	template <class Iter>
	class reverse_iterator {
		protected:
			Iter	current;

		public:
			typedef	Iter													iterator_type;
			typedef	typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
			typedef	typename ft::iterator_traits<Iter>::value_type			value_type;
			typedef	typename ft::iterator_traits<Iter>::difference_type		difference_type;
			typedef	typename ft::iterator_traits<Iter>::pointer				pointer;
			typedef	typename ft::iterator_traits<Iter>::reference			reference;

		public:
			reverse_iterator() : current(Iter()) {}
			explicit	reverse_iterator(iterator_type x) : current(x) {}
			// template <class U>
			reverse_iterator(const reverse_iterator<Iter>& other)  {*this = other; }

			// rit<it<pair> > => rit<it<const pair> > >
			template <class iter>
			reverse_iterator(const reverse_iterator<iter>& rit) {
				current = rit.base();
			}


			// template <class U>
			reverse_iterator&	operator=(const reverse_iterator<Iter>& other) {
				if (this != &other) {
					current = other.current;
				}

				return *this;
			}

			iterator_type	base() const { return current; }

			reference	operator*() const { iterator_type tmp(current); --tmp; return *tmp; /*return *(current - 1);*/ }
			pointer		operator->() const { iterator_type tmp(current); --tmp; return tmp.operator->(); /*return (current - 1).operator->();*/ }

			reference	operator[](difference_type n) const { return base()[- n - 1]; }

			reverse_iterator&	operator++() { --current; return *this; }
			reverse_iterator&	operator--() { ++current; return *this; }

			reverse_iterator	operator++(int) { reverse_iterator rit = *this; --current; return rit; }
			reverse_iterator	operator--(int) { reverse_iterator rit = *this; ++current; return rit; }

			reverse_iterator	operator+(difference_type n) const { return reverse_iterator(current - n); }
			reverse_iterator	operator-(difference_type n) const { return reverse_iterator(current + n); }

			template <class IterR>
			difference_type	operator-(reverse_iterator<IterR> n) const { return current - n.current; }

			reverse_iterator&	operator+=(difference_type n) { current - n; return *this; }
			reverse_iterator&	operator-=(difference_type n) { current + n; return *this; }
	};

	template <class Iterator1, class Iterator2>
	bool	operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() == rhs.base(); }
	template <class Iterator1, class Iterator2>
	bool	operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() != rhs.base(); }
	template <class Iterator1, class Iterator2>
	bool	operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() > rhs.base(); }
	template <class Iterator1, class Iterator2>
	bool	operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() >= rhs.base(); }
	template <class Iterator1, class Iterator2>
	bool	operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() < rhs.base(); }
	template <class Iterator1, class Iterator2>
	bool	operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() <= rhs.base(); }

	template <class Iter>
	reverse_iterator<Iter>	operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it) { return reverse_iterator<Iter>(it.base() - n); }
}

#endif
