#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <unistd.h>
#include <poll.h>
#include <vector>

#include "socket_client.hpp"

#include "socket_shared.hpp"

/*------------ get addressinfo for localhost to start connect on port ------------*/
struct addrinfo *get_remote_address(std::string ip, std::string port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC, AF_INET or AF_INET6
    hints.ai_socktype = SOCK_STREAM;

    int status;
    struct addrinfo *serverinfo;
    if ((status = getaddrinfo(ip.c_str(), port.c_str(), &hints, &serverinfo)) != 0)
    {
        printf("Error calling getaddrinfo(): %s\n", gai_strerror(status));
        return NULL;
    }
    return serverinfo;
}

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

    close(sockfd);
}

void receive_data(int sockfd)
{
    bool pollin_event = wait_for_pollevent(sockfd, POLLIN, 5);
    if (pollin_event)
    {
        int buffersize = 8; // bytes
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
                printf("Connection closed by client");
                break;
            }
            if (bytes_received > 0)
            {
                printf("Bytes in Buffer %d\n", bytes_received);
                data.insert(data.end(), buf.begin(), buf.begin() + bytes_received);
            }
        }
        printf("Res %d: %s\n", sockfd, data.data());
    }
}
