#include <vector>
#include "vector.hpp"
#include <iostream>
#include <limits.h>
#include <string>
#include <algorithm>
#include "map.hpp"
#include "pair.hpp"
#include "utility.hpp"
#include <map>
#include <functional>

#define NS ft
#define T int
#define PRINTLN(x) (std::cout << (x) << std::endl)
#define RATIO 10000

void	print_vec(NS::vector<T> vec) {
	std::cout << "vec size: " << vec.size() << " vec capacity: " << vec.capacity() << std::endl;
	for (NS::vector<int>::size_type i = 0; i < vec.size(); ++i) {
		PRINTLN(vec[i]);
	}
}

int	main(void) {

	std::map<int, int, std::less<int> >::iterator it;
	std::map<int, int, std::greater<int> > m;
	it = m.begin();

	// NS::map<int, int>	m;

	// m.insert(ft::make_pair(0, 10));
	// m.insert(ft::make_pair(1, 10));
	// m.insert(ft::make_pair(2, 10));
	// PRINTLN(m.lower_bound(1).operator*().first);

	// for (int i = 0, j = 10; i < 30 * RATIO; ++i, ++j) {
	// 	m.insert(ft::make_pair(i, j));
	// }

	// NS::map<int, int>	m2(m.begin(), m.end());
	// NS::map<int, int>::iterator it = m2.begin();
	// for (int i = 0; i < 30 * RATIO;++i, it++) {
	// 	// std::cout << it->first << "	" << it->second << std::endl;
	// 	v.push_back(it->first);
	// 	v.push_back(it->second);
	// }

	return 0;
}
