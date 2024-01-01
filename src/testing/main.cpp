#include <iostream>

#include "btree.hpp"

void print_b_tree(BTree* btree, std::vector<int>& key_bytes);

int main() {
    Table* table = new Table({INT, INT, CHAR_8}, {true});
    int keys[] = {4, 6, 5, 2, 3, 7, 1, 8, 9, 10, 0};
    for(int i = 0; i < sizeof(keys) / sizeof(int); ++i){
        void* data = malloc(sizeof(char) * 16);
        *((int*)data) = keys[i];
        table->insert(data);
        print_b_tree(table->btree, table->key_bytes);
    }
}
