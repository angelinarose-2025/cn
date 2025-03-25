#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
  char *ip = "127.0.0.1";
  int port = 5566;

  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  int buffer[1024];
  int n, sum = 0, avg, min = 0, max = 0;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0) {
    perror("[.]Socket Error");
    exit(1);
  }

  printf("[+]TCP server socket created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0) {
    perror("[.]Bind error");
    exit(1);
  }
  printf("[+]Bind to the port number %d\n", port);

  listen(server_sock, 5);
  printf("Listening...\n");

  while(1) {
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+] Client Connected.\n");

    /*
    bzero(buffer, 1024);
    recv(client_sock, buffer, sizeof(buffer),0);
    printf("Client message: %s\n", buffer);

    bzero(buffer, 1024);
    strcpy(buffer, "Hi, I am server, how are you doing?");
    printf("Server: %s\n", buffer);
    send(client_sock,buffer,strlen(buffer),0);

    close(client_sock);
    printf("Client disconnected");
    */

    recv(client_sock, &n, sizeof(n), 0);
    recv(client_sock, buffer, n * sizeof(int), 0);
    min = max = buffer[0];
    for (int i=0; i < n; i++) {
      sum += buffer[i];
      if (buffer[i] < min) {
        min = buffer[i];
      }
      else if (buffer[i] > max) {
        max = buffer[i];        
      }
    }
    avg = sum / n;
    printf("min: %d", min);
    printf("max: %d", max);

    send(client_sock, &avg, sizeof(avg), 0);
    printf("Server sent avg: %d\n", avg);
    send(client_sock, &min, sizeof(min), 0);
    printf("Server sent min: %d\n", min);
    send(client_sock, &max, sizeof(max), 0);
    printf("Server sent max: %d\n", max);

    close(client_sock);
    printf("Client disconnected");

  }

  return 0;

}
