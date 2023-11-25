/*
** showip.c -- show IP addresses for a host given on the command line
*/

#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>
#include <unistd.h>

const std::string PORT = "4466";

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo("127.0.0.1", PORT.c_str(), &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(s < 0) printf("Error %s\n", strerror(errno));
    int c = connect(s, res->ai_addr, res->ai_addrlen);
    if(c < 0) printf("Error %s\n", strerror(errno));

    const char *msg = "testaasdfasdfasdfasdfasdfsdf";
    int len = strlen(msg);
    int bytes_send = send(s, msg, len, 0);
    if(bytes_send < 0) printf("Error %s\n", strerror(errno));
    if(bytes_send > 0) printf("Bytes Send %d", bytes_send);

    close(s);

    freeaddrinfo(res); // free the linked list

    return 0;
}
