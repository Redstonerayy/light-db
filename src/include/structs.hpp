#pragma once

#include <vector>

struct Connection
{
    int sockfd;
    std::vector<char> buffered_data;
};

enum TABLE_DATATYPE {
    INT,             // 32 bit integer, id 1
    LONG,            // 64 bit integer, id 2
    CHAR_8,          // 8 bytes       , id 3
    CHAR_16,         // 16 bytes      , id 4
    CHAR_32,         // 32 bytes      , id 5
    UNIX_TIMESTAMP,  // 32 bit integer, id 6
    DATE,            // 32 bit integer, id 7
};
