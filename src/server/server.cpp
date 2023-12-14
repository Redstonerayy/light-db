#include <string>

#include "server.hpp"
#include "tcp_socket_server.hpp"
class Database;

Server::Server(std::string server_name, std::string server_port)
{
    this->server_name = server_name;
    this->server_port = server_port;
}

int Server::start_server(Database *db)
{
    this->db = db;
    this->tcp_server = new TCP_Socket_Server(db, this->server_port);

    return 0;
}
