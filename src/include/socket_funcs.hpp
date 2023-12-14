#pragma once
#ifndef SOCKET_FUNCS_HPP
#define SOCKET_FUNCS_HPP

#include <string>

bool wait_for_pollevent(int sockfd, int event, int timeout);
int create_socket_file_descriptor(struct addrinfo *info);
struct addrinfo *get_local_address(std::string port);
struct addrinfo *get_remote_address(std::string ip, std::string port);
int send_all(int s, const char *buf, int *len);
void send_data(int sockfd, std::string querystring);

#endif
