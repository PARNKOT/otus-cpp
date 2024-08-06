#pragma once
#include <vector>

#include "Field.hpp"

class UserStruct {
public:
    using Fields = std::vector<Field>;

    explicit UserStruct(const std::string& name) : name_{name} {}

    void add_field(Field field) {
        fields_.push_back(field);
    }

    const std::string name() const noexcept {
        return name_;
    }

    const Fields fields() const noexcept  {
        return fields_;
    }
private:
    std::string name_;
    Fields fields_;

};

using UserStructArray = std::vector<UserStruct>;