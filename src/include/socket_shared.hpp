#pragma once

#ifndef SHARED_HPP
#define SHARED_HPP

void *get_internet_addr(struct sockaddr *sa);
int create_nonblocking_socket(struct addrinfo *info);
bool wait_for_pollevent(int sockfd, int event, int timeout);
void send_data(int sockfd, std::string querystring);

#endif