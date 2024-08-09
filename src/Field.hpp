#pragma once
#include <string>

enum class FieldType  {
    BOOL,
    INT8_T,
    INT16_T,
    INT32_T,
    INT64_T,
    UINT8_T,
    UINT16_T,
    UINT32_T,
    UINT64_T,
    DOUBLE,
    STRING,
    UNKNOWN,
};

struct Field {
    std::string name;
    FieldType type;
};

FieldType string_to_type(const std::string& type);
std::string type_to_string(FieldType type);
