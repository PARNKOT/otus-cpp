#include <iostream>
#include <fstream>
#include <memory>
#include <yaml-cpp/yaml.h>
#include "version.hpp"
#include "YamlReader.hpp"
#include "CppGenerator.hpp"

void print_struct_array(const UserStructArray& arr) {
    for (const auto& s : arr) {
        std::cout << "UserStruct " << s.name() << " has fields: ";

        for (const auto& field : s.fields()) {
            std::cout << field.name << "(" << (int)field.type << ")" << ",";
        }

        std::cout << "\b\b" << std::endl;
    }
}

bool save_generated_code(std::string_view code, const fs::path& path) {
    std::ofstream file(path);

    if (!file.is_open()) {
        return false;
    }

    file.write(code.data(), code.size());

    return file.good();
}

int main(int argc, char* argv[])  {
    std::shared_ptr<ProtoReader> reader = std::make_shared<YamlReader>();

    UserStructArray struct_array;

    try {
        struct_array = reader->read("./proto/test.yaml");
    } catch (const YAML::BadFile& ex) {
        std::cout << "Cannot read proto file" << std::endl;
        std::cout << ex.what() << std::endl;
    }

    print_struct_array(struct_array);

    std::unique_ptr<CodeGenerator> generator = std::make_unique<CppGenerator>("./templates/cpp_h.txt");

    constexpr char* generate_to_dir = "./generated";
    if (!fs::exists(generate_to_dir)) {
        fs::create_directories(generate_to_dir);
    }

    for (const auto& s : struct_array) {
        auto str = generator->generate(s);
        save_generated_code(str, fs::path(generate_to_dir) / fs::path(s.name() + ".hpp"));
    }

    return 0;
}

// TEST

#include <type_traits>

using Byte = uint8_t;
using ByteVector = std::vector<uint8_t>;


template<typename T, typename std::enable_if<std::is_integral_v<T>>* = nullptr>
ByteVector to_bytes(T var) {
    size_t size = sizeof(T);
    ByteVector out(size);

    for (size_t i = 0; i < size; ++i) {
        out[size - i - 1] = (var >> 8 * i) & 0xFF;
    }
 
    return out;
}

template<typename T, typename std::enable_if<std::is_same_v<T, double>>* = nullptr>
ByteVector to_bytes(T var) {
    union {
        double from;
        uint64_t to;
    } tmp;
    tmp.from = var;
    
    return to_bytes(tmp.to);
}

template<typename T, typename std::enable_if<std::is_same_v<T, std::string>>* = nullptr>
ByteVector to_bytes(T var) {
    ByteVector out;
    out.reserve(var.size());

    std::copy(var.cbegin(), var.cend(), std::back_inserter(out));

    return out;
}
