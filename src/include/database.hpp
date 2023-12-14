#pragma once
#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>

#include "structs.hpp"

class Database
{
public:
    std::string db_path;
    std::string process_connection(Connection *con);
    std::string extract_query_from_connection(Connection *con);
    std::string execute_query(std::string query_string);

    Database(std::string dp_path);
};

#endif
