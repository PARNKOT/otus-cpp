#pragma once
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

enum class ReadStrategy {
    BlockRead,
};

struct FileReader {
    ~FileReader() {
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

    const std::ifstream& get_file() const {
        return file_;
    }

protected:
    std::ifstream file_;
};


struct FileReaderBlock : public  FileReader {
    FileReaderBlock(uint block_size) : block_size_{block_size} {}

    uint read(std::string& block) override {
        if (!file_.is_open() ||  block_size_ == 0) {
            return 0;
        }

        char* buf = new char[block_size_];
        std::memset(buf, '\0', block_size_);

        file_.read(buf, block_size_);

        int read_bytes = file_.gcount();

        if (read_bytes != 0) {
            block.clear();
            for (int i = 0; i < block_size_; ++i) {
                block.push_back(buf[i]);
            }
        }

        delete[] buf;

        return read_bytes;
    }

    private:
        uint block_size_ = 0;
};
