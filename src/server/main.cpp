#include "database_class.hpp"
#include "server_config.hpp"
#include "server_class.hpp"

int main(int argc, char *argv[])
{
    // TODO cli arguments
    Database* db = new Database;
    Server* server = new Server(NAME);
    server->start_server(db);

    return 0;
}
