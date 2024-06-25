#pragma once
#include <iostream>
#include <vector>
//#include <span>
#include "bulk.hpp"

using context_t = std::unique_ptr<bulk::Bulk>;
using buffer_t = bulk::commands;


context_t connect(uint64_t block_size) {
    auto context = std::make_unique<bulk::Bulk>(block_size);

    auto console_printer = std::make_shared<bulk::ConsolePrinter>();
    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    auto filename1 = "file1_" + std::to_string(now) + ".txt";
    auto filename2 = "file2_" + std::to_string(now) + ".txt";
    auto file1_printer = std::make_shared<bulk::FilePrinter>(std::filesystem::path(filename1));
    auto file2_printer = std::make_shared<bulk::FilePrinter>(std::filesystem::path(filename2));

    context->add_printer(std::make_shared<bulk::AsyncPrinter>(console_printer));
    context->add_printer(std::make_shared<bulk::AsyncPrinter>(file1_printer));
    context->add_printer(std::make_shared<bulk::AsyncPrinter>(file2_printer));

    return context;
}

void receive(buffer_t* buf, uint64_t size, const context_t& context) {
    context->execute(*buf);
}

void disconnect(context_t context) {
    context = nullptr;
}