#include <cmath>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

#include "binary_tree.hpp"

void print_binary_tree_node(BT_Node* node, std::vector<int>& key_attribute_lengths) {
    int byte_offset = 0;

    for (const int& bytes : key_attribute_lengths) {
        if (bytes <= 8) {
            long integer;
            if (bytes == 4) {
                integer = *((int*)((char*)node->key + byte_offset));
            } else if (bytes == 8) {
                integer = *((long*)((char*)node->key + byte_offset));
            }
            std::cout << integer << " ";
        } else {
            char* begin = (char*)node->key + byte_offset;
            for (int i = 0; i < bytes; ++i) {
                std::cout << begin[i] << " ";
            }
        }
        byte_offset += bytes;
    }
    std::cout << "/ " << node->balance << "\n";
}

void print_binary_tree(Binary_Tree* binary_tree, std::vector<int>& key_attribute_lengths) {
    std::deque<std::pair<BT_Node*, int>> print_q;
    print_q.push_back({binary_tree->root_node, 0});

    int last_level = -1;
    int w = 1000;

    while (!print_q.empty()) {
        std::pair<BT_Node*, int> current_node = print_q.front();
        print_q.pop_front();

        if (current_node.first == nullptr) {
            print_q.push_back({nullptr, current_node.second + 1});
            print_q.push_back({nullptr, current_node.second + 1});
        } else {
            if (current_node.first->left != nullptr) {
                print_q.push_back({current_node.first->left, current_node.second + 1});
            } else {
                print_q.push_back({nullptr, current_node.second + 1});
            }
            if (current_node.first->right != nullptr) {
                print_q.push_back({current_node.first->right, current_node.second + 1});
            } else {
                print_q.push_back({nullptr, current_node.second + 1});
            }
        }

        if (last_level < current_node.second) {
            bool exit = true;
            for (const auto& p : print_q) {
                if (p.first != nullptr) exit = false;
            }
            if (exit) {
                std::cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
                return;
            }
            std::cout << "\n";
            std::cout << "LV " << current_node.second << " : ";
            last_level = current_node.second;
        }

        int el_count = std::pow(2, current_node.second);
        int offset = w / el_count;

        for (int i = 0; i < offset / 2; ++i) {
            std::cout << " ";
        }

        if (current_node.first == nullptr) {
            std::cout << "--/--";
        } else {
            void* key = current_node.first->key;
            int byte_offset = 0;

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
            std::cout << "/ " << current_node.first->balance;
        }

        for (int i = 0; i < offset / 2; ++i) {
            std::cout << " ";
        }
    }
    std::cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
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