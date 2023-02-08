#include "vector.hpp"
#include <vector>

#include "swap.hpp"

#include "test_helpers.hpp"

template <class T>
void	print_vector(NS::vector<T>&	vec, std::string file, int line) {
	std::cout << file << " " << line << std::endl;
	std::cout << "size: " << vec.size() << std::endl;
	std::cout << "capacity: " << vec.capacity() << std::endl;

	for (typename NS::vector<T>::iterator it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << std::endl;
	}

	std::cout << std::endl;
}

#define print_vector(x) print_vector(x, __FILE__, __LINE__)


void	test_typedefs() {
	NS::vector<std::string>::value_type	val	= "this is a string";
	std::allocator<std::string>	alloc = NS::vector<std::string>::allocator_type();
	NS::vector<std::string>::size_type	st = 0; // --st should still be > 0
	--st;
	if (st < 0) { PRINTLN("size_type should be unsigned"); }
	NS::vector<std::string>::difference_type	dt = 0; // --dt should be < 0
	--dt;
	if (dt > 0) { PRINTLN("difference_type should be signed"); }
	NS::vector<std::string>::reference r = val;
	NS::vector<std::string>::const_reference cr = val;
	NS::vector<std::string>::pointer p = &val;
	NS::vector<std::string>::const_pointer cp = &val;
	NS::vector<std::string>::iterator	it;
	NS::vector<std::string>::const_iterator	cit;
	NS::vector<std::string>::reverse_iterator	rit;
	NS::vector<std::string>::const_reverse_iterator	crit;
}


void	test_default_constructor() {
	NS::vector<int>	vec;
	NS::vector<std::string>	vec2;

	print_vector(vec);
	print_vector(vec2);
}

void	test_fill_constructor() {
	NS::vector<int>	vec(0, 10);
	NS::vector<std::string>	vec2(10, "Hello World!");

	print_vector(vec);
	print_vector(vec2);
}

void	test_range_constructor() {
	std::istringstream	str("1234567890");
	std::istreambuf_iterator<char>	it(str), end;
	NS::vector<char>	vec(it, end);
	print_vector(vec);

	NS::vector<char>	vec2(vec.begin(), vec.end());
	print_vector(vec2);
}

void	test_copy_constructor() {
	NS::vector<std::string>	vec(3, "42");
	NS::vector<std::string>	vec2(vec);

	print_vector(vec);
	print_vector(vec2);

	vec.clear();

	print_vector(vec);
	print_vector(vec2);
}


void	test_assignment_op() {
	NS::vector<std::string>	vec(3, "42");
	NS::vector<std::string>	vec2(10, "24");

	vec2 = vec;

	print_vector(vec);
	print_vector(vec2);

	vec.clear();

	print_vector(vec);
	print_vector(vec2);
}

void	test_assign() {
	NS::vector<std::string>	vec;
	vec.assign(10, "42");

	vec.assign(++vec.begin(), --vec.end());
	print_vector(vec);

	NS::vector<char>	vec2;
	std::istringstream	str("1234567890");
	std::istreambuf_iterator<char>	it(str), end;
	vec2.assign(it, end);
	print_vector(vec2);
}

void	test_get_allocator() {
	NS::vector<int>::allocator_type	at = NS::vector<int>().get_allocator();
}

void	test_at() {
	NS::vector<int>	vec(3, 10);

	int i = vec.at(0);
	try {
		i = vec.at(vec.size());
	} catch (std::out_of_range e) {
		PRINTLN(e.what());
	}

	// this might look confusing but its just a workaround to allow inner functions in C++ 98
	// PS: instead of this construct one could also just cast or copy construct into a const vector
	struct need_const {
		static void	give_me_const(const NS::vector<int> vec) {
			for (int i = 0; i < vec.size(); ++i) {
				PRINTLN(vec.at(i));
			}

			try {
				int i = vec.at(vec.size());
			} catch (std::out_of_range e) {
				PRINTLN(e.what());
			}
		}
	};

	need_const::give_me_const(vec);
}

