#include <vector>
// #include "vector.hpp"
#include "new_vector.hpp"
#include <memory>

// Debugging
#define PRINTLN(x) std::cout << (x) << std::endl
#define LINE PRINTLN(__LINE__)
#define POS std::cout << __FILE__ << ": " << __LINE__ << std::endl

#ifndef NS
#define NS ft
#endif


class B {
	public:
		char	*l;
		int		i;

		B() : l(nullptr), i(1) {}
		B(const int& ex) {
			this->i = ex;
			this->l = new char('a');
		}

		virtual ~B() {
			delete this->l;
			this->l = nullptr;
		}
};

class A : public B {
	public:
		A() : B() {}
		A(const B* ex) {
			this->l = new char(*(ex->l));
			this->i = ex->i;
			if (ex->i == -1) throw "n";
		}

		~A() {
			delete this->l;
			this->l = nullptr;
		}
};


int	main() {
	std::vector<B*> b;
	std::vector<A>	a;

	std::unique_ptr<B>	k1(new B(1));
	std::unique_ptr<B>	k2(new B(2));
	std::unique_ptr<B>	k3(new B(-1));

	b.push_back(&(*k1));
	b.push_back(&(*k2));
	b.push_back(&(*k3));

	try {
		// a.insert(a.begin(), b.begin(), b.end());
		std::vector<A>	tmp(b.begin(), b.end());
	} catch (...) {}

	system("leaks a.out");

	return 0;
}
