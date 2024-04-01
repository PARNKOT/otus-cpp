#include <memory>
#include <cstring>

#define DEFAULT_CONTAINER_SIZE 2

template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
public:
    MyContainer(std::size_t n = DEFAULT_CONTAINER_SIZE) : _capacity{n} {
        allocate_mem(_capacity);
    }

    void add(const T& el) {
        if (_size  == _capacity) {
            reallocate_mem();
        }

        _mem[_size] = el; 
        _size += 1;
    }

    T& operator [](std::size_t pos) {
        return _mem[pos];
    }

    T* begin() {
        return _mem;
    }

    T* end() {
        return _mem + _size;
    }

private:
    void allocate_mem(size_t n) {
        _mem = std::allocator_traits<Allocator>::allocate(_alloc, n);
    }

    void reallocate_mem() {
        if (_mem == nullptr) {
            return;
        }

        T* tmp_mem = std::allocator_traits<Allocator>::allocate(_alloc, _capacity * 2);

        std::memcpy(tmp_mem, _mem, _size * sizeof(T));

        std::allocator_traits<Allocator>::deallocate(_alloc, _mem, _capacity);

        _mem = tmp_mem;

        _capacity *= 2;
    }

    T* _mem = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

    Allocator _alloc;
};

