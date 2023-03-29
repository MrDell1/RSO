#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define SERVER_ADDRESS "192.168.178.19"
#define SERVER_PORT "1234"

#define BUF_SIZE 1024


typedef struct {
    uint32_t type;
    uint32_t id;
    char value_bytes[sizeof(double)];
} Request;

typedef struct {
    uint32_t type;
    uint32_t id;
} TimeRequest;

int main(int argc, char *argv[]) {
    int sockfd, status;
    struct addrinfo hints, *res;

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
        Request req;

        req.type = htobe32(0x00000001); // Message type: 1 (calculate square root)
        req.id = htobe32(0x00000001);   // Request ID: 1
        double value = strtod(argv[2], NULL);
        memcpy(req.value_bytes, &value, sizeof(double));
        if (write(sockfd, &req, sizeof(req)) != sizeof(req)) {
            perror("write() failed");
            exit(EXIT_FAILURE);
        }

        int nbytes;
        if (read(sockfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        printf("Header: %d\n", be32toh(nbytes));

        if (read(sockfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        printf("Request id: %d\n", be32toh(nbytes));

        char num_bytes[sizeof(double)];
        if (read(sockfd, &num_bytes, sizeof(double)) != sizeof(double)) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        double num;
        memcpy(&num, num_bytes, sizeof(double));
        printf("Result: %lf\n", num);
        close(sockfd);
    } else if (strcmp(argv[1], "time") == 0) {
        TimeRequest req;

        req.type = htobe32(0x00000002); // Message type: 1 (calculate square root)
        req.id = htobe32(0x00000002);

        if (write(sockfd, &req, sizeof(req)) != sizeof(req)) {
            perror("write() failed");
            exit(EXIT_FAILURE);
        }
        int nbytes;
        if (read(sockfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        printf("Header: %d\n", be32toh(nbytes));

        if (read(sockfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        printf("Request id: %d\n", be32toh(nbytes));

        if (read(sockfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        printf("Length: %d\n", be32toh(nbytes));

        char time[100] = {0};
        if (read(sockfd, &time, nbytes) != nbytes) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        printf("Response: %s\n", time);
    } else {
        fprintf(stderr, "Invalid command: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }






    freeaddrinfo(res);
    close(sockfd);

    return 0;
}
