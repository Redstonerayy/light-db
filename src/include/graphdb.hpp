#pragma once

#include "structs.hpp"

#include <vector>

struct Node;

struct Link {
    Node* target;
    void* link_data;
    int link_schema;
};

struct Node {
    void* node_data;
    int node_schema;
    std::vector<Link> links;
};


class Graph {
   public:
    std::vector<std::vector<TABLE_DATATYPE>> node_schemas;
    std::vector<std::vector<TABLE_DATATYPE>> link_schemas;

    std::vector<Node*> nodes;

    Graph();
    void new_node_schema(std::vector<TABLE_DATATYPE> schema);
    void new_link_schema(std::vector<TABLE_DATATYPE> schema);

    void add_node(void* node_data, int node_schema);
    void add_link(Node* origin, Node* target, void* link_data, int link_schema);
    std::vector<Link> query_links(Node* node, int link_schema);

   private:
};