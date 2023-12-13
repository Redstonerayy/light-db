#pragma once

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

struct addrinfo *get_local_address(std::string port);
int make_listening_socket(struct addrinfo *serverinfo, int backlog);
void send_data(int sockfd, std::string querystring);

#endif
