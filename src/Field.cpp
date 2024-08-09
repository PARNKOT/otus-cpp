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


std::string type_to_string(FieldType type) {
    switch (type) {
    case FieldType::BOOL: return "bool";
    case FieldType::INT8_T: return "int8_t";
    case FieldType::INT16_T: return "int16_t";
    case FieldType::INT32_T: return "int32_t";
    case FieldType::INT64_T: return "int64_t";
    case FieldType::UINT8_T: return "uint8_t";
    case FieldType::UINT16_T: return "uint16_t";
    case FieldType::UINT32_T: return "uint32_t";
    case FieldType::UINT64_T: return "uint64_t";
    case FieldType::DOUBLE: return "double";
    case FieldType::STRING: return "std::string";
    default: return "unknown";
    }
}