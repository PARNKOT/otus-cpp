#include <iostream>
#include <fstream>
#include <memory>
#include <yaml-cpp/yaml.h>
#include <boost/program_options.hpp>

#include "version.hpp"
#include "YamlReader.hpp"
#include "CppGenerator.hpp"

void print_struct_array(const UserStructArray& arr) {
    for (const auto& s : arr) {
        std::cout << "UserStruct " << s.name() << " has fields: ";

        for (const auto& field : s.fields()) {
            std::cout << field.name << "(" << (int)field.type << ")" << ",";
        }

        std::cout << "\b\b" << std::endl;
    }
}

bool save_generated_code(std::string_view code, const fs::path& path) {
    std::ofstream file(path);

    if (!file.is_open()) {
        return false;
    }

    file.write(code.data(), code.size());

    return file.good();
}

int main(int argc, char* argv[])  {
    // Parsing options
    namespace po = boost::program_options;

    std::string proto_file;
    std::string save_to;
    std::string lang;

    po::options_description desc("Options");

    desc.add_options()
        ("help", "Usage: ./program [options] -p <proto_file>")
        ("proto,p", po::value<std::string>(&proto_file), "Protocol file, [yaml only]")
        ("save,s", po::value<std::string>(&save_to)->default_value("./generated"), "Directory to save")
        ("lang,l", po::value<std::string>(&lang)->default_value("cpp"), "Programming language to generate")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    std::shared_ptr<ProtoReader> reader;

    std::string extension = fs::path(proto_file).extension();
    if (extension == ".yaml" || extension == ".yml") {
        reader = std::make_shared<YamlReader>();
    } else {
        std::cout << "Unknown extension of protocol file: " << extension << std::endl;
        return -1;
    }

    std::unique_ptr<CodeGenerator> generator;


    if (lang == "cpp" || lang == "c++") {
        generator = std::make_unique<CppGenerator>("./templates/cpp_h.txt");
    } else {
        std::cout << "Unknown programming language: " << lang << std::endl;
        return -1;
    }

    if (!fs::exists(save_to)) {
        fs::create_directories(save_to);
    }

    UserStructArray struct_array;

    try {
        struct_array = reader->read(proto_file);
    } catch (const YAML::BadFile& ex) {
        std::cout << "Cannot read proto file" << std::endl;
        std::cout << ex.what() << std::endl;
    }

    print_struct_array(struct_array);

    for (const auto& s : struct_array) {
        auto str = generator->generate(s);
        save_generated_code(str, fs::path(save_to) / fs::path(s.name() + ".hpp"));
    }

    return 0;
}
