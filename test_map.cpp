#include "map.hpp"
#include <map>

#include "test_helpers.hpp"

template <class K, class T>
void	print(NS::map<K, T>& map, std::string file, int line) {
	std::cout << file << " " << line << std::endl;
	std::cout << "size: " << map.size() << std::endl;
	std::cout << "is empty: " << map.empty() << std::endl;

	for (typename NS::map<K, T>::iterator it = map.begin(); it != map.end(); ++it) {
		std::cout << it->first << " " << it->second << std::endl;
	}

	std::cout << std::endl;
}

#define print(m) print(m, __FILE__, __LINE__)

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
	(void)r;
	NS::map<int, int>::const_reference	cr = value;
	(void)cr;
	NS::map<int, int>::pointer ptr;
	(void)ptr;
	NS::map<int, int>::const_pointer cptr;
	(void)cptr;
	NS::map<int, int>::iterator it;
	NS::map<int, int>::const_iterator cit;
	NS::map<int, int>::reverse_iterator rit;
	NS::map<int, int>::const_reverse_iterator crit;
}

void	test_constructors() {
	NS::map<int, int>	m;

	std::greater<int>	greater_comp;
	std::allocator<NS::pair<const int, int> >	allocator;
	NS::map<int, int, std::greater<int> >	m1(greater_comp, allocator);
	NS::map<int, int, std::greater<int> >	m2(m.begin(), m.end(), greater_comp);

	NS::map<int, int, std::greater<int> >	m3(m.begin(), m.end());
	NS::map<int, int>	m4(m);
}

void	test_modifiers() {
	NS::map<int, int>	m;
	m.insert(NS::make_pair(1, 1));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(3, 3));
	print(m);

	m.erase(1);
	print(m);

	m.clear();
	print(m);

	m.insert(NS::make_pair(3, 3));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(1, 1));
	print(m);

	m.erase(2);
	print(m);

	m.insert(NS::make_pair(3, 3));
	m.insert(NS::make_pair(4, 4));
	m.insert(NS::make_pair(9, 9));
	m.insert(NS::make_pair(8, 8));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(1, 1));
	m.insert(NS::make_pair(5, 5));
	print(m);

	m.insert(++m.begin(), m.end());
	print(m);

	m.erase(++m.begin(), --m.end());
	print(m);

	PRINTLN(m[m.begin()->first + 10]);
	print(m);
	m[m.begin()->first + 10] = 100;
	print(m);

	PRINTLN(m.at(m.begin()->first));

	PRINTLN(m.count(m.begin()->first));
	PRINTLN(m.count(0));

	PRINTLN(m.find(((m.begin()->first)))->second);

	m.insert(NS::make_pair(3, 3));
	m.insert(NS::make_pair(4, 4));
	m.insert(NS::make_pair(9, 9));
	m.insert(NS::make_pair(8, 8));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(2, 2));
	m.insert(NS::make_pair(1, 1));
	m.insert(NS::make_pair(5, 5));
	print(m);

	PRINTLN(m.upper_bound(7)->first);
	PRINTLN(m.lower_bound(7)->first);
	PRINTLN(m.equal_range(7).first->first);
	print(m);

	PRINTLN(m.upper_bound(1)->first);
	PRINTLN(m.lower_bound(1)->first);
	PRINTLN(m.equal_range(1).first->first);
	print(m);

	PRINTLN(m.upper_bound(1000) == m.end());
	PRINTLN(m.lower_bound(1000) == m.end());
	PRINTLN(m.equal_range(1000).first == m.end());
	// print(m);
}

void	test_all() {
	TIME(test_typedefs());
	TIME(test_constructors());
	TIME(test_modifiers());
}

int	main() {
	TIME(test_all());
	PRINTLN("FINISHED");

	return 0;
}
