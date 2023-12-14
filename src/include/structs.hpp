#pragma once
#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <vector>

struct Connection
{
    int sockfd;
    std::vector<char> buffered_data;
};

#endif