void	test_array_like_access() {
	NS::vector<int>	vec(3, 42);
	vec.resize(2);

	int i = vec[2];
	vec[2] = 20;
	if (vec[2] != 20) { PRINTLN("vector's content should be modifiable through the []-operator"); }

	const NS::vector<int>	vec2(vec);
	i = vec[2];
}

void	test_front() {
	NS::vector<int>	vec;

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	if (vec.front() != 1) { PRINTLN("front should be equal to 1"); }

	const NS::vector<int>	vec2(vec);
	if (vec2.front() != 1) { PRINTLN("front should be equal to 1"); }
	const int& i = vec2.front();
}

void	test_back() {
	NS::vector<int>	vec;

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	if (vec.back() != 3) { PRINTLN("back should be equal to 3"); }

	const NS::vector<int>	vec2(vec);
	if (vec2.back() != 3) { PRINTLN("back should be equal to 3"); }
	const int& i = vec2.back();
}

void	test_data() {
	NS::vector<int>	vec;

	if (vec.data() != vec.data() + vec.size()) { PRINTLN("data, data + size must always be a valid range"); }

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	if (vec.data() != &(vec[0])) { PRINTLN("data should return a pointer to the underlying array"); }

	const NS::vector<int>	vec2(vec);
	if (vec2.data() != &(vec2[0])) { PRINTLN("data should return a pointer to the underlying array"); }
	const int* ptr = vec2.data();
}

void	test_begin_end() {
	NS::vector<int>	vec;

	if (vec.begin() != vec.end()) { PRINTLN("begin should be equal to end if the container is empty"); }

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	if (vec.begin().base() != vec.data()) { PRINTLN("begin should point to the begin of the array"); }
	if (vec.end() - vec.begin() != vec.size()) { PRINTLN("difference between begin and end should be equal to size"); }

	const NS::vector<int>	vec2(vec);
	NS::vector<int>::const_iterator	cit;
	cit = vec2.begin();
	cit = vec2.end();
}

void	test_empty() {
	NS::vector<int>	vec;

	if (!vec.empty()) { PRINTLN("default constructed vector should be empty"); };

	vec.push_back(1);
	if (vec.empty()) { PRINTLN("vector should no longer be empty"); }

	vec.clear();
	if (vec.size() != 0) { PRINTLN("vector should be empty after clear"); }

	const NS::vector<int>	vec2(vec);
	vec2.empty();
}

void	test_size_and_capacity() {
	NS::vector<int>	vec;

	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.reserve(100);
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.resize(123, 42);
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.clear();
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	NS::vector<int>	vec2(3, 42);
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	vec.push_back(6);
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.insert(vec.end(), vec.begin(), vec.end());
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.clear();
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.insert(vec.end(), vec.begin(), vec.end());
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());
}

void	test_reserve() {
	NS::vector<int>	vec;

	vec.reserve(0);
	if (vec.capacity() != 0) { PRINTLN("capacity should be 0"); }

	vec.reserve(10);
	if (vec.capacity() != 10) { PRINTLN("capacity should be 10"); }

	vec.reserve(9);
	if (vec.capacity() != 10) { PRINTLN("capacity should still be 10"); }

	if (vec.begin() != vec.end()) { PRINTLN("begin should be equal to end"); }
}

void	test_clear() {
	NS::vector<int>	vec;

	vec.reserve(10);
	vec.clear();
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	vec.clear();
	PRINTLN(vec.size());
	PRINTLN(vec.capacity());

	if (vec.begin() != vec.end()) { PRINTLN("after clear begin should equal end"); }
}

