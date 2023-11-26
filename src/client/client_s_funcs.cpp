#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <unistd.h>
#include <poll.h>

#include "client.hpp"
#include "shared.hpp"

/*------------ get addressinfo for localhost to start connect on port ------------*/
struct addrinfo *getLocalhostAddress()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    int status;
    struct addrinfo *serverinfo;
    if ((status = getaddrinfo("127.0.0.1", PORT.c_str(), &hints, &serverinfo)) != 0)
    {
        printf("Error calling getaddrinfo(): %s\n", gai_strerror(status));
        return NULL;
    }
    return serverinfo;
}

void connectOnSocket(int sockfd, struct addrinfo *clientinfo)
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

int makeConnectingSocket(struct addrinfo *clientinfo)
{
    int sockfd;
    struct addrinfo *info = clientinfo;
    for (; info != NULL; info = info->ai_next)
    {
        if ((sockfd = createNonblockingSocket(info)) != -1)
            break;
    }
    freeaddrinfo(clientinfo);

    if (info == NULL)
    {
        printf("Could not create socket with any Address\n");
        exit(1);
    }

    connectOnSocket(sockfd, clientinfo);

    return sockfd;
}

void sendData(int sockfd)
{
    struct pollfd pfds[1];
    pfds[0].fd = sockfd;
    pfds[0].events = POLLOUT;

    int poll_events = poll(pfds, 1, -1); // wait until sending possible
    int is_pollout_event = pfds[0].revents & POLLOUT;
    if (is_pollout_event)
    {
        const char *msg = "1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30";
        int len = strlen(msg);
        int bytes_send = send(sockfd, msg, len, 0);
        if (bytes_send < 0)
            printf("Error %s\n", strerror(errno));
        if (bytes_send > 0)
            printf("Bytes Send %d\n", bytes_send);
    }

    close(sockfd);
}