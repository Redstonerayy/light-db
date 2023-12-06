#include "server_config.hpp"
#include "server_class.hpp"

int main(int argc, char *argv[])
{
    // TODO cli arguments
    Server server(NAME);
    server.start_server();

    return 0;
}
