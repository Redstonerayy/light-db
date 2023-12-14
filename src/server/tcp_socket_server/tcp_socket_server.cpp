#include <thread>
#include <functional>
#include <string>
#include <vector>

#include "tcp_socket_server.hpp"
#include "util.hpp"
#include "worker.hpp"
#include "listener.hpp"
class Database;

TCP_Socket_Server::TCP_Socket_Server(Database *db, std::string server_port)
{
    this->db = db;
    const unsigned int logical_cpus = util::get_logical_cpus();
    this->create_workers(logical_cpus);
    listener_func(std::ref(*this));
}

void TCP_Socket_Server::create_listener()
{
    this->listener_thread = std::thread(listener_func, std::ref(*this));
}

void TCP_Socket_Server::create_workers(int worker_count)
{
    this->workers = std::vector<std::thread>(worker_count);
    for (int i = 0; i < worker_count; ++i)
    {
        this->workers.emplace_back(std::thread(worker_func, std::ref(*this), std::ref(*this->db)));
    }
}
