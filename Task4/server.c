#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT        9090
#define BACKLOG     10
#define BUF_SIZE    512
#define USERS_FILE  "users.txt"

pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;

int authenticate(const char *username, const char *password) {
    FILE *f = fopen(USERS_FILE, "r");
    if (!f) return 0;
    char line[256];
    int ok = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        char *sep = strchr(line, ':');
        if (!sep) continue;
        *sep = '\0';
        if (strcmp(line, username) == 0 && strcmp(sep + 1, password) == 0) {
            ok = 1;
            break;
        }
    }
    fclose(f);
    return ok;
}

int is_valid_line(const char *line, int len) {
    if (len <= 0 || len >= BUF_SIZE) return 0;
    for (int i = 0; i < len; i++)
        if (line[i] == '\0') return 0;
    return 1;
}

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);
    char buf[BUF_SIZE];
    int authenticated = 0;
    char username[64] = "";

    const char *welcome = "220 WELCOME OS-SECURITY-SERVER v1.0\n";
    send(client_fd, welcome, strlen(welcome), 0);

    while (1) {
        int n = recv(client_fd, buf, BUF_SIZE - 1, 0);
        if (n <= 0) {
            pthread_mutex_lock(&log_lock);
            printf("[server] client disconnected (fd=%d)\n", client_fd);
            pthread_mutex_unlock(&log_lock);
            break;
        }
        buf[n] = '\0';
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_valid_line(buf, strlen(buf))) {
            const char *err = "400 INVALID COMMAND\n";
            send(client_fd, err, strlen(err), 0);
            continue;
        }

        pthread_mutex_lock(&log_lock);
        printf("[server] received: %s\n", buf);
        pthread_mutex_unlock(&log_lock);

        if (strncmp(buf, "AUTH ", 5) == 0) {
            char user[64], pass[64];
            if (sscanf(buf + 5, "%63s %63s", user, pass) == 2 &&
                authenticate(user, pass)) {
                authenticated = 1;
                strncpy(username, user, sizeof(username) - 1);
                const char *ok = "200 AUTH OK\n";
                send(client_fd, ok, strlen(ok), 0);
            } else {
                const char *fail = "401 AUTH FAILED\n";
                send(client_fd, fail, strlen(fail), 0);
            }
        } else if (strncmp(buf, "MSG ", 4) == 0) {
            if (!authenticated) {
                const char *fail = "401 NOT AUTHENTICATED\n";
                send(client_fd, fail, strlen(fail), 0);
            } else {
                char reply[BUF_SIZE + 32];
                snprintf(reply, sizeof(reply), "200 ECHO(%s): %s\n", username, buf + 4);
                send(client_fd, reply, strlen(reply), 0);
            }
        } else if (strcmp(buf, "QUIT") == 0) {
            const char *bye = "200 BYE\n";
            send(client_fd, bye, strlen(bye), 0);
            break;
        } else {
            const char *unknown = "400 UNKNOWN COMMAND\n";
            send(client_fd, unknown, strlen(unknown), 0);
        }
    }
    close(client_fd);
    return NULL;
}

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on 127.0.0.1:%d ...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t alen = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &alen);
        if (*client_fd < 0) { perror("accept"); free(client_fd); continue; }

        printf("[server] new connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid);
    }
    close(server_fd);
    return 0;
}
