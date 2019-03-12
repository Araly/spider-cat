// Server side C/C++ program to demonstrate Socket programming
#include "main.h"

/*
void *example(void *arg) {
  printf("hello world\n");
  pthread_exit(NULL);
}
*/

int main(int argc, char *argv[]) {
  if (argc == 1) {
    server();
  }
  if (argc == 2) {
    client(argv[1]);
  }
  /*
  pthread_t thread;

  pthread_create(&thread, NULL, example, NULL);
  */

  pthread_exit(NULL);
}


