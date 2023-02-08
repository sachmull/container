#ifndef TEST_HELPERS_HPP
#define TEST_HELPERS_HPP

#include <time.h>
#include <stdio.h>
#include <sstream>

#include "is_const.hpp"
#include <type_traits>

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

#endif
