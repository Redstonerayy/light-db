#include "database_class.hpp"
#include "server_config.hpp"
#include "server_class.hpp"

int main(int argc, char *argv[])
{
    // TODO cli arguments
    Database db = Database();
    Server server(NAME);
    server.start_server(&db);

    return 0;
}
