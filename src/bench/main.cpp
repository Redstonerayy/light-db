#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include "structs.hpp"
#include "binary_tree.hpp"
#include "graphdb.hpp"
#include "header.hpp"

std::vector<Row> read_csv(std::string filepath);
// void print_binary_tree_node(BT_Node* node, std::vector<int>& key_attribute_lengths);
// void print_binary_tree(Binary_Tree* binary_tree, std::vector<int>& key_attribute_lengths);

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
            friendships->insert(data);
        }

        for (const auto hostility : row.hostilities) {
            void* data = malloc(sizeof(int) * 2);
            *((int*)data) = hostility.first;
            *((int*)data + 1) = hostility.second;
            friendships->insert(data);
        }
    }

    int search = 26;
    std::cout << students->search((void*)&search) << "\n";
}
