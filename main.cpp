
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "version.hpp"
#include "FileReader.hpp"
#include "Hashing.hpp"
#include "FilesComparor.hpp"
#include "constants.hpp"

#define SCAN_LEVEL_DEFAULT 1
#define FILE_MIN_SIZE_DEFAULT 1 // bytes
#define BLOCK_SIZE_DEFAULT 5


int main(int argc, char const *argv[])
{
    std::cout << "Version: " << version() << std::endl;

    // Parsing options
    namespace po = boost::program_options;

    std::vector<std::string> scan_directories; 
    std::vector<std::string> exclude_directories;
    uint scan_level = 0;
    uint file_min_size = 0;
    std::string mask;
    uint block_size = 0;
    std::string hash;

    po::options_description desc("Options");

    // Options
    desc.add_options()
        ("help", "TODO")
        ("input-dir,d", po::value<std::vector<std::string>>(&scan_directories), "Directory to scan")
        ("exclude,e", po::value<std::vector<std::string>>(&exclude_directories), "Directory to exclude from scanning")
        ("scan-level,l", po::value<uint>(&scan_level)->default_value(scan_level_default), "Level of scanning (0 - not recursive | 1 - recursive)")
        ("min-size,s", po::value<uint>(&file_min_size)->default_value(file_min_size_default), "Minimum  file size in bytes")
        ("mask", po::value<std::string>(&mask), "File mask")
        ("block-size,S", po::value<uint>(&block_size)->default_value(block_size_default), "Size of block to read from file")
        ("hash,H", po::value<std::string>(&hash)->default_value(hash_type_default), "Hashing algorithm, [crc16 | md5]")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //  Positional options
    po::positional_options_description pos_desc;
    pos_desc.add("input-dir", -1);
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if(scan_directories.size() == 0) {
        std::cerr << "Input directories are not provided"  << std::endl;
        return -1;
    }

    std::cout << "Running application with options:" << std::endl;
    std::cout << "\tscan-level = " << scan_level << std::endl;
    std::cout << "\tmin-size = " << file_min_size << std::endl;
    std::cout << "\tmask = " << mask << std::endl;
    std::cout << "\tblock-size = " << block_size << std::endl;
    std::cout << "\thash = " << hash << std::endl;

    std::cout << "\texclude directories:" << std::endl;
    for (const auto& dir : exclude_directories)
        std::cout << "\t\t- " << dir << std::endl;

    std::cout << "\tscan directories:" << std::endl;
    for (const auto& dir : scan_directories)
        std::cout << "\t\t- " << dir << std::endl;

    auto file_reader = std::make_shared<FileReaderBlock>(block_size);

    FilesComparor comparor{scan_directories, file_reader};

    if (exclude_directories.size() > 0) {
        comparor.add_exclude_directories(exclude_directories);
    }

    if (mask.size() > 0) {
        comparor.set_file_mask(mask);
    }

    if (boost::iequals(hash, "md5")) {
        comparor.set_hash_type(hashing::HashType::MD5);
    } else if (boost::iequals(hash, "crc32")) {
        comparor.set_hash_type(hashing::HashType::CRC32);
    
    comparor.set_scan_level(scan_level);

    auto groups = comparor.run();

    int counter = 1;
    for (const auto& group : groups) {
        std::cout << "Group #" << counter << std::endl;

        for (const auto& file : group) {
            std::cout << file << std::endl;
        }
        std::cout << std::endl;

        ++counter;
    }

    return 0;
}
