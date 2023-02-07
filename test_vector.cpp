#include "vector.hpp"
#include <vector>

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

}

int	main() {
	TIME(test_all());

	return 0;
}
