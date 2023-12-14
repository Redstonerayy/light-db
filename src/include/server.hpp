#pragma once
#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

#include "database.hpp"
#include "tcp_socket_server.hpp"

class Server
{
public:
    std::string server_name;
    std::string server_port;
    Database* db;
    TCP_Socket_Server* tcp_server;

    Server(std::string server_name, std::string server_port);
    int start_server(Database *db);
};

#endif
