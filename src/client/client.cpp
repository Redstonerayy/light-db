#include <string>
#include <stdio.h>
#include <vector>

#include "client.hpp"
#include "socket.hpp"

Client::Client(std::string ip, std::string port)
{
    this->socket = Socket();
    this->socket.connect_to(ip, port);
}

void Client::Query(std::string querystring)
{
    this->socket.send_when_ready(querystring);
}

void Client::Result()
{
    std::vector<char> data = this->socket.receive_when_ready();
    data.emplace_back(0);
    printf("%s\n", data.data());
}
