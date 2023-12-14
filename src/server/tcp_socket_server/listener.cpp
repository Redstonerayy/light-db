#include <stdio.h>
#include <string.h>
#include <cerrno>
#include <sys/socket.h>
#include <poll.h>
#include <mutex>
#include <queue>
#include <fcntl.h>
#include <condition_variable>

#include "listener.hpp"
#include "server_constants.hpp"
#include "tcp_socket_server.hpp"
#include "socket_server.hpp"
#include "socket_shared.hpp"

int accept_incoming_connection(int sockfd)
{
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_storage);
    int new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
    if (new_sockfd == -1)
        printf("Error calling accept(): %s\n", strerror(errno));
    return new_sockfd;
}

void add_connection_to_queue_and_notify(int new_sockfd, TCP_Socket_Server &server)
{
    std::unique_lock lock(server.incoming_connections_m);
    server.incoming_connections.emplace(new_sockfd);
    lock.unlock();
    server.incoming_connections_cv.notify_one();
}

void add_incoming_connections_to_queue(int sockfd, TCP_Socket_Server &server)
{
    printf("Accepting Connections\n");

    while (true)
    {
        int pollin_event = wait_for_pollevent(sockfd, POLLIN, -1);
        if (pollin_event)
        {
            int new_sockfd = accept_incoming_connection(sockfd);
            fcntl(new_sockfd, F_SETFL, O_NONBLOCK);
            add_connection_to_queue_and_notify(new_sockfd, server);
        }
    }
}

void listener_func(TCP_Socket_Server &server)
{
    struct addrinfo *serverinfo = get_local_address(PORT);
    int sockfd = make_listening_socket(serverinfo, BACKLOG);
    add_incoming_connections_to_queue(sockfd, server);
}
