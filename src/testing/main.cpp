#include <iostream>
#include <cstdlib>
#include <vector>

#include "binary_tree.hpp"

void print_binary_tree(Binary_Tree* binary_tree, std::vector<int>& key_attribute_lengths);

int main() {
    Table* table = new Table({INT, INT, CHAR_8}, {true});
    // int keys[] = {4, 6, 5, 2, 3, 7, 1, 8, 9, 10, 0};
    int keys[] = {1, 2, 3, 4, 5, 6, 7};//, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    for(int i = 0; i < sizeof(keys) / sizeof(int); ++i){
        void* data = malloc(sizeof(char) * 16);
        *((int*)data) = keys[i];
        table->insert(data);
        print_binary_tree(table->binary_tree, table->key_attribute_lengths);
    }
    // int search = 19;
    // int del = 15;
    // std::cout << *(int*)table->search((void*)&search) << "\n";
    // std::cout << table->remove((void*)&del) << "\n";
    // print_binary_tree(table->binary_tree, table->key_attribute_lengths);
}
