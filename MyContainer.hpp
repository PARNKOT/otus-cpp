#include <memory>

#define DEFAULT_CONTAINER_SIZE 2

template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
public:
    MyContainer() : _size{DEFAULT_CONTAINER_SIZE} {
        allocate_mem(DEFAULT_CONTAINER_SIZE);
    }

    void add(T el) {
        if (_pos  == _size) {
            // reallocate memory
        }

        _mem[pos] = el; 
        _pos += 1;
    }

private:
    void allocate_mem(size_t n) {
        _mem = std::allocator_traits<T>::allocate(_alloc, DEFAULT_CONTAINER_SIZE);
    }

    T* _mem = nullptr;
    size_t _pos = 0;
    size_t _size = 0;

    Allocator _alloc;
};