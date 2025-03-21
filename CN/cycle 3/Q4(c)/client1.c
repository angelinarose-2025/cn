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
  float num;

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
  
  printf("Enter float value: ");
  scanf("%f", &num);
  send(sock, &num, sizeof(num), 0);

  close(sock);
  printf("Disconnected from the server.\n");

  return 0;
}
