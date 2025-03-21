#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];  // Store client sockets
int client_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Struct to store client information
typedef struct {
    int id;
    int socket;
} ClientInfo;

// Broadcast message to all clients except sender
void broadcast_message(char *message, int sender_sock, int sender_id) {
    pthread_mutex_lock(&lock);
    char formatted_msg[BUFFER_SIZE];
    snprintf(formatted_msg, sizeof(formatted_msg), "Client %d: %s", sender_id, message);

    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_sock) {
            send(clients[i], formatted_msg, strlen(formatted_msg), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

// Handle client communication
void *handle_client(void *arg) {
    ClientInfo *client_info = (ClientInfo *)arg;
    int client_sock = client_info->socket;
    int client_id = client_info->id;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            printf("[!] Client %d disconnected.\n", client_id);
            close(client_sock);

            // Remove client from list
            pthread_mutex_lock(&lock);
            for (int i = 0; i < client_count; i++) {
                if (clients[i] == client_sock) {
                    clients[i] = clients[client_count - 1];
                    client_count--;
                    break;
                }
            }
            pthread_mutex_unlock(&lock);
            break;
        }

        printf("Client %d: %s\n", client_id, buffer);
        broadcast_message(buffer, client_sock, client_id);
    }
    free(client_info);  // Free allocated memory
    return NULL;
}

int main() {
    char *ip = "127.0.0.1";
    int port = 5566;
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t thread_id;

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[.] Socket Error");
        exit(1);
    }

    printf("[+] TCP server socket created.\n");

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Bind socket
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[.] Bind error");
        exit(1);
    }
    printf("[+] Bound to port %d\n", port);

    // Listen for connections
    listen(server_sock, MAX_CLIENTS);
    printf("Listening for connections...\n");

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("[-] Accept failed");
            continue;
        }

        // Assign client ID and add to list
        pthread_mutex_lock(&lock);
        clients[client_count] = client_sock;
        int client_id = client_count + 1;
        client_count++;
        pthread_mutex_unlock(&lock);

        printf("[+] Client %d connected.\n", client_id);

        // Create a new thread for each client
        ClientInfo *client_info = (ClientInfo *)malloc(sizeof(ClientInfo));
        client_info->id = client_id;
        client_info->socket = client_sock;
        pthread_create(&thread_id, NULL, handle_client, (void *)client_info);
        pthread_detach(thread_id);
    }

    close(server_sock);
    return 0;
}

