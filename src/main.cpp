#include <iostream>
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

    for (const auto& s : struct_array) {
        generator->generate(s);
    }

    return 0;
}