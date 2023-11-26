#include "server.hpp"

int main(int argc, char *argv[])
{
    struct addrinfo *serverinfo = getLocalAddress();
    int sockfd = makeListeningSocket(serverinfo);

    acceptConnections(sockfd);

    return 0;
}
