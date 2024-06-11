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
    }

    void set_hash_type(hashing::HashType hash_type) {
        hash_type_ = hash_type;
    }

    void add_exclude_directories(const std::vector<path_t>& exclude) {
        std::for_each(exclude.cbegin(), exclude.cend(), [&](const auto& dir) {
            exclude_directories_.emplace_back(dir);
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

        if (std::find(exclude_directories_.begin(), exclude_directories_.end(), entry.path()) != std::end(exclude_directories_)) {
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

        std::map<fs::path, int> path_to_group_map;

        int group_counter = 0;
        for (int i = 0; i < files.size(); ++i) {
            auto& first = files[i];
            for (int k = i + 1; k < files.size(); ++k) {
                auto& second = files[k];
                if (first != second) {
                    continue;
                }

                if (path_to_group_map.count(first.path()) == 0) {
                    path_to_group_map[first.path()] = group_counter;
                    ++group_counter;
                }

                path_to_group_map[second.path()] = path_to_group_map[first.path()];
            }
        }

        compare_result_t out(group_counter);

        std::for_each(path_to_group_map.cbegin(), path_to_group_map.cend(), [&](const auto& pair) {
            out[pair.second].push_back(fs::absolute(pair.first));
        });

        return out;
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
    hashing::HashType hash_type_ = hashing::HashType::MD5;
};