void	test_insert() {
	NS::vector<int>	vec;

	NS::vector<int>::const_iterator b, e; b = vec.begin(); e = vec.end();
	vec.insert(e, 1);
	b = vec.begin(); e = vec.end();
	vec.insert(e, *b);
	b = vec.begin(); e = vec.end();
	vec.insert(e, *b);
	b = vec.begin(); e = vec.end();
	print_vector(vec);

	vec.insert(b, b, e);
	b = vec.begin(); e = vec.end();
	print_vector(vec);

	vec.insert(b, 3, 42);
	b = vec.begin(); e = vec.end();
	vec.insert(e, 3, 42);
	print_vector(vec);

	NS::vector<char>	vec2;
	std::istringstream	str("1234567890");
	std::istreambuf_iterator<char>	it(str), end;
	vec2.insert(vec2.begin(), it, end);
	print_vector(vec2);
}

void	test_erase() {
	NS::vector<int>	vec;

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	vec.erase(vec.begin());
	print_vector(vec);
	vec.erase(++vec.begin(), vec.end());
	print_vector(vec);
}

void	test_push_back() {
	NS::vector<int>	vec;

	vec.push_back(1);
	vec.push_back(vec[0]);
	vec.push_back(vec.at(0));
	vec.push_back(vec[0]);
	vec.push_back(*(--vec.end()));
	print_vector(vec);
}

void	test_pop_back() {
	NS::vector<int>	vec;

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	vec.pop_back();
	vec.pop_back();
	vec.pop_back();

	print_vector(vec);
}

void	test_resize() {
	NS::vector<int>	vec;

	vec.resize(0, 0);
	print_vector(vec);

	vec.assign(3, 42);
	vec.resize(1, 42);
	print_vector(vec);

	vec.resize(5, 42);
	print_vector(vec);

	vec.resize(10);
	print_vector(vec);
}

void	test_swap() {
	NS::vector<int>	vec(3, 42);
	NS::vector<int>	vec2(5, 11);
	NS::vector<int>	vec3;

	vec.swap(vec2);
	ft::swap(vec2, vec3);

	print_vector(vec);
	print_vector(vec2);
	print_vector(vec3);
}

void	test_compare_ops() {
	NS::vector<int>	lhs;
	NS::vector<int>	rhs;

	lhs.push_back(1);
	lhs.push_back(2);
	lhs.push_back(3);
	lhs.push_back(4);

	rhs.push_back(1);
	rhs.push_back(2);
	rhs.push_back(3);
	rhs.push_back(4);

	PRINTLN(lhs == rhs);
	PRINTLN(lhs != rhs);
	PRINTLN(lhs < rhs);
	PRINTLN(lhs <= rhs);
	PRINTLN(lhs > rhs);
	PRINTLN(lhs >= rhs);

	NS::vector<int>	eq(lhs);
	PRINTLN(lhs == eq);
	PRINTLN(lhs != eq);
	PRINTLN(lhs < eq);
	PRINTLN(lhs <= eq);
	PRINTLN(lhs > eq);
	PRINTLN(lhs >= eq);
}

void	test_all() {
	TIME(test_typedefs());

	// test constructors
	TIME(test_default_constructor());
	// no testing of allocator_constructor
	TIME(test_fill_constructor());
	TIME(test_range_constructor());
	TIME(test_copy_constructor());

	TIME(test_assignment_op());

	TIME(test_assign());

	TIME(test_get_allocator());

	TIME(test_at());
	
	TIME(test_array_like_access());

	TIME(test_front());

	TIME(test_back());

	TIME(test_data());

	TIME(test_begin_end());

	TIME(test_empty());

	TIME(test_size_and_capacity());

	TIME(test_reserve());

	TIME(test_clear());

	TIME(test_insert());

	TIME(test_erase());

	TIME(test_push_back());

	TIME(test_pop_back());

	TIME(test_resize());

	TIME(test_swap());

	TIME(test_compare_ops());
}

int	main() {
	TIME(test_all());
	system("leaks a.out");
	PRINTLN("FINISHED");

	return 0;
}
