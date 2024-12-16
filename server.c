#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "register.h"
#include "login.h"
#include "logout.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int read_size;
    
    // Đọc yêu cầu từ client
    while ((read_size = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Client request: %s\n", buffer);
        
        // Xử lý yêu cầu
        if (strncmp(buffer, "register", 8) == 0) {
            // Đăng ký
            char *username = strtok(buffer + 9, " ");
            char *password = strtok(NULL, " ");
            if (registerAccount(username, password)) {
                send(client_sock, "Registration successful\n", 24, 0);
            } else {
                send(client_sock, "Username already exists\n", 24, 0);
            }
        } else if (strncmp(buffer, "login", 5) == 0) {
            // Đăng nhập
            char *username = strtok(buffer + 6, " ");
            char *password = strtok(NULL, " ");
            if (loginAccount(username, password)) {
                send(client_sock, "Login successful\n", 18, 0);
            } else {
                send(client_sock, "Invalid username or password\n", 30, 0);
            }
        } else if (strncmp(buffer, "logout", 6) == 0) {
            // Đăng xuất
            logout();
        } else {
            send(client_sock, "Invalid command\n", 17, 0);
        }
    }

    if (read_size == 0) {
        printf("Client disconnected\n");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_sock, 3) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, &client_len))) {
        printf("Connection accepted\n");
        handle_client(client_sock);
    }

    close(server_sock);
    return 0;
}
