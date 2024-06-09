#pragma once
#include <filesystem>
#include <memory>
#include "FileReader.hpp"
#include "Hashing.hpp"

namespace fs = std::filesystem;

struct HashedFile {
    HashedFile(const fs::path& file, hashing::HashType hash_type, std::shared_ptr<FileReader> file_reader) : file_{file}, hash_type_{hash_type}, file_reader_{file_reader} {
        file_reader_->set_filename(file);
    }

    const fs::path& path() const {
        return file_;
    }

    bool operator==(HashedFile& other) {
        if (is_hash_calculated_ == true && other.is_hash_calculated_ == true && hash_.size() != other.hash_.size()) {
            return false;
        }

        for (int i = 0;; ++i) {
            if (i >= hash_.size()) {
                if (is_hash_calculated_) {
                    break;
                }
                calculate_hash_once();
            }

            if (i >= other.hash_.size()) {
                if (other.is_hash_calculated_) {
                    break;
                }
                other.calculate_hash_once();
            }

            if (hash_[i] != other.hash_[i]) {
                return false;
            }
        }

        return (is_hash_calculated_ && other.is_hash_calculated_) ? true : false;
    }

    bool operator!=(HashedFile& other) {
        return !(*this == other);
    }

private:
    void calculate_hash_once() {
        if (is_hash_calculated_) {
            return;
        }

        std::string block;
        if (file_reader_->read(block) != 0) {
            hash_ += hashing::calculate(block, hash_type_);
        }

        if (file_reader_->get_file().eof()) {
            is_hash_calculated_ = true;
        }
    }

    fs::path file_;
    std::shared_ptr<FileReader> file_reader_ =  nullptr;
    hashing::hash_t hash_;
    hashing::HashType hash_type_ = hashing::HashType::MD5;
    bool is_hash_calculated_ = false;
};
