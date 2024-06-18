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
    for (const auto& obs : observers_) {
        obs->run();
    }

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
        // TODO: Реализовать распределение команд по принтерам
        
        for (const auto& cmd : cmds_) {
            observer->print(cmd);
        }
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


void AsyncPrinter::print(const command_t& cmd) {
    std::lock_guard<std::mutex> lock(cmds_mutex_);

    cmds_.push(cmd);
}

void AsyncPrinter::worker() {
    commands to_print;
    
    while (true) {
        if (cmds_.size() == 0) continue;
        
        {
            std::lock_guard<std::mutex> lock(cmds_mutex_);

            while (!cmds_.empty()) {
                to_print.push_back(cmds_.front());
                cmds_.pop();
            }
        }

        printer_->print(to_print);
        to_print.clear();
    }
}

void AsyncPrinter::run() {
    if  (printer_ == nullptr) {
        throw std::runtime_error("NULL printer");
    }

    if (f_.valid()) return;

    f_ = std::async(std::launch::async, &AsyncPrinter::worker, this);
}