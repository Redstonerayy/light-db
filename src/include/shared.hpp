#pragma once

#ifndef SHARED_HPP
#define SHARED_HPP

void *get_in_addr(struct sockaddr *sa);
int createNonblockingSocket(struct addrinfo *info);

#endif