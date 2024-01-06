#pragma once

#include <vector>

#include "structs.hpp"

struct BT_Node {
    void* key;
    void* data;
    short balance;
    BT_Node* parent;
    BT_Node* left;
    BT_Node* right;
};

class Binary_Tree {
   public:
    BT_Node* root_node;

    std::vector<int> key_attribute_offsets;
    std::vector<int> key_attribute_lengths;

    Binary_Tree(std::vector<int> key_attribute_offsets, std::vector<int> key_attribute_lengths);
    int insert(void* data);
    void* search(void* key);
    int remove(void* key);

   private:
    void* compute_key(void* data);
    BT_Node* search_for_key(void* key);
    BT_Node* find_inorder_successor(BT_Node* node);
    void rebalance(BT_Node* bt_node);
};

class Table {
   public:
    Binary_Tree* binary_tree;

    std::vector<int> key_attribute_offsets;
    std::vector<int> key_attribute_lengths;
    std::vector<TABLE_DATATYPE> schema;

    Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes);
    int insert(void* data);
    void* search(void* key);
    int remove(void* key);

   private:
    void compute_key_data(std::vector<bool> key_attributes);
};
