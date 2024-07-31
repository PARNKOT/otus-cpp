#include <yaml-cpp/yaml.h>
#include "YamlReader.hpp"

UserStructArray YamlReader::read(const fs::path& proto_path) {
    YAML::Node config = YAML::LoadFile(proto_path.c_str());

    UserStructArray parsed_structures;

    return parsed_structures;
}