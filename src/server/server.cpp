/*
** showip.c -- show IP addresses for a host given on the command line
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, "3003", &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    
    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(s < 0) printf("Error %s\n", strerror(errno));
    int yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }
    int b = bind(s, res->ai_addr, res->ai_addrlen);
    if(b < 0) printf("Error %s\n", strerror(errno));

    int l = listen(s, 20);
    if(l < 0) printf("Error %s\n", strerror(errno));

    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(struct sockaddr_storage);
    int a = accept(s, (struct sockaddr *)&their_addr, &addr_size); 
    if(a < 0) printf("Error %s\n", strerror(errno));

    char buf[20] = { 0 };
    int bytes_received = recv(a, buf, 20, 0);
    if(bytes_received < 0) printf("Error %s\n", strerror(errno));
    if(bytes_received > 0) printf("Bytes in Buffer %d\n", bytes_received);
    if(bytes_received == 0) printf("con closed");

    printf("Res: %s", buf);

    close(a);
    close(s);

    freeaddrinfo(res); // free the linked list

    return 0;
}
