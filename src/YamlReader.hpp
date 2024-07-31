#pragma once
#include "ProtoReader.hpp"


class YamlReader : public ProtoReader  {
    virtual UserStructArray read(const fs::path& proto_path) override;
};