#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <string>


// template<typename T, typename std::enable_if_t<std::is_same_v<T, const char*>>* = nullptr>
// void print_ip(T ip) {
//     std::cout << ip << std::endl;
// }

template<typename T, size_t N = sizeof(T), typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
void print_ip(T number) {
    for (int i = N - 1; i >= 0; --i) {
        uint8_t byte = (number >> i * 8);
        std::cout << std::to_string(byte) << ".";
    }
    std::cout << "\b " << std::endl;
}


template<
    typename T,
    typename std::enable_if_t<std::is_same_v<T, std::string> || std::is_same_v<T, const char*>>* = nullptr
>
void print_ip(T ip) {
    std::cout << ip << std::endl;
}


template<
    typename T,
    typename value_type = typename T::value_type,
    typename std::enable_if_t<std::is_same_v<T, std::vector<value_type>> || std::is_same_v<T, std::list<value_type>>>* = nullptr
>
void print_ip(T ip) {
    for (const auto& el : ip) {
        std::cout << el << ".";
    }

    std::cout << "\b " << std::endl;
}


int main(int argc, char* argv[]) {
    print_ip(int8_t{-1});
    print_ip(int16_t{0});
    print_ip(int32_t{2130706433});
    print_ip(int64_t{8875824491850138409});
    print_ip(std::string("Hello world!"));
    print_ip("Hello world!");
    print_ip(std::vector<int>{100, 200, 300, 400 });
    print_ip(std::list<int>{123, 456, 789, 0 });
    
    return 0;
}