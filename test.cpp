#include <vector>
#include "vector.hpp"
#include <iostream>
#include <limits.h>
#include <string>
#include <algorithm>
#include "map.hpp"
#include "pair.hpp"
#include "utility.hpp"

#define NS ft
#define T std::string
#define PRINTLN(x) (std::cout << (x) << std::endl)
#define RATIO 10000

void	print_vec(NS::vector<T> vec) {
	std::cout << "vec size: " << vec.size() << " vec capacity: " << vec.capacity() << std::endl;
	for (NS::vector<int>::size_type i = 0; i < vec.size(); ++i) {
		PRINTLN(vec[i]);
	}
}

int	main(void) {
	NS::map<int, int> m;
	m.insert(ft::make_pair(1, 2));
	m.insert(ft::make_pair(2, 3));
	m.insert(ft::make_pair(3, 4));
	PRINTLN(m.root->data->first);
	PRINTLN(m.root->data->second);

	NS::map<int, int>::Node*	n = m.search(1);
	PRINTLN(n->data->first);
	PRINTLN(n->data->second);

	m.remove(1);
	PRINTLN(m.root);

	return 0;
}
