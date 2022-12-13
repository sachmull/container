#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>

#include <memory>
#include <stdexcept>
#include <cstring>

#include "iterator_traits.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "swap.hpp"
#include "reverse_iterator.hpp"
#include "lexicographical_compare.hpp"

namespace ft {
	template <class T, class Allocator = std::allocator<T> >
	class vector {
		private:
			class Iterator;

		public:
			typedef	T									value_type;
			typedef	Allocator							allocator_type;
			typedef	size_t								size_type;
			typedef	ptrdiff_t							difference_type;
			typedef	value_type&							reference;
			typedef	const value_type&					const_reference;
			typedef	typename Allocator::pointer			pointer;
			typedef	typename Allocator::const_pointer	const_pointer;
			typedef	Iterator							iterator;
			typedef	const iterator						const_iterator;
			typedef	reverse_iterator<iterator>			reverse_iterator;
			typedef	const reverse_iterator				const_reverse_iterator;

		private:
			pointer			_data;
			size_type		_size;
			size_type		_capacity;
			allocator_type	_allocator;

		public:
			// Constructors
			vector() : _data(NULL), _size(0), _capacity(0), _allocator() {}
			explicit	vector(const Allocator& alloc) : _data(NULL), _size(0), _capacity(0), _allocator(alloc) {}
			explicit	vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
			: _data(NULL), _size(0), _capacity(0), _allocator(alloc)
			{
				resize(count, value);
			}
			template <class InputIt>
			vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			: _data(NULL), _size(0), _capacity(0), _allocator(alloc)
			{
				assign(first, last);
			}
			vector(const vector& other) : _data(NULL), _size(0), _capacity(0), _allocator()
			{
				*this = other;
			}

			~vector() { clear(); _allocator.deallocate(_data, _capacity); _data = NULL; }

			vector&	operator=(const vector& other) {
				if (this != &other) {
					// for (size_type i = 0; i < _size; ++i) {
					// 	_allocator.destroy(_data + i);
					// }
					_allocator.deallocate(_data, _capacity);
					_capacity = 0;
					_size = 0;

					_allocator = other._allocator;
					_data = _allocator.allocate(other._capacity);
					for (size_type i = 0; i < other._size; ++i) {
						_allocator.construct(_data + i, other[i]);
					}
					_size = other._size;
					_capacity = other._capacity;
				}

				return *this;
			}

			void	assign(size_type count, const T& value) {
				clear();
				reserve(count);

				for (size_type i = 0; i < count; ++i) {
					//_data[i] = value;
					_allocator.construct(_data + i, value);
				}
				_size = count;
			}

			template <class InputIt>
			void	assign(InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last) {
				clear();

				size_type new_size = 0;
				for (InputIt it = first; it != last; ++it) {
					++new_size;
				}

				reserve(new_size);

				for (size_type i = 0; i < new_size; ++i) {
					//_data[i] = *first;
					_allocator.construct(_data + i, *first);
					++first;
				}
				_size = new_size;
			}

			allocator_type	get_allocator() const { return _allocator; }

			// Element access
			reference	at(size_type pos) {
				if (pos >= _size) {
					throw std::out_of_range("vector");
				}
				return _data[pos];
			}
			const_reference	at(size_type pos) const {
				if (pos >= _size) {
					throw std::out_of_range("vector");
				}
				return _data[pos];
			}
			reference	operator[](size_type pos) { return _data[pos]; }
			const_reference	operator[](size_type pos) const { return _data[pos]; }
			reference	front() { return _data[0]; }
			const_reference	front() const { return _data[0]; }
			reference	back() { return _data[_size - 1]; }
			const_reference	back() const  { return _data[_size - 1]; }
			pointer	data() { return _data; }
			const_pointer	data() const { return _data; }

			// Iterators
			iterator	begin()	{ return _data; }
			const_iterator	begin() const { return _data; }
			iterator	end() { return _data + _size; }
			const_iterator	end() const { return _data + _size; }
			reverse_iterator	rbegin() { return reverse_iterator(_data + _size); }
			const_reverse_iterator	rbegin() const { return const_reverse_iterator(_data + _size); }
			reverse_iterator	rend() { return reverse_iterator(_data); }
			const_reverse_iterator	rend() const { return const_reverse_iterator(_data); }

