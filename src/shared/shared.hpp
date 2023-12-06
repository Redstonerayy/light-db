#pragma once

#ifndef SHARED_HPP
#define SHARED_HPP

void *get_in_addr(struct sockaddr *sa);
int createNonblockingSocket(struct addrinfo *info);
bool wait_for_pollevent(int sockfd, int event, int timeout);

#endif