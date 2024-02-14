#include "database.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "fs_b_tree.hpp"

Database::Database(std::string db_path) {
    this->db_path = db_path;
    this->btree = BTree{};
    b_tree_open(this->btree, db_path);
    int key = 4;
    int data = 53;
    b_tree_insert_record(this->btree, &key, &data);
    key = 5;
    data = 54;
    b_tree_insert_record(this->btree, &key, &data);
    // key = 6;
    // data = 55;
    // b_tree_insert_record(this->btree, &key, &data);
    // key = 7;
    // data = 56;
    // b_tree_insert_record(this->btree, &key, &data);
}

Database::~Database(){
    this->btree.fs.close();
}

std::string Database::process_connection(Connection *con) {
    std::string query = this->extract_query_from_connection(con);
    std::string query_result = this->execute_query(query);
    return query_result;
}

std::string Database::extract_query_from_connection(Connection *con) const {
    bool double_quote = false;
    bool single_quote = false;
    std::string query;
    for (int i = 0; i < con->buffered_data.size(); ++i) {
        if (con->buffered_data.at(i) == (char)34)  // "
            double_quote = single_quote ? double_quote : !double_quote;

        if (con->buffered_data.at(i) == (char)39)  // '
            single_quote = double_quote ? single_quote : !single_quote;

        if (con->buffered_data.at(i) == ';' && !single_quote && !double_quote) {
            query = std::string(con->buffered_data.begin(), con->buffered_data.begin() + i + 1);
            con->buffered_data = std::vector<char>();
            break;
        }
    }
    return query;
}

std::string Database::execute_query(const std::string &query_string) { return query_string; }
