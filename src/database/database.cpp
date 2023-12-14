#include <string>
#include <vector>

#include "database.hpp"

#include "structs.hpp"

Database::Database(std::string db_path)
{
    this->db_path = db_path;
}

std::string Database::process_connection(Connection *con)
{
    std::string query = this->extract_query_from_connection(con);
    std::string query_result = this->execute_query(query);
    return query_result;
}

std::string Database::extract_query_from_connection(Connection *con)
{
    bool double_quote = false;
    bool single_quote = false;
    std::string query;
    for (int i = 0; i < con->buffered_data.size(); ++i)
    {
        if (con->buffered_data.at(i) == (char)34) // "
            double_quote = single_quote ? double_quote : !double_quote;

        if (con->buffered_data.at(i) == (char)39) // '
            single_quote = double_quote ? single_quote : !single_quote;

        if (con->buffered_data.at(i) == ';' && !single_quote && !double_quote)
        {
            query = std::string(con->buffered_data.begin(), con->buffered_data.begin() + i + 1);
            con->buffered_data = std::vector<char>(65536);
            break;
        }
    }
    return query;
}

std::string Database::execute_query(std::string query_string)
{
    return query_string;
}