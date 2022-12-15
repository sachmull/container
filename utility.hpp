#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "pair.hpp"


namespace ft {
	template <class T1, class T2>
	ft::pair<T1, T2>	make_pair(T1 t, T2 u) {
		return ft::pair<T1, T2>(t, u);
	}
}

#endif
