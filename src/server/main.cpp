#include "server_class.hpp"

int main(int argc, char *argv[])
{
    /*------------ TODO cli arguments ------------*/
    Server db_server = Server();
    db_server.start_server();

    return 0;
}
