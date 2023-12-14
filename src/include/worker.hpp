#pragma once
#ifndef WORKER_HPP
#define WORKER_HPP

#include "tcp_socket_server.hpp"

void worker_func(TCP_Socket_Server &server, Database &db);

#endif
