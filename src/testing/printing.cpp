#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include <cmath>

#include "binary_tree.hpp"

void print_binary_tree(Binary_Tree* binary_tree, std::vector<int>& key_attribute_lengths){
    std::queue<std::pair<BT_Node*, int>> print_q;
    print_q.push({binary_tree->root_node, 0});

    int last_level = -1;
    int w = 201;

    while (!print_q.empty()) {
        std::pair<BT_Node*, int> current_node = print_q.front();
        print_q.pop();
        if (current_node.first->left != nullptr) {
            print_q.push({current_node.first->left, current_node.second + 1});
        }
        if (current_node.first->right != nullptr) {
            print_q.push({current_node.first->right, current_node.second + 1});
        }
        if(last_level < current_node.second){
            std::cout << "\n";
            std::cout << "LV " << current_node.second << " : ";
            last_level = current_node.second;
        }
        int el_count = std::pow(2, current_node.second);
        int offset = w / el_count;

        void* key = current_node.first->key;
        int byte_offset = 0;
        for(int i = 0; i < offset / 2; ++i){
            std::cout << " ";
        }
        for (const int& bytes : key_attribute_lengths) {
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
        for(int i = 0; i < offset / 2; ++i){
            std::cout << " ";
        }
    }
    std::cout << "\n---------------\n";
}

// void print_b_tree(BTree* btree, std::vector<int>& key_bytes) {
//     std::queue<std::pair<Page*, int>> print_q;
//     print_q.push({btree->root_page, 0});

//     while (!print_q.empty()) {
//         std::pair<Page*, int> current_page = print_q.front();
//         print_q.pop();
//         if (current_page.first->first_child_page != nullptr) {
//             print_q.push({current_page.first->first_child_page, current_page.second + 1});
//         }
//         std::cout << "LV: " << current_page.second << " ";
//         for (const auto page_entry : current_page.first->entries) {
//             void* key = page_entry.key;
//             int byte_offset = 0;
//             for (const int& bytes : key_bytes) {
//                 if (bytes <= 8) {
//                     long integer;
//                     if (bytes == 4) {
//                         integer = *((int*)((char*)key + byte_offset));
//                     } else if (bytes == 8) {
//                         integer = *((long*)((char*)key + byte_offset));
//                     }
//                     std::cout << integer << " ";
//                 } else {
//                     char* begin = (char*)key + byte_offset;
//                     for (int i = 0; i < bytes; ++i) {
//                         std::cout << begin[i] << " ";
//                     }
//                 }
//                 byte_offset += bytes;
//             }
//             std::cout << "-- ";
//             if (page_entry.child_page_pointer != nullptr) print_q.push({page_entry.child_page_pointer, current_page.second + 1});
//         }
//         std::cout << "\n";
//     }
//     std::cout << "---------------\n";
// }