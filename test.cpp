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

void	print_map(ft::map<int, int>& m) {
	std::cout << "size: " << m.size() << std::endl;
	for (ft::map<int, int>::iterator it = m.begin(); it != m.end(); ++it) {
		std::cout << it->first << "\t\t" << it->second << std::endl;
	}
}


int	main(void) {
	NS::map<int, int>	m;
	m.insert(NS::make_pair(1, 2));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(3, 2));

	for (NS::map<int, int>::iterator it = --m.end(); it != m.begin();  it--) {
		PRINTLN(it->first);
	}


	return 0;
}
