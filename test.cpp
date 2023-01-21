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
#include "AVLTree.hpp"

#define NS ft
#define T int
// #define PRINTLN(x) (std::cout << (x) << std::endl)
#define RATIO 10000

void	print_vec(NS::vector<T> vec) {
	std::cout << "vec size: " << vec.size() << " vec capacity: " << vec.capacity() << std::endl;
	for (NS::vector<int>::size_type i = 0; i < vec.size(); ++i) {
		PRINTLN(vec[i]);
	}
}

int	main(void) {
	ft::AVLTree<int, int> m1;
	m1.insert(ft::make_pair(2, 2));

	ft::AVLTree<int, int> m2;
	m2.insert(ft::make_pair(2, 1));

	const ft::AVLTree<int, int>::const_iterator it = m2.begin();

	PRINTLN(m1 == m2);
	PRINTLN(m1 != m2);
	PRINTLN(m1 < m2);
	PRINTLN(m1 <= m2);
	PRINTLN(m1 > m2);
	PRINTLN(m1 >= m2);

	PRINTLN("\n---------");

	ft::AVLTree<int, int> a;
	a.insert(ft::make_pair(2, 2));

	ft::AVLTree<int, int> b;
	b.insert(ft::make_pair(2, 1));

	PRINTLN(a == b);
	PRINTLN(a != b);
	PRINTLN(a <  b);
	PRINTLN(a <= b);
	PRINTLN(a >  b);
	PRINTLN(a >= b);

	swap(a, b);

	return 0;
}
