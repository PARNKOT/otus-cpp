#include <iostream>
#include <fstream>
#include "bulk.hpp"

using namespace bulk;

#define CMD_START_BLOCK "{"
#define CMD_END_BLOCK "}"

/*
##########
#  Bulk  #
##########
*/

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

        cmds_.push_back(cmd);

        if (cmds_.size() == block_size_ && block_counter == 0) {
            execute_commands();
        }
    }

    if (block_counter == 0) {
        execute_commands();
    }
}

void Bulk::add_printer(printer_ptr printer)  {
    observers_.push_back(printer);
}

void Bulk::notify() {
    for (const auto& observer : observers_)   {
        observer->print(cmds_);
    }
}

// Private

void Bulk::execute_commands() {
    notify();
    cmds_.clear();
}


/*
##############
#  Printers  #
##############
*/

void ConsolePrinter::print(const commands& cmds) {
    print_(std::cout, cmds);
}

void FilePrinter::print(const commands& cmds) {
    std::ofstream file(path_, std::ios::app);

    if (file.is_open()) {
        print_(file, cmds);
    }
}
