#include <type_traits>
#include "is_integral.hpp"
#include <iostream>

int	main() {
	ft::is_integral<int>::value_type x;
	std::cout << ft::is_integral<char>::value << std::endl;
	std::cout << ft::is_integral<int>::value << std::endl;

}