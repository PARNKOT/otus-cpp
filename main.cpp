#include <iostream>
#include <map>
#include "version.hpp"
#include "MyAllocator.hpp"
#include "MyContainer.hpp"

long factorial(long n) {
    if (n == 0 || n == 1) {
        return 1;
    }

    return factorial(n-1) * n;
}

template<typename T>
void fill_map(T&  map){
    for (int i = 0; i < DEFAULT_POOL_SIZE; ++i) {
        map[i] = factorial(i);
    }
}

template<typename T>
void print_map(T&  map){
    for (const auto& p :  map) {
        std::cout << p.first << " "  << p.second  << std::endl;
    }
}

int main(int argc, char* argv[])  {
    std::map<int, int> m1;

    fill_map(m1);

    std::map<int, int, std::less<int>, MyAllocator<std::pair<int, int>>> m2;

    fill_map(m2);
    std::cout << "Map container with MyAllocator:" << std::endl;
    print_map(m2);

    MyContainer<int> c;

    c.add(1);
    c.add(2);
    c.add(3);
    
    return 0;
}
