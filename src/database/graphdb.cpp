#include "graphdb.hpp"

#include <vector>

Graph::Graph() {}

void Graph::new_node_schema(std::vector<TABLE_DATATYPE> schema) {
    this->node_schemas.push_back(schema);
}
void Graph::new_link_schema(std::vector<TABLE_DATATYPE> schema) {
    this->link_schemas.push_back(schema);
}

void Graph::add_node(void* node_data, int node_schema){
    Node* new_node = new Node{node_data, node_schema,{}};
    this->nodes.emplace_back(new_node);
}

void query_links(Node* node, int link_schema){
    
}