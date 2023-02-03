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

class test {
	private:
		test() {}

	public:
		test(int) {}
};

int	main(void) {
	NS::map<int, int> m;
	NS::map<int, int>::iterator it;

	// it = m.insert(m.end(), NS::make_pair(64, 64));
	// it = m.insert(m.end(), NS::make_pair(64, 64));
	// it = m.insert(m.end(), NS::make_pair(100, 100));
	// it = m.end();
	// --it;
	// it = m.insert(it, NS::make_pair(100, 100));
	// it = m.insert(it, NS::make_pair(69, 69));
	// std::cout << it._base << std::endl; // ft: it.base() == NULL, std: it->first == 6

	m.insert(NS::make_pair(64, 64));
	// it = m.insert(NS::make_pair(64, 64)).first;
	// std::cout << it->first << std::endl;
	// it = m.insert(NS::make_pair(100, 100)).first;
	// it = m.end();
	// --it;
	m.insert(NS::make_pair(100, 100));
	it = m.insert(NS::make_pair(69, 69)).first;
	std::cout << it._base << std::endl; // ft: it.base() == NULL, std: it->first == 6
	

	return 0;
}
