#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <poll.h>

#include "socket_funcs.hpp"

bool wait_for_pollevent(int sockfd, int event, int timeout)
{
    struct pollfd pfds[1];
    pfds[0].fd = sockfd;
    pfds[0].events = event;

    int status = poll(pfds, 1, timeout);
    if (status > 0 && pfds[0].revents & event)
        return true;
    return false;
}

int create_socket_file_descriptor(struct addrinfo *info)
{
    int sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (sockfd == -1)
    {
        printf("Error calling socket(): %s\n", strerror(errno));
        return -1;
    }
    return sockfd;
}

/*------------ get addressinfo for binding to local port ------------*/
struct addrinfo *get_local_address(std::string port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_UNSPEC, AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    struct addrinfo *info;
    if ((status = getaddrinfo(NULL, port.c_str(), &hints, &info)) != 0)
    {
        printf("Error calling getaddrinfo(): %s\n", gai_strerror(status));
        return NULL;
    }
    return info;
}

/*------------ get addressinfo for localhost to start connect on port ------------*/
struct addrinfo *get_remote_address(std::string ip, std::string port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_UNSPEC, AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM;

    int status;
    struct addrinfo *info;
    if ((status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &info)) != 0)
    {
        printf("Error calling getaddrinfo(): %s\n", gai_strerror(status));
        return NULL;
    }
    return info;
}

int send_all(int s, const char *buf, int *len)
{
    int total = 0;
    int bytesleft = *len;
    int n;

    while (total < *len)
    {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }

    *len = total;

    return (n == -1) ? -1 : 0;
}
