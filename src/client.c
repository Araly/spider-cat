#include "main.h"
#define PORT 8080

int client(char *username) {
	int sock = 0;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

  client_chat(sock, username);

	return 0;
}

void *reading_from_server(void *arg) {
  int sock = (int)arg;
  char message[2050];
  for (;;) {
    memset(message, 0, strlen(message));
    read(sock, message, 2050);
    printf("%s", message);
  }
  pthread_exit(NULL);
}

int client_chat(int sock, char *username) {
  char message[2050], tmp_message[2000];
  strcpy(message, username);
  send(sock, message, strlen(message), 0);
  memset(message, 0, strlen(message));
  read(sock, message, 2050);
  if (strcmp(message, username) != 0) {
    printf("registration failed: %s\n", message);
    exit(EXIT_FAILURE);
  }
  pthread_t thread;
  pthread_create(&thread, NULL, reading_from_server, (void *)sock);
  for (;;) {
    memset(message, 0, strlen(message));
    memset(tmp_message, 0, strlen(tmp_message));
    fgets(tmp_message, 280, stdin);
    if (strcmp(tmp_message, "/exit\n") == 0) {
      send(sock, "/exit", strlen("/exit"), 0);
      printf("bye\n");
      break;
    }
    else if (tmp_message[0] != '\n') {
      strcat(message, tmp_message);
      send(sock, message, strlen(message), 0);
    }
  }
  return 0;
}
