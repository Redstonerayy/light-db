#pragma once
#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <thread>
#include <vector>
#include <mutex>
#include <queue>

class Server
{
public:
    int start_server();

private:
    std::queue<int> incoming_connections_fds;
    std::mutex incoming_connections_fds_mutex;

    std::vector<std::thread> workers;
    bool create_workers(int worker_count);

    std::thread listener_thread;
    bool create_listener();
};

#endif
