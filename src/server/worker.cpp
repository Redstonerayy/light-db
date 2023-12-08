#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cerrno>
#include <sys/socket.h>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <sys/epoll.h>

#include "worker.hpp"

#include "server_class.hpp"
#include "server_config.hpp"

struct Connection
{
    int sockfd;
    std::vector<char> buffered_data;
};

template <class T>
T queue_front_pop(std::queue<T> &q)
{
    T temp = q.front();
    q.pop();
    return temp;
}

int get_sockfd_if_not_max_reached(Server &server, int &connection_count)
{
    std::unique_lock lock(server.incoming_connections_m);
    if (server.incoming_connections.empty())
    {
        if (connection_count == 0)
            server.incoming_connections_cv.wait(lock);
    }
    int sockfd = -1;
    if (connection_count < WORKER_EPOLL_MAX)
    {
        sockfd = queue_front_pop(server.incoming_connections);
        ++connection_count;
    }
    lock.unlock();
    return sockfd;
}

int setup_epoll()
{
    int epollfd = epoll_create1(0);

    if (epollfd == -1)
    {
        printf("Error calling epoll_create1(): %s\nExiting Thread\n", strerror(errno));
        return -1;
    }

    return epollfd;
}

int epoll_add_pollin_event(int epollfd, int sockfd, void *connection_data)
{
    struct epoll_event event
    {
    };
    event.events = EPOLLIN;
    event.data.ptr = connection_data;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) == -1)
    {
        printf("Error calling epoll_ctl() CTL_ADD: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

int epoll_remove_event(int epollfd, int sockfd)
{
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, nullptr) == -1)
    {
        printf("Error calling epoll_ctl() CTL_DEL: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

void close_connection(Connection *con, int epollfd, int &connection_count)
{
    epoll_remove_event(epollfd, con->sockfd);
    --connection_count;
    close(con->sockfd);
    free(con);
}

void receive_all_data(Connection *connection)
{
    int buffersize = 65536; // 2 ** 16 bytes
    while (true)
    {
        std::vector<char> buf(buffersize);
        int bytes_received = recv(connection->sockfd, buf.data(), buffersize, 0);
        if (bytes_received == -1)
        {
            if(errno != EAGAIN && errno != EWOULDBLOCK)
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
            connection->buffered_data.insert(connection->buffered_data.end(), buf.begin(), buf.begin() + bytes_received);
        }
    }
}

void process_epoll_event(void *connection_ptr, int &connection_count, int epollfd)
{
    Connection *connection = static_cast<Connection *>(connection_ptr);
    receive_all_data(connection);
    printf("%s\n", connection->buffered_data.data());
    close_connection(connection, epollfd, connection_count);
}

void process_epoll_events(struct epoll_event events[], int event_count, int &connection_count, int epollfd)
{
    for (int i = 0; i < event_count; ++i)
    {
        process_epoll_event(events[i].data.ptr, connection_count, epollfd);
    }
}

Connection *create_new_connection(int sockfd)
{
    Connection *new_con = new Connection;
    new_con->sockfd = sockfd;
    return new_con;
}

void worker_func(Server &server)
{
    int connection_count = 0;
    struct epoll_event events[WORKER_EPOLL_MAX];
    int epollfd = setup_epoll();
    if (epollfd == -1)
        return;

    while (true)
    {
        int sockfd = get_sockfd_if_not_max_reached(server, connection_count);
        if (sockfd != -1)
        {
            Connection *new_connection = create_new_connection(sockfd);
            epoll_add_pollin_event(epollfd, sockfd, new_connection);
        }

        int event_count = epoll_wait(epollfd, events, WORKER_EPOLL_MAX, 5);
        if (event_count > 0)
        {
            process_epoll_events(events, event_count, connection_count, epollfd);
        }
    }
}
