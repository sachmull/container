#include <vector>
#include "vector.hpp"

// Debugging
#define PRINTLN(x) std::cout << (x) << std::endl
#define LINE PRINTLN(__LINE__)
#define POS std::cout << __FILE__ << ": " << __LINE__ << std::endl

#ifndef NS
#define NS ft
#endif

int	main() {
	NS::vector<int>	vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	for (NS::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
		PRINTLN(*it);
	}

	return 0;
}
