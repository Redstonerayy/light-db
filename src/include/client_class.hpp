#pragma once

#ifndef DATABASE_CLIENT_HPP
#define DATABASE_CLIENT_HPP

#include <string>

class Client {
public:
    Client(std::string ip, std::string port);
    ~Client();
    void Query(std::string querystring);
    void Result();
private:
    int sockfd;
};

#endif
