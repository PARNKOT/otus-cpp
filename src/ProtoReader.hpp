#pragma once
#include <vector>
#include <filesystem>
#include "UserStruct.hpp"

namespace fs = std::filesystem;

using UserStructArray = std::vector<UserStruct>;

class ProtoReader {
public:
    virtual UserStructArray read(const fs::path& proto_path) = 0;
private:
};