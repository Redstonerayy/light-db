#include <iostream>
#include <queue>
#include <utility>

#include "btree.hpp"

void print_b_tree(BTree* btree, std::vector<int>& key_bytes) {
    std::queue<std::pair<Page*, int>> print_q;
    print_q.push({btree->root_page, 0});

    while (!print_q.empty()) {
        std::pair<Page*, int> current_page = print_q.front();
        print_q.pop();
        if (current_page.first->first_child_page != nullptr) {
            print_q.push({current_page.first->first_child_page, current_page.second + 1});
        }
        std::cout << "LV: " << current_page.second << " ";
        for (const auto page_entry : current_page.first->entries) {
            void* key = page_entry.key;
            int byte_offset = 0;
            for (const int& bytes : key_bytes) {
                if (bytes <= 8) {
                    long integer;
                    if (bytes == 4) {
                        integer = *((int*)((char*)key + byte_offset));
                    } else if (bytes == 8) {
                        integer = *((long*)((char*)key + byte_offset));
                    }
                    std::cout << integer << " ";
                } else {
                    char* begin = (char*)key + byte_offset;
                    for (int i = 0; i < bytes; ++i) {
                        std::cout << begin[i] << " ";
                    }
                }
                byte_offset += bytes;
            }
            std::cout << "-- ";
            if (page_entry.child_page_pointer != nullptr) print_q.push({page_entry.child_page_pointer, current_page.second + 1});
        }
        std::cout << "\n";
    }
    std::cout << "---------------\n";
}