#include <vector>
#include "vector.hpp"
#include <iostream>
#include <limits.h>
#include <string>
#include <algorithm>
// #include "map.hpp"
#include "pair.hpp"
#include "utility.hpp"
#include <map>
#include <functional>
#include "map.hpp"
#include <sstream>

#define NS ft
#define T int
// #define PRINTLN(x) (std::cout << (x) << std::endl)
#define RATIO 10000

#define _vector ft::vector<int>
#define _iterator ft::vector<int>::iterator

void	print_map(ft::map<int, int>& m) {
	std::cout << "size: " << m.size() << std::endl;
	for (ft::map<int, int>::iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << it->first << "\t\t" << it->second << std::endl;
	}
}

void	print_vector(_vector& vec) {
	std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << std::endl;
	for (_iterator it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << std::endl;
	}
}

template <class C>
void	print_container(C& container) {
	for (typename C::iterator it = container.begin(); it != container.end(); ++it) {
		std::cout << *it << std::endl;
	}
}


void	test_func(ft::vector<int>& v) {
	v.push_back(2);
}


int	main(void) {
	ft::vector<char>	vec(64, 'a');

	return 0;
}
