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

int main(int argc, char *argv[]) {
  printf("hello world\n");
  return 0;
}
