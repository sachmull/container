#include <iostream>

class Base {
	public:
		Base() {
			std::cout << "Base Constructor\n";
		}

		virtual ~Base() {
			std::cout << "Base Destructor\n";
		}
};

class Derived : public Base {
	public:
		Derived() : Base() {
			std::cout << "Derived Constructor\n";
			throw "n";
		}
};

int	main() {
	Derived	d;
}

