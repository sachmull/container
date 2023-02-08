#include "stack.hpp"
#include <stack>

#include <deque>

#include "test_helpers.hpp"

template <class T>
void	print(NS::stack<T> stack, std::string file, int line) {
	std::cout << file << " " << line << std::endl;
	std::cout << "size: " << stack.size() << std::endl;
	while (!stack.empty()) {
		std::cout << stack.top() << std::endl;
		stack.pop();
	}
	std::cout << std::endl;
}

#define print(stack) print(stack, __FILE__, __LINE__)

void	test_typedefs() {
	NS::stack<int>::container_type	container;
	container.push_back(1);
	container.back();
	container.pop_back();
	NS::stack<int>::value_type		value;
	NS::stack<int>::size_type		st;
	NS::stack<int>::reference		r = value;
	NS::stack<int>::const_reference	cr = value;
}

void	test_constructors() {
	NS::stack<int>	s;

	s.push(1);
	NS::stack<int>	s3(s);

	print(s);
	print(s3);
}

void	test_assignment_op() {
	NS::stack<int>	s;

	s.push(1);
	s.push(2);
	s.push(3);

	NS::stack<int>	s2;
	s2.push(100);
	s2.push(101);
	s2.push(102);

	print(s);
	print(s2);

	s2 = s;

	print(s);
	print(s2);
}

void	test_top() {
	NS::stack<int>	s;

	s.push(1);
	s.push(2);

	if (s.top() != 2) { PRINTLN("top should be equal to 2"); }

	const NS::stack<int> cs(s);
	const int& i = cs.top();
}

void	test_pop() {
	NS::stack<int>	s;

	s.push(1);
	s.push(2);
	s.push(3);

	print(s);
	s.pop();
	print(s);
	s.pop();
	print(s);
	s.pop();
}

void	test_compare_ops() {
	NS::stack<int>	s;
	NS::stack<int>	s2;

	s.push(1);
	s.push(2);
	s.push(3);

	s2.push(3);
	s2.push(2);
	s2.push(1);

	PRINTLN(s == s2);
	PRINTLN(s != s2);
	PRINTLN(s < s2);
	PRINTLN(s <= s2);
	PRINTLN(s > s2);
	PRINTLN(s >= s2);

	NS::stack<int>	s3(s);
	PRINTLN(s == s3);
	PRINTLN(s != s3);
	PRINTLN(s < s3);
	PRINTLN(s <= s3);
	PRINTLN(s > s3);
	PRINTLN(s >= s3);
}

void	test_all() {
	TIME(test_typedefs());

	TIME(test_constructors());

	TIME(test_assignment_op());

	TIME(test_top());

	TIME(test_pop());

	TIME(test_compare_ops());
}

int	main() {
	TIME(test_all());
	system("leaks a.out");
	PRINTLN("FINISHED");

	return 0;
}
