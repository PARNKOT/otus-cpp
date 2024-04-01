#include <iostream>
#include <map>
//#include "version.hpp"
#include "MyAllocator.hpp"
#include "MyContainer.hpp"

#define DEFAULT_SIZE 10

long factorial(long n) {
    if (n == 0 || n == 1) {
        return 1;
    }

    return factorial(n-1) * n;
}

template<typename T>
void fill_map(T&  map){
    for (int i = 0; i < DEFAULT_SIZE; ++i) {
        map[i] = factorial(i);
    }
}

template<typename T>
void print_map(T&  map){
    for (const auto& p :  map) {
        std::cout << p.first << " "  << p.second  << std::endl;
    }
}

template<typename T>
void print_container(T&  container){
    for (const auto& v :  container) {
        std::cout << v << ", ";
    }
    std::cout << "\b\b " << std::endl;
}

int main(int argc, char* argv[])  {
    std::map<int, int> m1;

    fill_map(m1);

    std::map<int, int, std::less<int>, MyAllocator<std::pair<int, int>, DEFAULT_SIZE>> m2;

    fill_map(m2);
    std::cout << "Map container with MyAllocator:" << std::endl;
    print_map(m2);

    MyContainer<int> c1;

    for (int i = 0; i < DEFAULT_SIZE; ++i) {
        c1.add(i);
    }

    MyContainer<int, MyAllocator<int, DEFAULT_SIZE>> c2(DEFAULT_SIZE);

    for (int i = 0; i < DEFAULT_SIZE; ++i) {
        c2.add(i);
    }

    std::cout << "MyContainer with MyAllocator: " << std::endl;
    print_container(c2);

    return 0;
}
