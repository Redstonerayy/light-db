#pragma once

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

const std::string PORT = "4466";
const int BACKLOG = 20;

struct addrinfo *get_local_address();
int make_listening_socket(struct addrinfo *serverinfo);
void accept_connections(int sockfd);

#endif