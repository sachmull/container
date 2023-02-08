#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <sys/types.h>

#include "reverse_iterator.hpp"
#include "distance.hpp"
#include "enable_if.hpp"
#include "lexicographical_compare.hpp"
#include "is_integral.hpp"
#include "swap.hpp"

namespace ft {

	template <class T>
	class vector_iterator {
		public:
			typedef	T								value_type;
			typedef	T*								pointer;
			typedef	T&								reference;
			typedef	std::ptrdiff_t					difference_type;
			typedef	std::random_access_iterator_tag	iterator_category;

		private:
			pointer	_base;

		public:
			vector_iterator() : _base(NULL) {}
			vector_iterator(pointer base) : _base(base) {}
			vector_iterator(const vector_iterator& other) : _base(NULL) { *this = other; }
			~vector_iterator() {}

			operator	vector_iterator<const T>() const { return _base; }

			vector_iterator&	operator=(const vector_iterator& other) {
				if (this != &other) {
					_base = other._base;
				}

				return *this;
			}

			pointer	base() const { return _base; }

			vector_iterator			operator+(const difference_type n) const { return _base + n; }
			friend vector_iterator	operator+(const difference_type n, const vector_iterator& it) { return it._base + n; }
			vector_iterator&		operator+=(const difference_type n) { _base += n; return *this; }

			vector_iterator		operator-(const difference_type n) const { return _base - n; }
			difference_type		operator-(const vector_iterator& other) const { return _base - other._base; }
			vector_iterator&	operator-=(const difference_type n) { _base -= n; return *this; }

			reference	operator[](const difference_type n) const { return _base[n]; }
			reference	operator*() const { return *_base; }
			pointer		operator->() const { return _base; }

			vector_iterator&	operator++() { ++_base; return *this; }
			vector_iterator		operator++(int) { vector_iterator it(*this); ++_base; return it; }

			vector_iterator&	operator--() { --_base; return *this; }
			vector_iterator		operator--(int) { vector_iterator it(*this); --_base; return it; }

			bool	operator==(const vector_iterator<const T>& other) const { return _base == other.base(); }
			bool	operator!=(const vector_iterator<const T>& other) const { return _base != other.base(); }

			bool	operator<(const vector_iterator<const T>& other) const { return _base < other.base(); }
			bool	operator<=(const vector_iterator<const T>& other) const { return _base <= other.base(); }

			bool	operator>(const vector_iterator<const T>& other) const { return _base > other.base(); }
			bool	operator>=(const vector_iterator<const T>& other) const { return _base >= other.base(); }
	};

	template <class T, class Allocator = std::allocator<T> >
	class vector {
		public:
			typedef	T										value_type;
			typedef	Allocator								allocator_type;
			typedef	std::size_t								size_type;
			typedef	std::ptrdiff_t							difference_type;
			typedef	value_type&								reference;
			typedef	const value_type&						const_reference;
			typedef	typename Allocator::pointer				pointer;
			typedef	typename Allocator::const_pointer		const_pointer;
			typedef	vector_iterator<value_type>				iterator;
			typedef	vector_iterator<const value_type>		const_iterator;
			typedef	reverse_iterator<iterator>				reverse_iterator;
			typedef	ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		private:
			pointer			_data;
			allocator_type	_allocator;
			size_type		_size;
			size_type		_capacity;

		public:
			vector() : _data(NULL), _allocator(), _size(0), _capacity(0) {}

			explicit vector(const allocator_type& alloc) : _data(NULL), _allocator(alloc), _size(0), _capacity(0) {}

			explicit vector(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type())
			: _data(NULL), _allocator(alloc), _size(0), _capacity(0) {
				vector	buf;
				buf.reserve(count);
				buf.resize(count, value);
				swap(buf);
			}

			template <class InputIt> // do i need enable if?
			vector(InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last, const allocator_type& alloc = allocator_type())
			: _data(NULL), _allocator(alloc), _size(0), _capacity(0) {
				vector	buf;
				buf.assign(first, last);
				swap(buf);
			}

			vector(const vector& other) : _data(NULL), _allocator(), _size(0), _capacity(0) {
				*this = other;
			}

			~vector() {
				clear_deallocate();
			}

			vector&	operator=(const vector& other) {
				if (this != &other) {
					clear_deallocate();

					vector	buf(other._allocator);
					buf.assign(other.begin(), other.end());
					swap(buf);
				}

				return *this;
			}

			void	assign(size_type count, const value_type& value) {
				clear();
				reserve(count);

				for (size_type idx = 0; idx < count; ++idx) {
					assign_or_construct(_data + idx, value);
					++_size;
				}
			}

			template <class InputIt>
			void	assign(InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last) {
				// store in buf
				vector	buf;
				for (; first != last; ++first) {
					buf.push_back(*first);
				}

				// clear old content
				clear();

				reserve(buf.size());

				// insert new content
				for (size_type idx = 0; idx < buf.size(); ++idx) {
					assign_or_construct(_data + idx, buf[idx]);
					++_size;
				}
			}

			allocator_type	get_allocator() const { return _allocator; }

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

			const_reference	back() const { return _data[_size - 1]; }

			pointer	data() {return _data; }

			const_pointer	data() const { return _data; }

			iterator	begin() { return iterator(_data); }

			const_iterator	begin() const { return const_iterator(_data); }

			iterator	end() { return iterator(_data + _size); }

			const_iterator	end() const { return const_iterator(_data + _size); }

			reverse_iterator	rbegin() { return reverse_iterator(end()); }

			const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }

			reverse_iterator	rend() { return reverse_iterator(begin()); }

			const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

			bool	empty() const { return _size == 0; }

