#pragma once
#ifndef TCP_SOCKET_SERVER_HPP
#define TCP_SOCKET_SERVER_HPP

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

#include "database.hpp"

class TCP_Socket_Server
{
public:
    Database *db;

    std::queue<int> incoming_connections;
    std::mutex incoming_connections_m;
    std::condition_variable incoming_connections_cv;
    
    std::thread listener_thread;
    void create_listener();
    std::vector<std::thread> workers;
    void create_workers(int worker_count);

    TCP_Socket_Server(Database *db, std::string server_port);
};

#endif
