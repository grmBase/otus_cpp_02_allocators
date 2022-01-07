#pragma once

#include <iostream>
#include <memory>
#include <vector>

template <typename T, size_t t_unSize>
struct t_allocator {
	using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

	t_allocator();
        ~t_allocator();

        template<typename U>
        struct rebind {
                using other = t_allocator<U, t_unSize>;
        };


	template <typename U>
        t_allocator(const t_allocator<U>&);

	T* allocate (std::size_t n);
	
	void deallocate (T* p, std::size_t n);
};

template <typename T, size_t t_unSize>
t_allocator<T, t_unSize>::t_allocator() {
}

template <typename T, size_t t_unSize>
template <typename U>
t_allocator<T, t_unSize>::t_allocator(const t_allocator<U>&) {
	// should we make a copy of the rhs.m_buffer ?
	// No, we should not!
}

template <typename T, size_t t_unSize>
T* t_allocator<T, t_unSize>::allocate(std::size_t n) {
	auto p = std::malloc(n * sizeof(T));
	if (!p)
		throw std::bad_alloc();
	return reinterpret_cast<T *>(p);
}

template <typename T, size_t total_size>
void t_allocator<T, total_size>::deallocate (T* p, std::size_t ) {
	std::free(p);
}


/*
template <class T, class U>
constexpr bool operator== (const t_allocator<T>&, const t_allocator<U>&) noexcept {
	return false;
}

template <class T, class U>
constexpr bool operator!= (const t_allocator<T>&, const t_allocator<U>&) noexcept {
	return false;
}
*/
