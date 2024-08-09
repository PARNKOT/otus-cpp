#pragma once
#include "CodeGenerator.hpp"

class CppGenerator : public CodeGenerator {
public:
    explicit CppGenerator(std::string_view template_path) : template_path_(template_path) {}

    std::string generate(const UserStruct& user_struct) override;
private:
    std::string generate_encryptor_code(const UserStruct& user_struct, const std::string& encrypt_type);
    std::string generate_decryptor_code(const UserStruct& user_struct, const std::string& encrypt_type);

    

    std::string_view template_path_;
};
