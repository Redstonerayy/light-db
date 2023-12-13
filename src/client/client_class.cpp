#include <string>
#include <unistd.h>
#include <stdio.h>
#include <vector>

#include "client_class.hpp"

#include "socket_client.hpp"

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
    std::vector<char> data = receive_data(this->sockfd);
    data.emplace_back(0);
    printf("%s\n", data.data());
    printf("%zu", data.size());
}
