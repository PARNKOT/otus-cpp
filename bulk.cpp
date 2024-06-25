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

void Bulk::execute(const commands& cmds) {
    std::cout << "Start executing" << std::endl;

    int block_counter = 0;
    command_t cmd;

    for(int i = 0; true; ++i) {
        if (i >= cmds.size()) {
            break;
        }

        cmd = cmds.at(i);

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

    if constexpr (std::is_same_v<printer_ptr::element_type, AsyncPrinter>)  {
        std::cout << "Run async printer" << std::endl;
        printer->run();
    }
}

void Bulk::notify() {
    for (const auto& observer : observers_)   {
       
        for (const auto& cmd : cmds_) {
            observer->print(cmd);
        }
    }
}

void Bulk::notify(const commands& cmds) {
    int cmds_per_file_printer = (observers_.size() - 1) != 0 ? cmds.size() / (observers_.size() - 1) : 0;
    int pos = 0;
    for (const auto& observer : observers_)   {
        if (observer->is_console_printer()) {
            observer->print(cmds);
            continue;
        }
        
        int start = pos;
        int end =  cmds_per_file_printer == 0 ? cmds.size() : pos + cmds_per_file_printer;
        for (int i = start; i < end; ++i) {
            observer->print(cmds[i]);
        }

        pos = end;
    }
}

// Private

void Bulk::execute_commands() {
    notify(cmds_);
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

void AsyncPrinter::print(const commands& cmds) {
    std::lock_guard<std::mutex> lock(cmds_mutex_);

    for (const auto& cmd : cmds) {
        cmds_.push(cmd);
    }
}

void AsyncPrinter::worker() {
    commands to_print;
    
    while (!need_stop_.load()) {
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

void AsyncPrinter::stop()  {
    while (cmds_.size() != 0) {}
    need_stop_.store(true);
}