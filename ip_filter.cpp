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

void print_ip(const std::vector<std::string>& ip) {
    for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
    {
        if (ip_part != ip.cbegin())
        {
            std::cout << ".";

        }
        std::cout << *ip_part;
    }
    std::cout << std::endl;
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
        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort

        std::sort(ip_pool.begin(), ip_pool.end(), [](auto ip1, auto ip2){
            for (int i = 0; i < ip1.size(); ++i){
                int part1 = std::stoi(ip1[i], nullptr, 10);
                int part2 = std::stoi(ip2[i], nullptr, 10);

                if (part1 < part2) {
                    return false;
                } else if (part1 > part2) {
                    return true;
                }
            }

            return true;
        });

        std::cout << "Sorted ip:" << std::endl;

        for(std::vector<std::vector<std::string> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            print_ip(*ip);
        }

        filter_and_print(ip_pool,
        [](auto ip){
            int part1 = std::stoi(ip[0], nullptr, 10);
            return part1 == 1 ? true : false;
        }, print_ip);

        filter_and_print(ip_pool,
        [](auto ip){
            int part1 = std::stoi(ip[0], nullptr, 10);
            int part2 = std::stoi(ip[1], nullptr, 10);
            return (part1 == 46 && part2 == 70) ? true : false;
        }, print_ip);

        filter_and_print(ip_pool,
        [](auto ip){
            for (const auto& part : ip) {
                int n = std::stoi(part);
                if (n == 46) {
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
