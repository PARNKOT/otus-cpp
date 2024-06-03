#pragma once
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


struct FileReadStrategy {
    FileReadStrategy(const std::string& filename) {
        set_filename(filename);
    }

    ~FileReadStrategy() {
        if (file_.is_open()){
            file_.close();
        }
    }

    virtual uint read(std::string& block) = 0;

    void set_filename(const fs::path& filename) {
        if (file_.is_open()) {
            file_.close();
        }

        file_.open(filename);
    }

    std::ifstream& get_file() {
        return file_;
    }

    private:
        std::ifstream file_;
};


struct FileReadBlockStrategy : public  FileReadStrategy {
    FileReadBlockStrategy(const fs::path& filename, uint block_size) : FileReadStrategy(filename), block_size_{block_size} {}

    uint read(std::string& block) override {
        if (!get_file().is_open() ||  block_size_ == 0) {
            return 0;
        }

        char* buf = new char[block_size_];

        get_file().read(buf, block_size_);

        int read_bytes = get_file().gcount();

        if (read_bytes != 0) {
            block.clear();
            block = buf;
        }

        delete[] buf;

        return read_bytes;
    }

    private:
        uint block_size_ = 0;
};