			// Capacity
			bool	empty() const { return _size == 0; }
			size_type	size() const { return _size; }
			size_type	max_size() const { return _allocator.max_size(); }
			void	reserve(size_type new_cap) {
				if (new_cap > max_size())
					throw std::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported size");

				if (new_cap > _capacity) {
					// Allocate new memory
					pointer	new_data = _allocator.allocate(new_cap);

					// Copy from old to new
					for (size_type i = 0; i < _size; ++i) {
						//new_data[i] = _data[i];
						_allocator.construct(new_data + i, _data[i]);
						_allocator.destroy(_data + i);
					}

					_allocator.deallocate(_data, _capacity);

					_data = new_data;
					_capacity = new_cap;
				}
			}
			size_type	capacity() const { return _capacity; }

			// Modifiers
			void	clear() { erase(begin(), end()); }

			iterator	insert(const_iterator pos, const T& value) {
				difference_type	pos_idx = pos - _data;
				if (_size == _capacity)
					reserve(_size * 2);
				difference_type i = _size;
				for (; i > pos_idx; --i) {
					//_data[i] = _data[i - 1];
					_allocator.construct(_data + i, _data[i - 1]);
				}
				//_data[i] = value;
				_allocator.construct(_data + i, value);
				++_size;
				return iterator(_data + pos_idx);
			}

			iterator	insert(const_iterator pos, size_type count, const T& value) {
				size_type	pos_idx = pos - _data;

				// Reserve new space
				reserve(_size + count);

				// Move old elements to create space for new elements
				if (_size > 0) {
					for (difference_type i = _size - 1; i >= pos_idx; --i) {
						//_data[i + count] = _data[i];
						_allocator.construct(_data + i + count, _data[i]);
					}
				}

				// Insert new elements
				for (size_type i = 0; i < count; ++i) {
					//_data[pos_idx + i] = value;
					_allocator.construct(_data + pos_idx + i, value);
				}
				
				_size += count;
				return _data + pos_idx;
			}

			template <class InputIt>
			iterator	insert(const_iterator pos, InputIt first, typename enable_if<!is_integral<InputIt>::value, InputIt>::type last) {
				if (first == last)
					return pos;
				difference_type			pos_idx = pos - _data;
				vector<value_type>	tmp(first, last);
				const size_type		offset = tmp.size();

				// Reserve new space
				size_type	new_cap = (_capacity != 0) ? _capacity : 1;
				while (_size + offset > new_cap)
					new_cap *= 2;
				reserve(new_cap);

				// Move values to create space to insert into
				if (_size > 0) {
					for (difference_type i = _size - 1; i >= pos_idx && i >= 0; --i) {
						//_data[i + offset] = _data[i];
						_allocator.construct(_data + i + offset, _data[i]);
					}
				}

				// Insert new values
				for (size_type i = 0; i < offset; ++i) {
					//_data[pos_idx + i] = tmp[i];
					_allocator.construct(_data + pos_idx + i, tmp[i]);
				}
				_size += tmp._size;

				return _data + pos_idx;
			}

			iterator	erase(iterator pos) {
				iterator it = pos;
				iterator jt = pos + 1;
				const_iterator e = end();
				while (jt != e) {
					*it = *jt;
					++it;
					++jt;
				}
				--_size;
				_allocator.destroy(end().base());
				return pos;
			}

			iterator	erase(iterator first, iterator last) {
				iterator	ret = first;

				for (iterator it = first; it != last; ++it) {
					_allocator.destroy(it.base());
				}
				while (last != end()) {
					*first = *last;
					++first;
					++last;
				}
				_size -= last - first;

				return ret;
			}

			void	push_back(const T& value) {
				T	tmp = value;
				if (_size == _capacity) {
					reserve((_size == 0) ? 1 : _size * 2);
				}
				//_data[_size++] = value;
				_allocator.construct(_data + _size, tmp);
				++_size;
			}

