#include "database_constants.hpp"
#include "database.hpp"
#include "server_constants.hpp"
#include "server.hpp"

int main(int argc, char *argv[])
{
    Database* db = new Database(DB_PATH);
    Server* server = new Server(NAME, PORT);
    server->start_server(db);

    return 0;
}
