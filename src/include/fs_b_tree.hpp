#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "structs.hpp"

struct Page {
    std::uint32_t size;
    std::uint32_t fill;
    void* page_data;
    std::uint64_t parent_page;
};

struct BTree {
    std::string filepath;
    std::fstream fs;
    Page* root;
};

void open_b_tree(BTree& btree, std::string filepath);
