#include <iostream>
#include "version.hpp"


int main(int argc, char* argv[])  {
    std::cout << "Version: " << version() << std::endl;
    std::cout << "Hello world!" << std::endl;
    return 0;
}