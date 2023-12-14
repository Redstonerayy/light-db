#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <poll.h>
#include <vector>

#include "socket_client.hpp"

#include "socket_shared.hpp"

void connect_on_socket(int sockfd, struct addrinfo *clientinfo)
{
    if (connect(sockfd, clientinfo->ai_addr, clientinfo->ai_addrlen) == -1)
        // if the error ist EINPROGRESS everything is fine
        // we could poll() and check the error,
        // but we will poll() later anyway
        if (errno != EINPROGRESS)
        {
            printf("Error calling connect(): %s\n", strerror(errno));
            exit(1);
        }
}

int make_connecting_socket(struct addrinfo *clientinfo)
{
    int sockfd;
    struct addrinfo *info = clientinfo;
    for (; info != NULL; info = info->ai_next)
    {
        if ((sockfd = create_nonblocking_socket(info)) != -1)
            break;
    }
    freeaddrinfo(clientinfo);

    if (info == NULL)
    {
        printf("Could not create socket with any Address\n");
        exit(1);
    }

    connect_on_socket(sockfd, clientinfo);

    return sockfd;
}

std::vector<char> receive_data(int sockfd)
{
    bool pollin_event = wait_for_pollevent(sockfd, POLLIN, -1);
    std::vector<char> data;
    data.reserve(65546);
    if(pollin_event)
    {
        int buffersize = 65536; // 2 ** 16 bytes
        while (true)
        {
            std::vector<char> buf(buffersize);
            int bytes_received = recv(sockfd, buf.data(), buffersize, 0);
            if (bytes_received == -1)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                    printf("Error calling recv(): %s\n", strerror(errno));
                break;
            }
            else if (bytes_received == 0)
            {
                printf("Connection closed by client\n");
                break;
            }
            else if (bytes_received > 0)
            {
                data.insert(data.end(), buf.begin(), buf.begin() + bytes_received);
            }
        }
        return data;
    }
    return data;
}
