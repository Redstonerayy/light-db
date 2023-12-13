#pragma once

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

struct addrinfo *get_remote_address(std::string ip, std::string port);
int make_connecting_socket(struct addrinfo *clientinfo);
void send_data(int sockfd, std::string querystring);
std::vector<char> receive_data(int sockfd);

#endif
