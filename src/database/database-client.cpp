#include <string>
#include <unistd.h>

#include "client.hpp"
#include "database-client.hpp"

DatabaseClient::DatabaseClient(std::string ip)
{
    /*------------ connect ------------*/
    struct addrinfo *clientinfo = getRemoteAddress(ip);
    int sockfd = makeConnectingSocket(clientinfo);
    // TODO http(s)
    this->sockfd = sockfd;
}

DatabaseClient::~DatabaseClient()
{
    close(this->sockfd);
}

void DatabaseClient::Query(std::string querystring)
{
    sendData(this->sockfd, querystring);
}

void DatabaseClient::Result()
{
    receiveData(this->sockfd);
}
