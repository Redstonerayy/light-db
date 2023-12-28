#include <string>
#include <iostream>

#include "client.hpp"
#include "client_constants.hpp"
#include "client_funcs.hpp"

int main(int argc, char *argv[]) {
    /*------------ CLI args ------------*/
    /*------------ authenticate ------------*/  // skip for now, TODO openssl, secret
    Client client("127.0.0.1", PORT);

    std::string query_string;
    std::cout << "lightdb>";
    while (true) {
        if(!getline(std::cin, query_string, '\n'))
            continue;
        
        if (check_query_validity(query_string)) {
            client.Query(query_string);
            client.Result();
        } else {
            std::cout << "Error in Query String!\n";
        }
        std::cout << "lightdb>";
    }

    return 0;
}
