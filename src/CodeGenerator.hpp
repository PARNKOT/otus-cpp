#pragma once
#include <string>
#include "UserStruct.hpp"

class CodeGenerator {
public:
    virtual std::string generate(const UserStruct& user_struct) = 0;
private:
};