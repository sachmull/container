#ifndef IS_INTEGRAL_HPP
#define IS_INTEGRAL_HPP

namespace ft {
	template <class T>
	struct is_integral {
		typedef	T		type;
		typedef	bool	value_type;

		static const bool	value = false;
	};

	template <>
	struct is_integral<bool> {
		typedef	bool		type;
		typedef	bool		value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<char> {
		typedef	char		type;
		typedef	bool		value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<char16_t> {
		typedef	char16_t		type;
		typedef	bool			value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<char32_t> {
		typedef	char32_t		type;
		typedef	bool			value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<wchar_t> {
		typedef	wchar_t		type;
		typedef	bool			value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<short> {
		typedef	short		type;
		typedef	bool		value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<int> {
		typedef	int		type;
		typedef	bool	value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<long> {
		typedef	long		type;
		typedef	bool		value_type;

		static const bool	value = true;
	};

	template <>
	struct is_integral<long long> {
		typedef	long long		type;
		typedef	bool			value_type;

		static const bool	value = true;
	};
}

#endif
