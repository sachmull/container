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
	// std::map<int, int> x;
	// PRINTLN(sizeof(x));
	// x.insert(std::make_pair(4, 4));
	// std::map<int, int>::iterator	i = x.begin();
	// PRINTLN((*i).first);

	NS::map<int, int> m;
	m.insert(ft::make_pair(1, 2));
	m.insert(ft::make_pair(2, 3));
	m.insert(ft::make_pair(3, 4));
	m.insert(ft::make_pair(4, 5));
	m.insert(ft::make_pair(5, 6));
	m.insert(ft::make_pair(6, 7));

	// // root is set wrong
	// auto node = m.root;
	// PRINTLN(node->left);
	// PRINTLN(node->right->parent->data->first);
	// PRINTLN(node->right->right->parent->data->first);
	// PRINTLN(node->data->first);
	// PRINTLN(node->left);
	// PRINTLN(node->right);
	// PRINTLN(node->right->data->first);
	// PRINTLN(node->left->data->first);
	// PRINTLN(m.root->data->first);
	// PRINTLN(m.root->data->second);

	// NS::map<int, int>::iterator it = m.begin();
	// PRINTLN("it");
	// PRINTLN((*it).first);

	PRINTLN("ROOT");
	PRINTLN(m.root->data->first);
	m.erase(m.find(m.root->data->first));
	PRINTLN("");

	for (NS::map<int, int>::iterator it = m.begin(); it != m.end(); it = m.begin()) {
		PRINTLN(it->first);
		m.erase(it);
		// PRINTLN("a");
	}

	PRINTLN("still still fine");
	m.find(4);

	// NS::map<int, int>::Node*	n = m.pSearch(3);
	// PRINTLN(n->data->first);
	// PRINTLN(n->data->second);

	// m.pRemove(1);
	// PRINTLN(m.root);

	return 0;
}
