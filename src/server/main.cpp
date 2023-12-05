#include "server.hpp"

int main(int argc, char *argv[])
{
    /*------------ TODO cli arguments ------------*/
    struct addrinfo *serverinfo = getLocalAddress();
    int sockfd = makeListeningSocket(serverinfo);

    acceptConnections(sockfd);

    return 0;
}
