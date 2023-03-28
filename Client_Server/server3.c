#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9734

void handle_sqrt_request(int client_sockfd, int rq_id, const char* value_bytes) {
    double num;
    memcpy(&num, value_bytes, sizeof(double));
    double squared_num = sqrt(num);
    write(client_sockfd, &rq_id, sizeof(int));
    write(client_sockfd, &squared_num, sizeof(double));
}

void handle_time_request(int client_sockfd, int rq_id) {
    time_t rawtime;
    struct tm * timeinfo;
    char time_str[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_str, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("str with time: %s\n", time_str);
    printf("rq_id: %d\n", rq_id);
    int len = strlen(time_str);
    write(client_sockfd, &rq_id, sizeof(int));
    write(client_sockfd, &len, sizeof(int));
    write(client_sockfd, time_str, len);
}

int main() {
    int server_sockfd, client_sockfd;
    socklen_t client_len;
    struct sockaddr_in server_address, client_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    memset(server_address.sin_zero, 0, sizeof(server_address.sin_zero));

    if (bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sockfd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Server waiting for connection...\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

        printf("Client connected from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        while (1) {
            int request_type;
            int rq_id;
            int bytes_read = read(client_sockfd, &request_type, sizeof(request_type));
            printf("%d bytes_read from header: \n", bytes_read);
            if (bytes_read <= 0) {
                break;
            }

            request_type = be32toh(request_type);
            read(client_sockfd, &rq_id, sizeof(int));
            printf("request_type: %d\n", request_type);

            if (request_type == 0x00000001) {
                printf("sqrt\n");
                char num_bytes[sizeof(double)];
                read(client_sockfd, num_bytes, sizeof(double));
                handle_sqrt_request(client_sockfd, rq_id, num_bytes);
            } else if (request_type == 0x00000002) {
                printf("time\n");
                handle_time_request(client_sockfd, rq_id);
            } else {
                printf("wrong header :((\n");
            }
        }

        printf("Client disconnected from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        close(client_sockfd);
    }

    close(server_sockfd);

    return
}