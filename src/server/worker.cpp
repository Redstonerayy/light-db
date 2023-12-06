#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cerrno>
#include <sys/socket.h>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "worker.hpp"

#include "server_class.hpp"
#include "util.hpp"

template <class T>
T queue_front_pop(std::queue<T> &q)
{
    T temp = q.front();
    q.pop();
    return temp;
}

int get_next_sockfd(Server &server)
{
    std::unique_lock lock(server.incoming_connections_m);
    if (server.incoming_connections.empty())
    {
        server.incoming_connections_cv.wait(lock);
    }
    int sockfd = queue_front_pop(server.incoming_connections);
    lock.unlock();
    return sockfd;
}

void worker_func(Server &server)
{
    while (true)
    {
        int sockfd = get_next_sockfd(server);
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
                // printf("Bytes in Buffer %d\n", bytes_received);
                // printf("VSize %d\n", buf.size());
                data.insert(data.end(), buf.begin(), buf.begin() + bytes_received);
            }
        }
        // printf("VSize %d\n", data.size());
        data.resize(data.size() + 1);
        data.at(data.size() - 1) = 0;
        printf("Client Message %d: %s\n", server.incoming_connections.size(), data.data());
        close(sockfd);
    }
}
