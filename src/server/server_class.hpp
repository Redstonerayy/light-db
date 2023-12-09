#pragma once
#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

#include "database_class.hpp"

class Server
{
public:
    std::queue<int> incoming_connections;
    std::mutex incoming_connections_m;
    std::condition_variable incoming_connections_cv;

    std::string server_name;
    Database* db;

    Server(std::string server_name);
    int start_server(Database *db);

private:
    std::vector<std::thread> workers;
    void create_workers(int worker_count);

    std::thread listener_thread;
    void create_listener();
};

#endif
