#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.100"  // Địa chỉ IP của máy chủ
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void send_request(int sock, const char *request) {
    send(sock, request, strlen(request), 0);
    char buffer[BUFFER_SIZE];
    int read_size = recv(sock, buffer, sizeof(buffer), 0);
    buffer[read_size] = '\0';
    printf("Server response: %s\n", buffer);
}

int main() {
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        perror("Invalid server IP address");
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Gửi yêu cầu đăng ký
    send_request(sock, "register john_doe password123");

    // Gửi yêu cầu đăng nhập
    send_request(sock, "login john_doe password123");

    // Gửi yêu cầu đăng xuất
    send_request(sock, "logout");

    close(sock);
    return 0;
}
