#include "btree.hpp"

BTree::BTree(int k_factor) {
    this->k_factor = k_factor;
    this->height = 0;
    this->root_page = new Page(k_factor, nullptr);
}

int BTree::insert(void* key, void* data_pointer){
    return this->root_page->insert(key, data_pointer);
}

Page::Page(int k_factor, Page* parent_page) {
    this->k_factor = k_factor;
    this->parent_page = parent_page;
    this->first_child_page = nullptr;
}

int Page::insert(void* key, void* data_pointer){
    if(this->entries.size() < this->k_factor){
        // binary search insert
    } else if(this->first_child_page == nullptr){}


    if(this->first_child_page == nullptr){
        this->first_child_page = new Page(this->k_factor, this);
    }
}
