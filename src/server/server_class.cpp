#include <thread>
#include <functional>

#include "server_class.hpp"

#include "util.hpp"
#include "worker.hpp"
#include "listener.hpp"


Server::Server(){}

int Server::start_server()
{
    const unsigned int logical_cpus = util::get_logical_cpus();
    this->create_workers(logical_cpus);
    // this->create_listener();
    listener_func(std::ref(*this));

    return 0;
}

void Server::create_workers(int worker_count)
{
    this->workers = std::vector<std::thread>(worker_count);
    for(int i = 0; i < worker_count; ++i){
        this->workers.emplace_back(std::thread(worker_func, std::ref(*this)));
    }
}

void Server::create_listener()
{
    this->listener_thread = std::thread(listener_func, std::ref(*this));
}
