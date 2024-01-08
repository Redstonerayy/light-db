#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
// #include <chrono>
// #include <thread>
#include <cstdio>
#include <limits>

#include "structs.hpp"
#include "binary_tree.hpp"
// #include "graphdb.hpp"
#include "header.hpp"

std::vector<Row> read_csv(std::string filepath);
void print_binary_tree_node(BT_Node* node, std::vector<int>& key_attribute_lengths);
void print_binary_tree(Binary_Tree* binary_tree, std::vector<int>& key_attribute_lengths);

int main() {
    Table* students;
    Table* friendships;
    Table* hostilities;

    students = new Table({INT, CHAR_16}, {true});
    friendships = new Table({INT, INT}, {true, true});
    hostilities = new Table({INT, INT}, {true, true});

    std::vector<Row> rows = read_csv("./run/records.csv");

    for (const auto row : rows) {
        void* data = malloc(sizeof(int) + sizeof(char) * 16);
        *((int*)data) = row.id;
        memcpy(((char*)data + 4), row.name.data(), 16);
        students->insert(data);

        for (const auto friendship : row.friendships) {
            void* data = malloc(sizeof(int) * 2);
            *((int*)data) = friendship.first;
            *((int*)data + 1) = friendship.second;
            int insert_status = friendships->insert(data);
            std::cout << friendship.first << ":" << friendship.second << "//\n";
            if(row.id == 2){
                void* key_eleven = malloc(sizeof(int) * 2);
                *((int*)key_eleven) = 2;
                *((int*)key_eleven + 1) = 11;
                std::cout << friendship.second << "-" << friendships->search(key_eleven) << "\n";
                // print_binary_tree(friendships->binary_tree, friendships->binary_tree->key_attribute_lengths);
            }
            // std::cout << friendship.first << ":" << friendship.second << ":" << insert_status << "\n";
        }

        for (const auto hostility : row.hostilities) {
            void* data = malloc(sizeof(int) * 2);
            *((int*)data) = hostility.first;
            *((int*)data + 1) = hostility.second;
            hostilities->insert(data);
        }
        // std::cout << "---xxx---\n";
    }

    std::cout << "TREES\n";
    // print_binary_tree(students->binary_tree, students->binary_tree->key_attribute_lengths);
    // print_binary_tree(friendships->binary_tree, friendships->binary_tree->key_attribute_lengths);
    // print_binary_tree(hostilities->binary_tree, hostilities->binary_tree->key_attribute_lengths);

    // find all friends of student no 2
    void* key_left = malloc(sizeof(int) * 2);
    *((int*)key_left) = 2;
    *((int*)key_left + 1) = 0;
    void* key_right = malloc(sizeof(int) * 2);
    *((int*)key_right) = 2;
    *((int*)key_right + 1) = std::numeric_limits<int>::max();
    std::vector<void*> friendship_ptrs = friendships->binary_tree->search_between_keys(key_left, key_right);
    std::cout << friendship_ptrs.size() << "\n";
    for(const void* ptr : friendship_ptrs){
        printf("%d %d\n", *((int*)ptr), *((int*)ptr + 1));
    }
    // find all enemies of student no 1



    // std::this_thread::sleep_for(std::chrono::seconds(10));
}
