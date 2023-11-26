#pragma once

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

const std::string PORT = "4466";
const int BACKLOG = 20;

struct addrinfo *getLocalAddress();
int makeListeningSocket(struct addrinfo *serverinfo);
void acceptConnections(int sockfd);

#endif