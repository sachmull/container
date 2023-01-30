#include "new_vector.hpp"

#define _vector ft::vector<int>
#define _iterator ft::vector<int>::iterator

int	main() {
	_vector	vec;
	
	vec.insert(vec.end(), 3);
	vec.insert(vec.end(), 4);

	std::cout << "size: " << vec.size() << " capacity: " << vec.capacity() << std::endl;
	for (_iterator it = vec.begin(); it!= vec.end(); ++it) {
		std::cout << *it << std::endl;
	}
}
