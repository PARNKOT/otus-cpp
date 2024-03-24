#include <iostream>
#include <memory>

#define DEFAULT_POOL_SIZE 10

template <typename T>
struct MyAllocator {
    using value_type = T;

    MyAllocator () noexcept {}
    //_pool (::operator new(_N * sizeof(T))) {}
    template <class U> MyAllocator (const MyAllocator<U>&) noexcept {}

    T* allocate(size_t n) {
        if (n > (_pool_size - _alloc_n))  {
            throw std::bad_alloc();
        }

        if (_pool == nullptr) {
            allocate_pool();
        }
        
        T* allocated_mem;
        
        allocated_mem = _pool + _alloc_n;
        _alloc_n += n;
        
        std::cout << "[MyAllocator] Allocated: " << _alloc_n << ", bytes: " << n * sizeof(T) << std::endl;

        return allocated_mem;
    }

    void deallocate(T* mem, size_t n) {
        if (n > _alloc_n) {
            _alloc_n = 0;
        }

        _alloc_n -= n;

        std::cout << "[MyAllocator] Deallocate memory size = " << n * sizeof(T) << ", allocated: " << _alloc_n <<  std::endl;
        // std::cout << "[MyAllocator] Allocated: " << _alloc_n << std::endl;
        //delete[] mem;
    }

    ~MyAllocator() {
        if (_pool != nullptr) {
            std::cout << "[MyAllocator] Deleting pool" << std::endl;
            delete[] _pool;
        }
    }

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

private:
    void allocate_pool() {
        if (_pool != nullptr)  {
            //  TODO: delete
        }

        _pool = new T[_pool_size * sizeof(T)];
    }

    static constexpr long _pool_size = DEFAULT_POOL_SIZE;

    T* _pool = nullptr;
    size_t _alloc_n = 0;
};

template <class T, class U>
constexpr bool operator== (const MyAllocator<T>& a1, const MyAllocator<U>& a2) noexcept
{
    return true;
}

template <class T, class U>
constexpr bool operator!= (const MyAllocator<T>& a1, const MyAllocator<U>& a2) noexcept
{
    return false;
}