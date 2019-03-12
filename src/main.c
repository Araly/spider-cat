// Server side C/C++ program to demonstrate Socket programming
#include "main.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    server();
  }
  if (argc == 2) {
    client(argv[1]);
  }
}


