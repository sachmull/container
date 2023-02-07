#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP

#include <time.h>
#include <stdio.h>
#include <sstream>

#include "is_const.hpp"

#ifndef NS
#define NS ft
#endif

#define PRINTLN(msg)	\
do {	\
	std::cout << __FILE__ << " " << __LINE__ << ": " << (msg) << std::endl;	\
} while(0)

#define TIME(func)	\
do {	\
	clock_t	t;	\
	t = clock();	\
	func;	\
	t = clock() - t;	\
	double	tt = ((double)t) / CLOCKS_PER_SEC;	\
	printf("%fs\n", tt);	\
} while(0)

// #define TEST(func)	\
// do {	\
// 	clock_t	t1, t2;	\
// 	\
// 	t1 = clock();	\
// 	func;	\
// 	t1 = clock() - t1;	\
// 	\
// 	t2 = clock();	\
// 	func;	\
// 	t2 = clock() - t2;	\
// 	\
// 	if (t1 * 20 > t2) { std::cerr << #func << " took to long" << std::endl; }	\
// } while(0)


// Since std::alloctor is stateless, do I even have to test correct copying of the containers allocator
// Allocator
// template <class T>
// class test_allocator {
// 	private:
// 		std::allocator<T>	allocator;
	
// 	public:
// 		typedef	typename std::allocator<T>::value_type	value_type;
// 		typedef	typename std::allocator<T>::pointer		pointer;
// 		typedef	typename std::allocator<T>::const_pointer	const_pointer;
// 		typedef	typename std::allocator<T>::reference		reference;
// 		typedef	typename std::allocator<T>::const_reference	const_reference;
// 		typedef	typename std::allocator<T>::size_type		size_type;
// 		typedef	typename std::allocator<T>::difference_type	difference_type;

// 	public:
// 		test_allocator() : allocator() {}
// 		test_allocator(const test_allocator& other) { *this = other; }
// 		template <class U>
// 		test_allocator(const test_allocator<U>& other) { *this = other; }

// 		~test_allocator() {}
// };

#endif
