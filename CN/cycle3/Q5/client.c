#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int sock;

// Function to receive messages from the server
void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("\n[!] Server disconnected.\n");
            exit(1);
        }
        printf("\n%s\n", buffer);
    }
    return NULL;
}

int main() {
    char *ip = "127.0.0.1";
    int port = 5566;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    pthread_t recv_thread;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[.] Socket Error");
        exit(1);
    }

    printf("[+] TCP client socket created.\n");

    // Configure server address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    // Connect to server
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[-] Connection failed");
        exit(1);
    }
    printf("[+] Connected to server.\n");

    printf("Enter EXIT_CHAT in all caps to leave chatroom\n");

    // Create a separate thread to receive messages
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_detach(recv_thread);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Check for exit condition
        if (strcmp(buffer, "EXIT_CHAT") == 0) {
            printf("\nDisconnected from the server.\n");
            close(sock);
            break;
        }

        send(sock, buffer, strlen(buffer), 0);
    }

    return 0;
}

