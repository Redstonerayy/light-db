#include <stdio.h>
#include <string.h>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <string>

#include "socket_shared.hpp"
#include "socket_funcs.hpp"

/*------------ convert sockaddr to address matching ip version ------------*/
void *get_internet_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/*------------ use poll() with this socket ------------*/
int create_nonblocking_socket(struct addrinfo *info)
{
    int sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (sockfd == -1)
    {
        printf("Error calling socket(): %s\n", strerror(errno));
        return -1;
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
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
