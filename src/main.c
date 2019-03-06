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
#define BUFFER_SIZE 1000

int server();
void server_chat(int sock);
int client(char pseudo[]);
void client_chat(int sock);

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

// SERVER

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

  server_chat(sock);

  close(sock);

  return 0;
}

void server_chat(int sock) {
  char buffer[BUFFER_SIZE];
  int n;

  for (;;) {
    bzero(buffer, BUFFER_SIZE);

    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    bzero(buffer, BUFFER_SIZE);
    n = 0;

    while ((buffer[n++] = getchar()) != '\n') {
      ;
    }
    write(sock, buffer, sizeof(buffer));

    if (strncmp("exit", buffer, 4) == 0) {
      printf("exit\n");
      break;
    }
  }
}

// CLIENT

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

  client_chat(sock);

  close(sock);

  return 0;
}

void client_chat(int sock) {
  char buffer[BUFFER_SIZE];
  int n;
  for (;;) {
    bzero(buffer, sizeof(buffer));
    printf("> ");
    n = 0;
    while ((buffer[n++] = getchar()) != '\n') {
      ;
    }
    printf("%s\n", buffer);
    write(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    bzero(buffer, sizeof(buffer));
    printf("%s\n", buffer);
    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    if ((strncmp(buffer, "exit", 4)) == 0) {
      printf("exit\n");
      break;
    }
  }
}
