#pragma once

#ifndef DATABASE_CLIENT_HPP
#define DATABASE_CLIENT_HPP

#include <string>

class DatabaseClient {
public:
    DatabaseClient(std::string ip);
    ~DatabaseClient();
    void Query(std::string querystring);
    void Result();
private:
    int sockfd;
};

#endif
