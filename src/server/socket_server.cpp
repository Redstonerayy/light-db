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

#include "socket_server.hpp"

#include "socket_shared.hpp"

/*------------ get addressinfo for binding to local port ------------*/
struct addrinfo *get_local_address(std::string port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_UNSPEC, AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    struct addrinfo *serverinfo;
    if ((status = getaddrinfo(NULL, port.c_str(), &hints, &serverinfo)) != 0)
    {
        printf("Error calling getaddrinfo(): %s\n", gai_strerror(status));
        return NULL;
    }
    return serverinfo;
}

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

void send_data(int sockfd, std::string querystring)
{
    bool pollout_event = wait_for_pollevent(sockfd, POLLOUT, -1);
    if (pollout_event)
    {
        const char *msg = querystring.c_str();
        int len = querystring.size();
        int bytes_send = send_all(sockfd, msg, &len);
        if (bytes_send == -1)
            printf("Error calling send_all()");
        if (bytes_send > 0)
            printf("%d Bytes Send\n", bytes_send);
    }
}
