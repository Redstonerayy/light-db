#pragma once

#include <fstream>
#include <string>
#include <vector>

struct Page {
    int size;
    int fill;
    long parent_page;
    void* page_data;
};

struct BTree {
    std::string filepath;
    std::fstream fs;
    std::vector<int> key_ids;
    std::vector<int> data_ids;
    int page_k;
    int key_size;
    int data_size;
    int page_size;
    int page_data_size;
    int page_offset;
};

void b_tree_open(BTree& btree, std::string filepath);
bool b_tree_insert_record(BTree &btree, void* key, void* data);
