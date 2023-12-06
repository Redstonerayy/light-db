#include <string>
#include <unistd.h>

#include "client.hpp"
#include "client_class.hpp"

Client::Client(std::string ip, std::string port)
{
    /*------------ connect ------------*/
    struct addrinfo *clientinfo = get_remote_address(ip, port);
    int sockfd = make_connecting_socket(clientinfo);
    this->sockfd = sockfd;
}

Client::~Client()
{
    close(this->sockfd);
}

void Client::Query(std::string querystring)
{
    send_data(this->sockfd, querystring);
}

void Client::Result()
{
    receive_data(this->sockfd);
}
