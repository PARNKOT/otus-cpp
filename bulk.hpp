#pragma once
#include <queue>
#include <string>

namespace bulk {
    using command_t = std::string;

    struct Bulk {
        explicit Bulk(int block_size) : block_size_(block_size) {}
        void execute();

    private:
        void execute_commands();

        std::queue<command_t> cmd_queue_;
        int block_size_;
    };
}