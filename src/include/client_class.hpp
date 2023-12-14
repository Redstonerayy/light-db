#pragma once

#ifndef DATABASE_CLIENT_HPP
#define DATABASE_CLIENT_HPP

#include <string>

#include "socket.hpp"

class Client {
public:
    Client(std::string ip, std::string port);
    void Query(std::string querystring);
    void Result();
private:
    Socket socket;
};

#endif
