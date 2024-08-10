#include <fmt/core.h>
#include "inja/inja.hpp"
#include "nlohmann/json.hpp"
#include "CppGenerator.hpp"

std::string CppGenerator::generate(const UserStruct& user_struct) {
    // TODO: Implement

    inja::json data;

    data["struct_name"] = user_struct.name();
    data["fields"] = inja::json::array();

    auto user_fields = user_struct.fields();

    for (int i = 0; i < user_fields.size(); ++i) {
        if (user_fields[i].type == FieldType::UNKNOWN) {
            std::cout << "[ERROR] Field " << user_fields[i].name << " has unknown type" << std::endl; 
            continue;
        }

        auto type_str = type_to_string(user_fields[i].type);
        data["fields"].push_back({{"name", user_fields[i].name}, {"type", type_str}});
    }

    std::string encrypt_type = "std::vector<uint8_t>";
    data["encrypt_type"] = encrypt_type;
    data["encryptor_code"] = generate_encryptor_code(user_struct, encrypt_type);
    data["decryptor_code"] = generate_decryptor_code(user_struct, encrypt_type);

    inja::Environment env;

    auto render_str = env.render_file("./templates/cpp_h.txt", data);

    return render_str;
}

std::string CppGenerator::generate_encryptor_code(const UserStruct& user_struct, const std::string& encrypt_type) {
    std::string out;
    out += "ByteVector bytes;\n";

    for (const auto& field : user_struct.fields()) {
        out += fmt::format("\t\tbytes = to_bytes(user_struct.{0});\n", field.name.c_str());
        out += "\t\tstd::copy(bytes.cbegin(), bytes.cend(), std::back_inserter(out));\n";
    }


    // TODO

    return out;
}

std::string CppGenerator::generate_decryptor_code(const UserStruct& user_struct, const std::string& encrypt_type) {
    std::string out = "size_t offset = 0;\n";

    for (const auto& field : user_struct.fields()) {
        out += fmt::format("\t\toffset += from_bytes(msg, offset, user_struct.{0});\n", field.name);
    }

    // TODO

    return out;
}
