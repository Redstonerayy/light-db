#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "graphdb.hpp"
#include "header.hpp"
#include "sorted_array.hpp"
#include "structs.hpp"
#include "timer.hpp"

std::vector<Row> read_csv(std::string filepath);

int main() {
    std::vector<Row> rows = read_csv("./run/records.csv");

    {
        Table* students;
        Table* friendships;
        Table* hostilities;

        students = new Table({INT, CHAR_16}, {true});
        friendships = new Table({INT, INT}, {true, true});
        hostilities = new Table({INT, INT}, {true, true});

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

            for (int j = 0; j < 10; ++j) {
                Timer::start("sorted_array_friends");
                std::vector<void*> friendship_ptrs = friendships->sorted_array->search_between_keys(key_left, key_right);
                Timer::stop("sorted_array_friends");
            }

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

            for (int j = 0; j < 10; ++j) {
                Timer::start("sorted_array_hostilities");
                std::vector<void*> hostilities_ptrs = hostilities->sorted_array->search_between_keys(key_left, key_right);
                Timer::stop("sorted_array_hostilities");
            }
            // for (const void* ptr : hostilities_ptrs) {
            //     printf("%d %d\n", *((int*)ptr), *((int*)ptr + 1));
            // }
        }
    }

    {
        Graph* graph = new Graph();

        graph->new_node_schema({INT, CHAR_16});
        graph->new_link_schema({});
        graph->new_link_schema({});

        for (const auto row : rows) {
            void* data = malloc(sizeof(int) + sizeof(char) * 16);
            *((int*)data) = row.id;
            memcpy(((char*)data + 4), row.name.data(), 16);

            graph->add_node(data, 0);
        }

        for (const auto row : rows) {
            for (const auto friendship : row.friendships) {
                graph->add_link(graph->nodes.at(row.id), graph->nodes.at(friendship.second), nullptr, 0);
            }

            for (const auto hostility : row.hostilities) {
                graph->add_link(graph->nodes.at(row.id), graph->nodes.at(hostility.second), nullptr, 1);
            }
        }

        // find all friends of student no 2
        for (int j = 0; j < 10; ++j) {
            Timer::start("graph_friends");
            std::vector<Link> friends = graph->query_links(graph->nodes.at(2), 0);
            Timer::stop("graph_friends");
        }
        // for(const auto f : friends){
        //     printf("%d %d\n", 2, *((int*)f.target->node_data));
        // }

        // find all enemies of student no 1
        for (int j = 0; j < 10; ++j) {
            Timer::start("graph_hostilities");
            std::vector<Link> enemies = graph->query_links(graph->nodes.at(1), 1);
            Timer::stop("graph_hostilities");
        }
        // for(const auto e : enemies){
        //     printf("%d %d\n", 1, *((int*)e.target->node_data));
        // }
    }

    // std::this_thread::sleep_for(std::chrono::seconds(10));
}
