#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT     9090
#define BUF_SIZE 512

int send_and_receive(int sock, const char *msg, char *reply, int reply_size) {
    if (send(sock, msg, strlen(msg), 0) < 0) {
        perror("send");
        return -1;
    }
    int n = recv(sock, reply, reply_size - 1, 0);
    if (n <= 0) {
        printf("Connection closed by server or error.\n");
        return -1;
    }
    reply[n] = '\0';
    return n;
}

int main(int argc, char *argv[]) {
    const char *username = (argc > 1) ? argv[1] : "admin";
    const char *password = (argc > 2) ? argv[2] : "admin123";

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    char buf[BUF_SIZE];
    int n = recv(sock, buf, sizeof(buf) - 1, 0);
    if (n > 0) { buf[n] = '\0'; printf("Server: %s", buf); }

    char msg[BUF_SIZE];
    snprintf(msg, sizeof(msg), "AUTH %s %s\n", username, password);
    if (send_and_receive(sock, msg, buf, sizeof(buf)) < 0) { close(sock); return 1; }
    printf("Server: %s", buf);

    snprintf(msg, sizeof(msg), "MSG Hello from client, this is a test message\n");
    if (send_and_receive(sock, msg, buf, sizeof(buf)) < 0) { close(sock); return 1; }
    printf("Server: %s", buf);

    snprintf(msg, sizeof(msg), "QUIT\n");
    if (send_and_receive(sock, msg, buf, sizeof(buf)) < 0) { close(sock); return 1; }
    printf("Server: %s", buf);

    close(sock);
    return 0;
}
