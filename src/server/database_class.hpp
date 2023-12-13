#pragma once
#ifndef DATABASE_CLASS_HPP
#define DATABASE_CLASS_HPP

#include <string>

#include "structs.hpp"

class Database
{
public:
    std::string process_connection(Connection *con);

private:
    std::string extract_query_from_connection(Connection *con);
    std::string execute_query(std::string query_string);
};

#endif
