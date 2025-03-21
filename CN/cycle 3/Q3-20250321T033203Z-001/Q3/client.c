#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
  char *ip = "127.0.0.1";
  int port = 5566;
  int sock;
  struct sockaddr_in addr;
  socklen_t addr_size;
  int numbers[] = {10, 20, 30, 40, 50};
  int n = 5, min = 0, max = 0;
  int avg;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock<0) {
    perror("[.]Socket Error");
    exit(1);
  }

  printf("[+]TCP server socket created.\n");

  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip);

  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("Connection failed");
    exit(1);
  }

  send(sock, &n, sizeof(n), 0);
  send(sock, numbers, sizeof(numbers),0);

  recv(sock, &avg, sizeof(avg), 0);
  printf("Average is: %d\n", avg);
  recv(sock, &min, sizeof(min), 0);
  recv(sock, &max, sizeof(max), 0);
  printf("Min value: %d\nMax value: %d\n", min, max);
  close(sock);
  printf("Disconnected from the server.\n");

  return 0;
}
