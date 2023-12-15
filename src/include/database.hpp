#pragma once

#include <string>

#include "structs.hpp"

class Database
{
public:
    std::string db_path;
    std::string process_connection(Connection *con);
    std::string extract_query_from_connection(Connection *con) const;
    std::string execute_query(const std::string &query_string);

    Database(std::string dp_path) : db_path(dp_path){};
};
