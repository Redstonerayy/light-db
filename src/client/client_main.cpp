#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <unistd.h>

#include "client.hpp"

int main(int argc, char *argv[])
{
    struct addrinfo *clientinfo = getLocalhostAddress();
    int sockfd = makeConnectingSocket(clientinfo);

    sendData(sockfd);

    return 0;
}
