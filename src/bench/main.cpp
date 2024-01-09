#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
// #include <thread>
#include <cstdio>
#include <limits>

#include "sorted_array.hpp"
#include "structs.hpp"
// #include "graphdb.hpp"
#include "header.hpp"

std::vector<Row> read_csv(std::string filepath);

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
        }

        for (const auto hostility : row.hostilities) {
            void* data = malloc(sizeof(int) * 2);
            *((int*)data) = hostility.first;
            *((int*)data + 1) = hostility.second;
            hostilities->insert(data);
        }
    }

    {
        // find all friends of student no 2
        void* key_left = malloc(sizeof(int) * 2);
        *((int*)key_left) = 2;
        *((int*)key_left + 1) = 0;
        void* key_right = malloc(sizeof(int) * 2);
        *((int*)key_right) = 2;
        *((int*)key_right + 1) = std::numeric_limits<int>::max();

        std::chrono::high_resolution_clock::time_point start_t = std::chrono::high_resolution_clock::now();
        std::vector<void*> friendship_ptrs = friendships->sorted_array->search_between_keys(key_left, key_right);
        std::chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::duration duration = end_t - start_t;
        std::cout << duration.count() << "\n";

        // for (const void* ptr : friendship_ptrs) {
        //     printf("%d %d\n", *((int*)ptr), *((int*)ptr + 1));
        // }
    }
    {
        // find all enemies of student no 1
        void* key_left = malloc(sizeof(int) * 2);
        *((int*)key_left) = 1;
        *((int*)key_left + 1) = 0;
        void* key_right = malloc(sizeof(int) * 2);
        *((int*)key_right) = 1;
        *((int*)key_right + 1) = std::numeric_limits<int>::max();

        std::vector<void*> hostilities_ptrs = hostilities->sorted_array->search_between_keys(key_left, key_right);
        
        // for (const void* ptr : hostilities_ptrs) {
        //     printf("%d %d\n", *((int*)ptr), *((int*)ptr + 1));
        // }
    }

    // std::this_thread::sleep_for(std::chrono::seconds(10));
}
