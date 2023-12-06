#include <stdio.h>
#include <string.h>
#include <cerrno>
#include <sys/socket.h>
#include <poll.h>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "listener.hpp"

#include "server.hpp"
#include "shared.hpp"
#include "server_class.hpp"

/*------------ use poll and add file descriptors to job queue ------------*/
void add_incoming_connections_to_queue(int sockfd, Server& server)
{
    printf("Accepting Connections\n");

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_storage);
    while (true)
    {
        int pollin_event = wait_for_pollevent(sockfd, POLLIN, -1);
        if (pollin_event)
        {
            int new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
            if (new_sockfd == -1)
                printf("Error calling accept(): %s\n", strerror(errno));
            
            std::unique_lock lock(server.incoming_connections_m);
            // server.incoming_connections_cv.wait(lock);
            server.incoming_connections.emplace(new_sockfd);
            printf("Jobs %d\n", server.incoming_connections.size());
            lock.unlock();
            server.incoming_connections_cv.notify_one();
        }
    }
}

void listener_func(Server& server)
{
    struct addrinfo *serverinfo = get_local_address();
    int sockfd = make_listening_socket(serverinfo);
    add_incoming_connections_to_queue(sockfd, server);
}
