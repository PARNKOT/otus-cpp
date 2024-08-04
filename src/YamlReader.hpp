#pragma once
#include <yaml-cpp/yaml.h>
#include "ProtoReader.hpp"


class YamlReader : public ProtoReader  {
public:
    virtual UserStructArray read(const fs::path& proto_path) override;

private:

    std::string parse_name(const YAML::Node& node);
    std::vector<std::pair<std::string, std::string>> parse_fields(const YAML::Node& node);

};