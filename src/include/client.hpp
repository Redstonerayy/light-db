#pragma once

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

const std::string PORT = "4466";

struct addrinfo *getRemoteAddress(std::string ip);
int makeConnectingSocket(struct addrinfo *clientinfo);
void sendData(int sockfd, std::string querystring);
void receiveData(int sockfd);

#endif
