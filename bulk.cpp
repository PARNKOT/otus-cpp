#include <iostream>
#include "bulk.hpp"

using namespace bulk;

#define CMD_START_BLOCK "{"
#define CMD_END_BLOCK "}"

// Public

void Bulk::execute() {
    int block_counter = 0;
    command_t cmd;

    while(true) {
        std::cin >> cmd;

        if (std::cin.eof()) {
            break;
        }

        if (cmd == CMD_START_BLOCK) {
            if (block_counter == 0) {
                execute_commands();
            }
            ++block_counter;
            continue;
        }

        if (cmd == CMD_END_BLOCK) {
            if (block_counter == 0) {
                continue;
            }
            
            if (--block_counter == 0) {
                execute_commands();
            }

            continue;
        }

        cmd_queue_.push(cmd);

        if (cmd_queue_.size() == block_size_ && block_counter == 0) {
            execute_commands();
        }
    }

    if (block_counter == 0) {
        execute_commands();
    }
}


// Private

void Bulk::execute_commands() {
    if (cmd_queue_.size() == 0) {
        return;
    }

    std::cout << "bulk: ";
    while (cmd_queue_.size() > 0) {
        std::cout << cmd_queue_.front() << ", ";
        cmd_queue_.pop();
    }

    std::cout << "\b\b " << std::endl;
}