			size_type	size() const { return _size; }

			size_type	max_size() const { return _allocator.max_size(); }

			void	reserve(size_type new_cap) {
				if (new_cap > max_size()) {
					throw std::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported size");
				}

				if (new_cap > _capacity) {
					vector	buf(_allocator);
					buf._data = buf._allocator.allocate(new_cap);
					buf._capacity = new_cap;

					for (size_type idx = 0; idx < _size; ++idx) {
						buf.assign_or_construct(buf._data + idx, _data[idx]);
						++buf._size;
					}

					swap(buf);
				}
			}

			size_type	capacity() const { return _capacity; }

			void	clear() { if (_size > 0) { erase(begin(), end()); } }

			iterator	insert(const_iterator pos, const value_type& value) {
				// use an index insead of an iterator, in case a reallocation happens and invalidates all iterators
				difference_type	pos_idx = pos - _data;

				// allocate space if necessary
				if (_size == _capacity) {
					reserve((_capacity == 0) ? 1 : _capacity * 2);
				}

				// move old values
				difference_type	insert_idx = _size;
				while (insert_idx > pos_idx) {
					assign_or_construct(_data + insert_idx, _data[insert_idx - 1]);
					--insert_idx;
				}

				// insert new value
				assign_or_construct(_data + insert_idx, value);
				++_size;

				return iterator(_data + pos_idx);
			}

			iterator	insert(const_iterator pos, size_type count, const value_type& value) {
				// use an index insead of an iterator, in case a reallocation happens and invalidates all iterators
				difference_type	pos_idx = pos - _data;

				// allocate necessary space
				reserve(_size + count);

				// move old values
				if (_size > 0) {
					for (ssize_t idx = _size - 1; idx >= pos_idx; --idx) {
						assign_or_construct(_data + count + idx, _data[idx]);
					}
				}

				// insert new values
				for (size_type idx = 0; idx < count; ++idx) {
					assign_or_construct(_data + pos_idx + idx, value);
				}

				_size += count;
				return iterator(_data + pos_idx);
			}

			template <class InputIt>
			iterator	insert(const_iterator pos, InputIt first, typename enable_if<!is_integral<InputIt>::value, InputIt>::type last) {
				if (first == last) {
					return iterator((value_type*)pos.base());
				}

				vector	buf;
				buf.assign(first, last);
				difference_type	pos_idx = pos - _data;

				size_type	offset = buf.size();

				// reserve necessary space
				size_type new_cap = (_capacity == 0) ? 1 : _capacity;
				while (_size + offset > new_cap) {
					new_cap *= 2;
				}
				reserve(new_cap);

				// move values to create space to insert into
				if (_size > 0) {
					for (difference_type idx = _size - 1; idx >= pos_idx; --idx) {
						assign_or_construct(_data + offset + idx, _data[idx]);
					}
				}

				// insert new elements
				iterator	it = buf.begin();
				for (size_type idx = 0; idx < offset; ++idx) {
					assign_or_construct(_data + pos_idx + idx, *it);
					++it;
				}
				_size += offset;

				return iterator(_data + pos_idx);
			}

			iterator	erase(iterator pos) {
				pointer	current = pos.base();
				pointer	next = current + 1;
				pointer	_end = _data + _size;

				while (next != _end) {
					assign_or_construct(current, *next);
					++current;
					++next;
				}
				_allocator.destroy(_data + (_size - 1));
				--_size;

				return pos;
			}

			iterator	erase(iterator first, iterator last) {
				iterator	_first = first;

				while (last != end()) {
					*first = *last;
					++first;
					++last;
				}

				for (iterator it = first; it != last; ++it) {
					_allocator.destroy(it.base());
				}

				_size -= last - first;

				return _first;
			}

			void	push_back(const value_type& value) {
				value_type	tmp = value;

				if (_size == _capacity) {
					reserve((_capacity == 0) ? 1 : _capacity * 2);
				}

				assign_or_construct(_data + _size, tmp);
				++_size;
			}

			void	pop_back() {
				erase(--end());
			}

			void	resize(size_type count, value_type value = value_type()) {
				if (count > _size) {
					// allocate necessary space
					size_type	new_cap = (_capacity == 0) ? 1 : _capacity;
					while (count > new_cap) {
						new_cap *= 2;
					}
					reserve(new_cap);

					// insert new values
					while (count > _size) {
						assign_or_construct(_data + _size, value);
						++_size;
					}
				} else if (count < _size) {
					erase(end() - (_size - count), end());
				}
			}

			void	swap(vector& other) {
				ft::swap(_data, other._data);
				ft::swap(_allocator, other._allocator);
				ft::swap(_size, other._size);
				ft::swap(_capacity, other._capacity);
			}

		private:
			void	clear_deallocate() {
				if (_size > 0) {
					clear();
				}
				if (_data != NULL && _capacity) {
					_allocator.deallocate(_data, _capacity);
					_data = NULL;
					_capacity = 0;
				}
			}

			#ifdef assign_or_construct
			#undef assign_or_construct
			#endif
			void	assign_or_construct(pointer ptr, const value_type& value, __attribute__((unused))int line = -1) {
				if (ptr - _data < (difference_type)_size) {
					*ptr = value;
				} else {
					_allocator.construct(ptr, value);
				}
			}
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
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
	bool	operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}

	template <class T, class Alloc>
	bool	operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return (lhs < rhs || lhs == rhs);
	}

	template <class T, class Alloc>
	bool	operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		return (lhs > rhs || lhs == rhs);
	}

	template <class T, class Alloc>
	void	swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
		lhs.swap(rhs);
	}
}

#endif
