#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <vector>

class Socket
{
public:
    int sockfd;

    Socket();
    ~Socket();
    void connect_to(std::string ip, std::string port, bool blocking = false);
    void listen_on(std::string port, int backlog, bool blocking = false);

    void send_when_ready(std::string querystring);
    std::vector<char> receive_when_ready();
    
private:
    void create_socket_from_addrinfo(struct addrinfo *info, bool blocking);
    void set_to_nonblocking();
    int bind_to(struct addrinfo *info);
};

#endif
