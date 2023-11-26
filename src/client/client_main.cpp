#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <cerrno>
#include <unistd.h>

#include "database-client.hpp"

int main(int argc, char *argv[])
{
    /*------------ authenticate ------------*/
    //skip for now, TODO openssl, secret

    /*------------ query ------------*/
    DatabaseClient client("127.0.0.1");
    client.Query("hallo server");

    /*------------ visualize data ------------*/

    return 0;
}
