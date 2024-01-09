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

std::vector<Link> Graph::query_links(Node* node, int link_schema){
    std::vector<Link> matching_links;
    for(const auto link : node->links){
        if (link.link_schema == link_schema){
            matching_links.push_back(link);
        }
    }
    return matching_links;
}

void Graph::add_link(Node* origin, Node* target, void* link_data, int link_schema){
    if(link_schema == -1) link_data = nullptr;
    origin->links.emplace_back(Link{target, link_data, link_schema});
}
