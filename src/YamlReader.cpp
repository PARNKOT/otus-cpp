#include <stdexcept>
#include <yaml-cpp/yaml.h>
#include "YamlReader.hpp"

UserStructArray YamlReader::read(const fs::path& proto_path) {
    YAML::Node config = YAML::LoadFile(proto_path.c_str());

    UserStructArray parsed_structures;

    if (!config["structures"]) {
        throw std::runtime_error("Keyword \"structures\" has not been found in yaml");
    }

    if (!config["structures"].IsSequence()) {
        throw std::runtime_error("Keyword \"structures\" type is not a sequence");
    }

    YAML::Node structures = config["structures"];

    for (int i = 0; i < structures.size(); ++i) {
        UserStruct user_struct(parse_name(structures[i]));
        
        auto fields = parse_fields(structures[i]);
        for (const auto& field : fields) {
            Field user_field;
            user_field.name = field.first;
            user_field.type = string_to_type(field.second);

            user_struct.add_field(user_field);
        }

        parsed_structures.push_back(user_struct);
    }

    return parsed_structures;
}

std::string YamlReader::parse_name(const YAML::Node& node) {
    return node["name"].as<std::string>();
}

std::vector<std::pair<std::string, std::string>> YamlReader::parse_fields(const YAML::Node& node) {
    YAML::Node node_fields = node["fields"];
    std::vector<std::pair<std::string, std::string>> out;

    for (int i = 0; i < node_fields.size(); ++i) {
        auto field = node_fields[i];

        out.emplace_back(field["name"].as<std::string>(),
                         field["type"].as<std::string>());
    }

    return out;
}