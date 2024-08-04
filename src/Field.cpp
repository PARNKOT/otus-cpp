#include "Field.hpp"


FieldType string_to_type(const std::string& type) {
    if (type == "bool" || type == "boolean") {
        return FieldType::BOOL;
    } else if (type == "int8_t") {
        return FieldType::INT8_T;
    } else if (type == "int16_t") {
        return FieldType::INT16_T;
    } else if (type == "int32_t") {
        return FieldType::INT32_T;
    } else if (type == "int64_t") {
        return FieldType::INT64_T;
    } else if (type == "uint8_t") {
        return FieldType::UINT8_T;
    } else if (type == "uint16_t") {
        return FieldType::UINT16_T;
    } else if (type == "uint32_t") {
        return FieldType::UINT32_T;
    } else if (type == "uint64_t") {
        return FieldType::UINT64_T;
    } else if (type == "double") {
        return FieldType::DOUBLE;
    } else if (type == "string") {
        return FieldType::STRING;
    }

    return FieldType::UNKNOWN;
}