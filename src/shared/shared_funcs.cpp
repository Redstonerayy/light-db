#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <thread>
#include <vector>

/*------------ convert sockaddr to address matching ip version ------------*/
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/*------------ use poll() with this socket ------------*/
int createNonblockingSocket(struct addrinfo *info)
{
    int sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (sockfd == -1)
    {
        printf("Error calling socket(): %s\n", strerror(errno));
        return -1;
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
}
