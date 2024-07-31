#include <iostream>
#include <memory>
#include <yaml-cpp/yaml.h>
#include "version.hpp"
#include "YamlReader.hpp"

int main(int argc, char* argv[])  {
    std::shared_ptr<ProtoReader> reader = std::make_shared<YamlReader>();

    try {
        reader->read("./proto/test.yaml");
    } catch (const YAML::BadFile& ex) {
        std::cout << "Cannot read proto file" << std::endl;
        std::cout << ex.what() << std::endl;
    }

    return 0;
}