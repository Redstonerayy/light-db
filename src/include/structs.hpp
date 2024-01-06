#pragma once

#include <vector>

struct Connection
{
    int sockfd;
    std::vector<char> buffered_data;
};

enum TABLE_DATATYPE {
    INT,             // 32 bit integer
    LONG,            // 64 bit integer
    CHAR_8,          // 8 bytes
    CHAR_16,         // 16 bytes
    CHAR_32,         // 32 bytes
    UNIX_TIMESTAMP,  // 32 bit integer
    DATE,            // 32 bit integer
};