			void	pop_back() {
				erase(_data + _size - 1);
			}

			void	resize(size_type count, T value = T()) {
				if (count > _size) {
					size_type	new_cap = _capacity;
					while (count > new_cap)
						new_cap *= 2;
					reserve(new_cap);
					while (count > _size) {
						//_data[_size++] = value;
						_allocator.construct(_data + _size++, value);
					}
				} else if (count < _size) {
					size_type	diff = _size - count;
					erase(end() - diff, end());
				}
			}

			void	swap(vector& other) {
				pointer	tmp_data = _data;
				size_type	tmp_size = _size;
				size_type	tmp_cap = _capacity;
				allocator_type	tmp_alloc = _allocator;

				_data = other._data;
				_size = other._size;
				_capacity = other._capacity;
				_allocator = other._allocator;

				other._data = tmp_data;
				other._size = tmp_size;
				other._capacity = tmp_cap;
				other._allocator = tmp_alloc;
				// ::swap(_data, other._data);
				// ::swap(_size, other._size);
				// ::swap(_capacity, other._capacity);
				// ::swap(_allocator, other._allocator);
			}

			private:
				// Iterator
				class	Iterator {
					public:
						typedef	value_type						value_type;
						typedef	pointer							pointer;
						typedef	reference						reference;
						typedef	difference_type					difference_type;
						typedef	std::random_access_iterator_tag	iterator_category;

					private:
						pointer	_base;

					public:
						Iterator() : _base(NULL) {}
						Iterator(pointer base) : _base(base) {}
						Iterator(const Iterator& other) { *this = other; }
						Iterator&	operator=(const Iterator& other) { _base = other._base; return *this;}
						pointer	base() { return _base; }
						const_pointer	base() const {return _base; }

						Iterator	operator+(const difference_type n) const { return _base + n; }
						Iterator&	operator+=(const difference_type n) { _base += n; return *this; }
						Iterator	operator+(const Iterator& rhs) const { return Iterator(_base + rhs._base); }

						Iterator	operator-(const difference_type n) const { return _base - n; }
						Iterator&	operator-=(const difference_type n) { _base -= n; return *this; }
						difference_type	operator-(const Iterator& rhs) const { return _base - rhs._base; }

						reference		operator[](const difference_type n) const { return _base[n]; }
						reference		operator*()	const { return *_base; }

						bool	operator<(const Iterator& rhs) const { return _base < rhs._base; }
						bool	operator<=(const Iterator& rhs) const {return _base <= rhs._base; }
						bool	operator>(const Iterator& rhs) const { return _base > rhs._base; }
						bool	operator>=(const Iterator& rhs) const { return _base >= rhs._base; }
						bool	operator==(const Iterator& rhs) const {return _base == rhs._base; }
						bool	operator!=(const Iterator& rhs) const { return _base != rhs._base; }

						Iterator	operator++(int _) { (void)_; Iterator tmp(*this); ++_base; return tmp; }
						Iterator&	operator++()	{ ++_base; return *this; }

						Iterator	operator--(int _) { (void)_; Iterator tmp(*this); --_base; return tmp; }
						Iterator&	operator--() { --_base; return *this; }

						friend Iterator	operator+(const difference_type lhs, const Iterator& rhs) { return Iterator(rhs._base + lhs); }
				};
	};

	template <class T, class Alloc>
	bool	operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		if (lhs.size() == rhs.size()) {
			for (typename vector<T, Alloc>::size_type i = 0; i < lhs.size(); ++i) {
				if (!(lhs[i] == rhs[i])) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	template <class T, class Alloc>
	bool	operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs == rhs); }

	template <class T, class Alloc>
	bool	operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
	bool	operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}

	template <class T, class Alloc>
	bool	operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lhs < rhs || lhs == rhs;
	}

	template <class T, class Alloc>
	bool	operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return lhs > rhs || lhs == rhs;
	}


	template <class T, class Alloc>
	void	swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}

#endif
