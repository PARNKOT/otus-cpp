#pragma once
#include <iostream>
#include <vector>
#include <span>
#include "bulk.hpp"

using context_t = int;
using buffer_t = bulk::commands;


context_t connect(uint64_t block_size) {

}

void receive(buffer_t* buf, uint64_t size, context_t context_t) {

}

void disconnect(context_t context_t) {
    
}