#include <memory>
#include <cstring>

#define DEFAULT_CONTAINER_SIZE 2

template <typename T, typename Allocator = std::allocator<T>>
class MyContainer {
public:
    MyContainer() : _capacity{DEFAULT_CONTAINER_SIZE} {
        allocate_mem(DEFAULT_CONTAINER_SIZE);
    }

    void add(T el) {
        if (_size  == _capacity) {
            // reallocate memory
            reallocate_mem();
        }

        _mem[_size] = el; 
        _size += 1;
    }

private:
    void allocate_mem(size_t n) {
        _mem = std::allocator_traits<Allocator>::allocate(_alloc, DEFAULT_CONTAINER_SIZE);
    }

    void reallocate_mem() {
        if (_mem == nullptr) {
            return;
        }

        T* tmp_mem = std::allocator_traits<Allocator>::allocate(_alloc, _capacity * 2);

        std::memcpy(tmp_mem, _mem, _size);

        std::allocator_traits<Allocator>::deallocate(_alloc, _mem, _capacity);

        _capacity *= 2;
    }

    T* _mem = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

    Allocator _alloc;
};

// template <class T, class Allocator = std::allocator<T> >
// class my_vector {
// public:
// void push_back(const T& x)
// {
//     if (size == capacity)
//     {
//         capacity = capacity * 2 + 1;
//         T* newData = std::allocator_traits<Allocator>::allocate(alloc, capacity);
//         std::copy(data, data + capacity * sizeof(T), newData); //naive
//         std::swap(newData, data);
//         std::allocator_traits<Allocator>::deallocate(alloc, newData, capacity);
//     }

//     std::allocator_traits<Allocator>::construct(alloc, data + size * sizeof(T), x);
//     ++size;
// }

// private:
//     std::size_t size = 0;
//     std::size_t capacity = 0;
//     T* data = nullptr;

//     Allocator alloc;
// };