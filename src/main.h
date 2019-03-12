#ifndef HEADER_FILE
#define HEADER_FILE

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int server();
int client(char *username);
int server_chat(int sock);
int client_chat(int sock, char *username);

#endif
