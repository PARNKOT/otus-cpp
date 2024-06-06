#pragma once
#include <filesystem>
#include <vector>
#include "Hashing.hpp"
#include "FileReader.hpp"
#include "constants.hpp"

namespace fs = std::filesystem;

using path_t = std::string;

struct FilesComparor {
    using group_t = std::vector<path_t>;
    using compare_result_t = std::vector<group_t>;

    FilesComparor(const std::vector<path_t>& directories) {
        set_scan_directories(directories);
        file_reader_ = std::make_shared<FileReadBlockStrategy>(block_size_default);
    }

    FilesComparor(const std::vector<path_t>& directories, std::shared_ptr<FileReadStrategy> file_reader) : file_reader_{file_reader}  {
        set_scan_directories(directories);
    }

    void set_hash_type(hashing::HashType hash_type) {
        hash_type_ = hash_type;
    }

    void set_file_reader(std::shared_ptr<FileReadStrategy> file_reader)  {
        file_reader_ = file_reader;
    }

    void add_exclude_directories(const std::vector<path_t>& exclude) {
        exclude_directories_.resize(exclude.size());

        std::for_each(exclude.cbegin(), exclude.cend(), [&](const auto& dir) {
            scan_directories_.emplace_back(dir);
        });
    }

    void set_file_mask(const std::string& mask) {
        file_mask_ = mask;
    }

    void set_scan_level(uint scan_level) {
        scan_level_ = scan_level;
    }

    compare_result_t run() {
        std::vector<fs::path> files;
        files.reserve(50);

        return {
            {"a1", "a2", "a3"},
            {"b1", "b2", "b3"},
            {"c1", "c2", "c3"}
        };
    }

private:
    void set_scan_directories(const std::vector<path_t>& directories) {
        scan_directories_.resize(directories.size());

        std::for_each(directories.cbegin(), directories.cend(), [&](const auto& dir) {
            scan_directories_.emplace_back(dir);
        });
    }

    std::vector<fs::path> scan_directories_;
    std::vector<fs::path> exclude_directories_;
    std::string file_mask_;
    uint scan_level_ = 0;
    std::shared_ptr<FileReadStrategy> file_reader_ = nullptr;
    hashing::HashType hash_type_ = hashing::HashType::MD5;
};