#include "util.hpp"
#include "server_class.hpp"

int Server::start_server(){
    const unsigned int logical_cpus = util::get_logical_cpus();
    this->create_workers(logical_cpus);
    this->create_listener();
}
