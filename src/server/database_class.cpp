#include <iostream>
#include <string>

#include "database_class.hpp"

#include "structs.hpp"

void *Database::process_connection(Connection *con)
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
            con->buffered_data = std::vector<char>(con->buffered_data.begin() + i + 1, con->buffered_data.end());
            break;
        }
    }
    std::cout << query << "\n";
    // con->buffered_data.push_back(0);
    // std::cout << con->buffered_data.data() << "\n";
    return 0;
}
