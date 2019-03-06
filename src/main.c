#ifdef _WIN32
#include<io.h>
#include <ws2tcpip.h>
#include <windows.h>
#include<winsock2.h>
#include<stdio.h>
//LINKER with library  -lws2_32
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

#ifdef linux
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#define PORT 8888
#define BUFFER_SIZE 1000;

int server();
int client(char pseudo[]);

int main(int argc, char *argv[]) {
  switch(argc) {
  case 1:
    server();
    break;
  case 2:
    client(argv[1]);
    break;
  }
  return 0;
}

int server() {
  int sock, ret_sock;
  struct sockaddr_in server, client;
  int sockaddr_in_size;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf("socket creation failed: %m\n");
    exit(0);
  }
  printf("socket created\n");

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("bind failed: %m\n");
    exit(0);
  }
  puts("bind done\n");

  listen(sock, 3);

  puts("waiting for incomming connections...");

  sockaddr_in_size = sizeof(struct sockaddr_in);
  ret_sock = accept(sock, (struct sockaddr *)&client, &sockaddr_in_size);
  if (ret_sock == 0) {
    printf("accept failed: %m\n");
    exit(0);
  }
  puts("accept successful");
  close(sock);

  return 0;
}

int client(char pseudo[]) {
  printf("welcome %s\n", pseudo);

  int sock, ret;
  struct sockaddr_in server, client;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("socket creation failed: %m\n");
    exit(0);
  }

  printf("socket created\n");

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(PORT);

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
    printf("connection to the server failed: %m\n");
    exit(0);
  }

  printf("connected to the server\n");

  close(sock);

  return 0;
}
