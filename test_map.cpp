#include "map.hpp"
#include <map>

#include "test_helpers.hpp"

template <class K, class T>
void	print(NS::map<K, T>& map, std::string file, int line) {
	std::cout << file << " " << line << std::endl;
	std::cout << "size: " << map.size() << std::endl;

	for (typename NS::map<K, T>::iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}

	std::cout << std::endl;
}

void	test_typedefs() {
	bool b = std::is_same<std::string, NS::map<std::string, int>::key_type >::value;
	PRINTLN(b);

	b = std::is_same<int, NS::map<std::string, int>::mapped_type >::value;
	PRINTLN(b);

	b = std::is_same<NS::pair<const std::string, int>, NS::map<std::string, int>::value_type >::value;
	PRINTLN(b);

	NS::map<int, int>::size_type st = 0;
	--st;
	if (st < 0) { PRINTLN("size_type should be unsigned"); }

	NS::map<int, int>::difference_type	dt = 0;
	--dt;
	if (dt >= 0) { PRINTLN("difference_type should be signed"); }

	NS::map<int, int>::key_compare	comp;
	PRINTLN(comp(1, 1));
	PRINTLN(comp(1, 2));
	PRINTLN(comp(2, 1));

	NS::map<int, int>::allocator_type at;

	NS::map<int, int>::value_type	value = NS::make_pair(1, 2);
	NS::map<int, int>::reference	r = value;
	NS::map<int, int>::const_reference	cr = value;
	NS::map<int, int>::pointer ptr;
	NS::map<int, int>::const_pointer cptr;
	NS::map<int, int>::iterator it;
	NS::map<int, int>::const_iterator cit;
	NS::map<int, int>::reverse_iterator rit;
	NS::map<int, int>::const_reverse_iterator crit;
}

void	test_constructors() {
	NS::map<int, int>	m;
	NS::map<int, int, std::greater<int> >	m1(std::greater<int>(), std::allocator<NS::pair<const int, int> >);
	NS::map<int, int, std::greater<int> >	m2(NULL, NULL, std::greater<int>());
	NS::map<int, int>	m3(m);
}

void	test_all() {
	TIME(test_typedefs());
}

int	main() {
	TIME(test_all());
	TIME(test_constructors());
	// system("leaks a.out");
	// PRINTLN("FINISHED");

	return 0;
}
