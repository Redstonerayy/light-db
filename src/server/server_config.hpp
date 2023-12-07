#pragma once

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>

const std::string NAME = "lightdb";
const std::string PORT = "4466";
const int BACKLOG = 50;
const int WORKER_EPOLL_MAX = 1000;

#endif
