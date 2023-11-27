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

#include "server.hpp"
#include "shared.hpp"

/*------------ get addressinfo for binding to local port ------------*/
struct addrinfo *getLocalAddress()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_UNSPEC, AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    struct addrinfo *serverinfo;
    if ((status = getaddrinfo(NULL, PORT.c_str(), &hints, &serverinfo)) != 0)
    {
        printf("Error calling getaddrinfo(): %s\n", gai_strerror(status));
        return NULL;
    }
    return serverinfo;
}

/*------------ avoids "address already in use" error ------------*/
int bindSocketWithReuse(int sockfd, struct addrinfo *info)
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

void listenOnSocket(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) == -1)
    {
        printf("Error calling listen(): %s\n", strerror(errno));
        exit(1);
    }
}

/*------------ loop over serverinfo and create socket with matching entry ------------*/
int makeListeningSocket(struct addrinfo *serverinfo)
{
    int sockfd;
    struct addrinfo *info = serverinfo;
    for (; info != NULL; info = info->ai_next)
    {
        sockfd = createNonblockingSocket(info);
        if (bindSocketWithReuse(sockfd, info) == 0)
            break;
    }
    freeaddrinfo(serverinfo);

    if (info == NULL)
    {
        printf("Could not create or bind socket with any Address\n");
        exit(1);
    }

    listenOnSocket(sockfd, BACKLOG);

    return sockfd;
}

void processConnection(int sockfd)
{
    int buffersize = 512 * 32; // bytes
    std::vector<char> data;
    while (true)
    {
        std::vector<char> buf(buffersize);
        int bytes_received = recv(sockfd, buf.data(), buffersize, 0);
        if (bytes_received == -1)
        {
            printf("Error calling recv(): %s\n", strerror(errno));
            break;
        }
        if (bytes_received == 0)
        {
            printf("Connection closed by client\n");
            break;
        }
        if (bytes_received > 0)
        {
            printf("Bytes in Buffer %d\n", bytes_received);
            data.insert(data.end(), buf.begin(), buf.begin() + bytes_received);
        }
    }
    printf("Client Message: %s\n", data.data());
    close(sockfd);
}

/*------------ use poll and start new threads for each connection ------------*/
void acceptConnections(int sockfd)
{
    printf("Accepting Connections\n");

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_storage);
    std::vector<std::thread> workers;
    while (true)
    {
        struct pollfd pfds[1];
        pfds[0].fd = sockfd;
        pfds[0].events = POLLIN;

        int poll_events = poll(pfds, 1, -1); // wait until connection
        int is_pollin_event = pfds[0].revents & POLLIN;
        if (is_pollin_event)
        {
            int new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
            if (new_sockfd == -1)
                printf("Error calling accept(): %s\n", strerror(errno));
            std::thread new_con(processConnection, new_sockfd);
            new_con.detach();
        }
    }

    close(sockfd);
}
