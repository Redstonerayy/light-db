#pragma once

#include <string>

#include "structs.hpp"
#include "fs_b_tree.hpp"

class Database
{
public:
    std::string db_path;
    BTree btree;
    std::string process_connection(Connection *con);
    std::string extract_query_from_connection(Connection *con) const;
    std::string execute_query(const std::string &query_string);

    Database(std::string db_path);
    ~Database();
};
