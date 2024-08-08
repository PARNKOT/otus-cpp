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
        data["fields"].push_back({{"name", user_fields[i].name}, {"type", "test"}});
        // data["fields"].push_back((int)user_fields[i].type); // TODO: convert FieldType to string
    }

    data["encrypt_type"] = "SIMPLE_ENCRYPT";

    inja::Environment env;

    auto render_str = env.render_file("./templates/cpp_h.txt", data);

    return render_str;
}