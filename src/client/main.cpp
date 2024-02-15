#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "client.hpp"
#include "client_constants.hpp"
#include "client_funcs.hpp"

int main(int argc, char *argv[]) {
    /*------------ CLI args ------------*/
    /*------------ authenticate ------------*/  // skip for now, TODO openssl, secret
    Client client("127.0.0.1", PORT);

    std::string query_string = "";
    std::cout << "lightdb>";
    std::cin.exceptions(std::ios_base::failbit);
    while (true) {
        try {
            std::cin.clear();
            std::getline(std::cin, query_string, '\n');
        } catch (std::ios_base::failure &e) {
            std::cout << e.what() << "\n";
        }

        if (check_query_validity(query_string)) {
            printf("Start Query\n");
            // client.Query(query_string);
            // client.Result();
        } else {
            std::cout << "Error in Query String!\n";
        }
        // std::cout << query_string << "\n";
        std::cout << "lightdb>";
    }

    return 0;
}
