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

int client_chat(int sock, char *username) {
  char message[2050], tmp_message[2000];
  send(sock, username, strlen(username), 0);
  read(sock, message, 2050);
  if (strcmp(message, "success") != 0) {
    perror("registration failed");
    exit(EXIT_FAILURE);
  }
  for (;;) {
    printf("> ");
    memset(message, 0, strlen(message));
    memset(tmp_message, 0, strlen(tmp_message));
    fgets(tmp_message, 2000, stdin);
    if (tmp_message[0] == '/') {
      if (strcmp(tmp_message, "/exit\n") == 0) {
        printf("bye\n");
        return 0;
      }
    }
    else if (tmp_message[0] != '\n') {
      strcpy(message, username);
      strcat(message, ": ");
      strcat(message, tmp_message);
      send(sock, message, strlen(message), 0);
    }
  }
  return 0;
}
