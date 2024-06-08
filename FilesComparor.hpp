#pragma once
#include <filesystem>
#include <vector>
#include <map>
#include "Hashing.hpp"
#include "FileReader.hpp"
#include "HashedFile.hpp"
#include "constants.hpp"


namespace fs = std::filesystem;

using path_t = std::string;


struct FilesComparor {
    using group_t = std::vector<path_t>;
    using compare_result_t = std::vector<group_t>;

    FilesComparor(const std::vector<path_t>& directories) {
        set_scan_directories(directories);
        file_reader_ = std::make_shared<FileReaderBlock>(block_size_default);
    }

    FilesComparor(const std::vector<path_t>& directories, std::shared_ptr<FileReader> file_reader) : file_reader_{file_reader}  {
        set_scan_directories(directories);
    }

    void set_hash_type(hashing::HashType hash_type) {
        hash_type_ = hash_type;
    }

    void set_file_reader(std::shared_ptr<FileReader> file_reader)  {
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

    bool validate_entry(const fs::directory_entry& entry) {
        if (entry.is_directory()) {
            return false;
        }

        if (entry.file_size() == 0) {
            return false;
        }

        return true;
    }

    std::vector<fs::path> iterate_directory(const fs::path& dir) {
        std::vector<fs::path> files;

        if (scan_level_ == 1)  {
            for (const auto& entry : fs::recursive_directory_iterator(dir)) {
                if (!validate_entry(entry)) {
                    continue;
                }

                files.push_back(entry.path());
            }
        } else {
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (!validate_entry(entry)) {
                    continue;
                }

                files.push_back(entry.path());
            }
        }

        return files;
    }

    compare_result_t run() {
        std::vector<HashedFile> files;
        files.reserve(50);

        std::for_each(scan_directories_.cbegin(), scan_directories_.cend(), [&](const auto& dir){
            if (!fs::exists(dir)) {
                return;
            }

            auto dir_files = iterate_directory(dir);
            for (const auto& file : dir_files) {
                files.push_back(HashedFile(file, hash_type_, std::make_shared<FileReaderBlock>(5))); // TODO: add block_size argument!
            }
        });

        // std::for_each(files.cbegin(), files.cend(), [](const auto& file) {
        //     std::cout << file << std::endl;
        // });

        files[0] == files[1];

        std::map<fs::path, int> path_to_group_map;

        for (int i = 0; i < files.size(); ++i) {
            for (int k = 0; k < files.size(); ++k) {

            }
        }

        return {
            {"a1", "a2", "a3"},
            {"b1", "b2", "b3"},
            {"c1", "c2", "c3"}
        };
    }

private:
    void set_scan_directories(const std::vector<path_t>& directories) {
        scan_directories_.reserve(directories.size());

        std::for_each(directories.cbegin(), directories.cend(), [&](const auto& dir) {
            scan_directories_.emplace_back(dir);
        });
    }

    std::vector<fs::path> scan_directories_;
    std::vector<fs::path> exclude_directories_;
    std::string file_mask_;
    uint scan_level_ = 0;
    std::shared_ptr<FileReader> file_reader_ = nullptr;
    hashing::HashType hash_type_ = hashing::HashType::MD5;
};