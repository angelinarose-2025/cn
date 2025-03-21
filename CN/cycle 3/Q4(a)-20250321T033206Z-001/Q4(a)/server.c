#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void reverseString(char str[]) {
    int length = strlen(str);
    int i, j;
    char temp;

    for (i = 0, j = length - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int main() {
  char *ip = "127.0.0.1";
  int port = 5566;
  int n;
  int server_sock, client1_sock, client2_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  char rev_str[1024];

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
    client1_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+] Client 1 Connected.\n");
    
    client2_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+] Client 2 Connected.\n");
    
    
    recv(client1_sock, buffer, sizeof(buffer), 0);
    close(client1_sock);
    printf("Client 1 disconnected");
 
    reverseString(buffer);
    send(client2_sock, buffer, sizeof(buffer), 0);
    close(client2_sock);
    
    
	   
  }

  return 0;

}
