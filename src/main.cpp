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