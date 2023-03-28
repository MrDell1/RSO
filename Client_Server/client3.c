#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT "9734"

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sockfd, status;
    struct addrinfo hints, *res;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <sqrt|time> <value>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(SERVER_ADDRESS, SERVER_PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect() failed");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "sqrt") == 0) {
        double value = strtod(argv[2], NULL);
        snprintf(buffer, BUF_SIZE, "sqrt %lf", value);
    } else if (strcmp(argv[1], "time") == 0) {
        snprintf(buffer, BUF_SIZE, "time");
    } else {
        fprintf(stderr, "Invalid command: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (write(sockfd, buffer, strlen(buffer)) == -1) {
        perror("write() failed");
        exit(EXIT_FAILURE);
    }

    int nbytes;
    while ((nbytes = read(sockfd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[nbytes] = '\0';
        printf("%s", buffer);
    }
    if (nbytes == -1) {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);
    close(sockfd);

    return 0;
}