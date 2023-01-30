#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include "iterator_traits.hpp"

namespace ft {
	template <class InputIt>
	typename ft::iterator_traits<InputIt>::difference_type	distance(InputIt first, InputIt last) {
		typename ft::iterator_traits<InputIt>::difference_type	_distance = 0;

		while (first != last) {
			++first;
			++_distance;
		}

		return _distance;
	}
}

#endif
