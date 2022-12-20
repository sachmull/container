#include <iostream>
#include "vector.hpp"
#include <vector>

#define NS ft
#define T int
#define PRINTLN(x) (std::cout << (x) << std::endl)

void	print_vec(NS::vector<T> vec) {
	std::cout << "vec size: " << vec.size() << " vec capacity: " << vec.capacity() << std::endl;
	for (NS::vector<int>::size_type i = 0; i < vec.size(); ++i) {
		PRINTLN(vec[i]);
	}
}

int	main() {
	NS::vector<T>	vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	NS::vector<T>	bec;
	bec.push_back(100);
	bec.push_back(200);
	bec.insert(bec.begin() + 1, vec.begin(), vec.end());
	print_vec(bec);
}
