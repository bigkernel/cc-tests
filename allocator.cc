#include <iostream>
#include <utility> // for std::addressof, std::forward
#include <memory> // for GCC implement allocator
#include <new> // for operator new/delete
#include <stdexcept>
#include <vector>

template<typename Tp>
class allocator {
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Tp* pointer;
    typedef const Tp* const_pointer;
    typedef Tp& reference;
    typedef const Tp& const_reference;
    typedef Tp value_type;

    template<typename Tp1>
    struct rebind {
        typedef allocator<Tp1> other;
    };

    pointer address(reference v) const noexcept {
        return std::addressof(v);
    }

    const_pointer address(const_reference v) const noexcept {
        return std::addressof(v);
    }

    size_t max_size() const noexcept {
        return size_type(-1) / sizeof(value_type);
    }

    pointer allocate(size_type n, const void* = 0) {
        if (n > this->max_size())
            throw std::bad_alloc();
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }

    template<typename Up, typename... Args>
    void construct(Up* p, Args&&... args) {
        ::new(static_cast<void*>(p)) Up(std::forward<Args>(args)...);
    }

    template<typename Up>
    void destroy(Up* p) {
        p->~Up();
    }

};

template<>
class allocator<void> {
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef void value_type;

    template<typename Tp1>
    struct rebind {
        typedef allocator<Tp1> other;
    };
};

int main(void) {
    std::vector<int, allocator<int>> iv;
    for (auto i = 0; i < 10; ++i)
        iv.push_back(i);
    for (auto i = 0; i < 10; i++)
        std::cout << iv[i] << std::endl;
    return 0;
}
