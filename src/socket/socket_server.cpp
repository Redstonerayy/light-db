#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>

#include "socket_server.hpp"

#include "socket_shared.hpp"

/*------------ avoids "address already in use" error ------------*/
int bind_socket_with_reuse(int sockfd, struct addrinfo *info)
{
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
    {
        printf("Error calling setsockopt(): %s\nAddress already in use?\n", strerror(errno));
        return -1;
    }

    if (bind(sockfd, info->ai_addr, info->ai_addrlen) == -1)
    {
        printf("Error calling bind(): %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

void listen_on_socket(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) == -1)
    {
        printf("Error calling listen(): %s\n", strerror(errno));
        exit(1);
    }
}

/*------------ loop over serverinfo and create socket with matching entry ------------*/
int make_listening_socket(struct addrinfo *serverinfo, int backlog)
{
    int sockfd;
    struct addrinfo *info = serverinfo;
    for (; info != NULL; info = info->ai_next)
    {
        sockfd = create_nonblocking_socket(info);
        if (bind_socket_with_reuse(sockfd, info) == 0)
            break;
    }
    freeaddrinfo(serverinfo);

    if (info == NULL)
    {
        printf("Could not create or bind socket with any Address\n");
        exit(1);
    }

    listen_on_socket(sockfd, backlog);

    return sockfd;
}
