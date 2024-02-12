#pragma once

#include <cstdint>
#include <fstream>
#include <string>

#include "structs.hpp"

struct Page {
    std::uint32_t size;
    std::uint32_t fill;
    std::uint64_t parent_page;
    void* page_data;
};

struct BTree {
    std::string filepath;
    std::fstream fs;
    std::vector<int> key_ids;
    std::vector<int> data_ids;
    int key_size;
    int data_size;
    int page_size;
    int page_offset;
};

void b_tree_open(BTree& btree, std::string filepath);
bool b_tree_insert_record(BTree &btree, void* key, void* data);
