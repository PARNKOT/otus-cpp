#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "version.hpp"
// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

#define IPv4 4
#define IPv IPv4

using ip_t = std::array<uint8_t, IPv>;


std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}


ip_t ip_string_to_array(const std::string& ip_str) {
    ip_t ip;

    std::size_t old_pos = 0;
    std::size_t current_pos = ip_str.find_first_of('.');

    for (short i = 0; i < ip.size(); ++i)  {
        ip[i] =  std::stoi(ip_str.substr(old_pos, current_pos), nullptr, 10);
        old_pos = current_pos  + 1;
        current_pos = ip_str.find_first_of('.', old_pos);
    }

    return ip;
}


void print_ip(const ip_t& ip) {
    for (const auto& part : ip) {
        std::cout << std::to_string(part) << ".";
    }
    std::cout << "\b " << std::endl;
}

template <typename T, typename filter, typename print>
void filter_and_print(const T& _container, filter _filter, print _print) {
    for (const auto& el : _container) {
        if (_filter(el)) {
            _print(el);
        }
    }
}

int main(int argc, char const *argv[])
{
    std::cout << "Version: " << version() << std::endl;

    try
    {
        std::vector<ip_t > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(ip_string_to_array(v.at(0)));
        }

        // TODO reverse lexicographically sort

        std::sort(ip_pool.begin(), ip_pool.end(), [](auto ip1, auto ip2){
            for (int i = 0; i < ip1.size(); ++i){
                if (ip1[i] < ip2[i]) {
                    return false;
                } else if (ip1[i] > ip2[i]) {
                    return true;
                }
            }

            return true;
        });

        std::cout << "Sorted ip:" << std::endl;

        for(std::vector<ip_t>::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            print_ip(*ip);
        }

        filter_and_print(ip_pool,
        [](auto ip){
            return ip[0] == 1 ? true : false;
        }, print_ip);

        filter_and_print(ip_pool,
        [](auto ip){
            return (ip[0] == 46 && ip[1] == 70) ? true : false;
        }, print_ip);

        filter_and_print(ip_pool,
        [](auto ip){
            for (const auto& part : ip) {
                if (part == 46) {
                    return true;
                }
            }
            return false;
        }, print_ip);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
