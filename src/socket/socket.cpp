#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

#include "socket.hpp"
#include "socket_funcs.hpp"

Socket::Socket() {}

Socket::~Socket() {
    close(this->sockfd);
}

void Socket::set_to_nonblocking()
{
    fcntl(this->sockfd, F_SETFL, O_NONBLOCK);
}

void Socket::create_socket_from_addrinfo(struct addrinfo *info, bool blocking)
{
    for (; info != NULL; info = info->ai_next)
    {
        if ((this->sockfd = create_socket_file_descriptor(info)) != -1)
            break;
    }

    if (info == NULL)
    {
        printf("Could not create socket with any Address\n");
    }

    if (!blocking)
        this->set_to_nonblocking();
}

void Socket::connect_to(std::string ip, std::string port, bool blocking)
{
    struct addrinfo *info_linked_list = get_remote_address(ip, port);
    struct addrinfo *info = info_linked_list;
    this->create_socket_from_addrinfo(info, blocking);

    if (connect(this->sockfd, info->ai_addr, info->ai_addrlen) == -1)
        // if the error ist EINPROGRESS everything is fine
        // we could poll() and check the error,
        // but we will poll() later anyway
        if (errno != EINPROGRESS)
            printf("Error calling connect(): %s\n", strerror(errno));

    freeaddrinfo(info_linked_list);
}

int Socket::bind_to(struct addrinfo *info)
{
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
    {
        printf("Error calling setsockopt(): %s\nAddress already in use?\n", strerror(errno));
        return -1;
    }

    if (bind(this->sockfd, info->ai_addr, info->ai_addrlen) == -1)
    {
        printf("Error calling bind(): %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

void Socket::listen_on(std::string port, int backlog, bool blocking)
{
    struct addrinfo *info_linked_list = get_local_address(port);
    struct addrinfo *info = info_linked_list;
    this->create_socket_from_addrinfo(info, blocking);

    if(this->bind_to(info) == -1)
        return;

    if (listen(this->sockfd, backlog) == -1)
    {
        printf("Error calling listen(): %s\n", strerror(errno));
        exit(1);
    }

    freeaddrinfo(info_linked_list);
}

void Socket::send_when_ready(std::string querystring)
{
    bool pollout_event = wait_for_pollevent(this->sockfd, POLLOUT, -1);
    if (pollout_event)
    {
        const char *msg = querystring.c_str();
        int len = querystring.size();
        int bytes_send = send_all(this->sockfd, msg, &len);
        if (bytes_send == -1)
            printf("Error calling send_all()");
        if (bytes_send > 0)
            printf("%d Bytes Send\n", bytes_send);
    }
}

std::vector<char> Socket::receive_when_ready()
{
    bool pollin_event = wait_for_pollevent(this->sockfd, POLLIN, -1);
    std::vector<char> data;
    data.reserve(65546);
    if (pollin_event)
    {
        int buffersize = 65536; // 2 ** 16 bytes
        while (true)
        {
            std::vector<char> buf(buffersize);
            int bytes_received = recv(this->sockfd, buf.data(), buffersize, 0);
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
