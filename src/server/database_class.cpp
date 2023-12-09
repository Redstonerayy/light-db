#include <iostream>

#include "database_class.hpp"

#include "structs.hpp"

void Database::process_connection(Connection *con){
    std::cout << con->buffered_data.at(0) << "\n";
}
