#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const std::string PORT = "4466";
const int BACKLOG = 20;

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char *argv[])
{
    struct addrinfo hints;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    struct addrinfo *serverinfo;
    if ((status = getaddrinfo(NULL, PORT.c_str(), &hints, &serverinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    int sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if(sockfd < 0) printf("Error %s\n", strerror(errno));
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }
    int b = bind(sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen);
    if(b < 0) printf("Error %s\n", strerror(errno));

    int l = listen(sockfd, 20);
    if(l < 0) printf("Error %s\n", strerror(errno));

    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(struct sockaddr_storage);
    int a = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size); 
    if(a < 0) printf("Error %s\n", strerror(errno));

    char buf[21] = { 0 };
    int bytes_received = recv(a, buf, 20, 0);
    if(bytes_received < 0) printf("Error %s\n", strerror(errno));
    if(bytes_received > 0) printf("Bytes in Buffer %d\n", bytes_received);
    if(bytes_received == 0) printf("con closed");

    printf("Res: %s", buf);

    close(a);
    close(sockfd);

    freeaddrinfo(serverinfo); // free the linked list

    return 0;
}
