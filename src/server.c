#include "main.h"
#define PORT 8080
#define MAX_USERS 50

int server_fd, new_socket;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
struct user {
  int client_addrlen;
  struct sockaddr_in client_addr;
  int sockfd;
  char author[16];
};
struct user users[MAX_USERS], tmp_user;
int users_no = 0;

_Bool starts_with(const char *restrict string, const char *restrict prefix)
{
  while(*prefix)
    {
      if(*prefix++ != *string++)
        return 0;
    }

  return 1;
}

void *server_chat(void *arg) {
  int user_no = (int)arg;
  char message[2050];
  for (;;) {
    memset(message, 0, strlen(message));
    read(users[user_no].sockfd, message, 2050);
    if (strcmp(message, "") == 0) {
      char reply[2100];
      snprintf(reply, strlen(reply), "%s crashed\n", users[user_no].author);
      printf("%s", reply);
      for (int i = 0; i < users_no; i++) {
        send(users[i].sockfd, reply, strlen(reply), 0);
      }
      break;
    }
    else if (starts_with(message, "/exit")) {
      char reply[2100];
      snprintf(reply, strlen(reply), "%s left\n", users[user_no].author);
      printf("%s", reply);
      for (int i = 0; i < users_no; i++) {
        send(users[i].sockfd, reply, strlen(reply), 0);
      }
      break;
    }
    else if (starts_with(message, "/w")) {
      char receiver[2100];
      char cut_message[2100];
      char reply[2200];
      strcpy(receiver, message + 3);
      for (long unsigned int i = 0; i < strlen(receiver); i++) {
        if (receiver[i] == ' ') {
          memset(receiver, 0, strlen(receiver));
          strncpy(receiver, message + 3, i);
          receiver[i] = '\0';
          strcpy(cut_message, message + i + 4);
          break;
        }
      }
      for (int i = 0; i < users_no; i++) {
        if ((strcmp(users[i].author, receiver)) == 0) {
          memset(reply, 0, strlen(reply));
          snprintf(reply, sizeof(reply), "from %c[1;34m%s%c[0;00m to %c[1;34m%s%c[0;00m: %s", 27, users[user_no].author, 27, 27, users[i].author, 27, cut_message);
          printf("%s", reply);
          send(users[user_no].sockfd, reply, strlen(reply), 0);
          send(users[i].sockfd, reply, strlen(reply), 0);
        }
      }
    }
    else if (starts_with(message, "/lu")) {
      char reply[2100];
      memset(reply, 0, strlen(reply));
      strcpy(reply, users[0].author);
      for (int i = 1; i < users_no; i++) {
        strcat(reply, " ");
        strcat(reply, users[i].author);
      }
      strcat(reply, "\n");
      printf("%s\n", reply);
      send(users[user_no].sockfd, reply, strlen(reply), 0);
    }
    else if (starts_with(message, "/me")) {
      char reply[2100];
      strcpy(message, message + 4);
      memset(reply, 0, strlen(reply));
      snprintf(reply, sizeof(reply), "%c[1;33m%s %s%c[0;00m", 27, users[user_no].author, message, 27);
      printf(reply);
      send(users[user_no].sockfd, reply, strlen(reply), 0);
    }
    else {
      char reply[2100];
      snprintf(reply, sizeof(reply), "%c[1;33m%s:%c[0;00m %s", 27, users[user_no].author, 27, message);
      printf("%s", reply);
      for (int i = 0; i < users_no; i++) {
        send(users[i].sockfd, reply, strlen(reply), 0);
      }
    }
  }
  pthread_exit(NULL);
}

int server() {
  pthread_t threads[MAX_USERS];

  pthread_create(&threads[0], NULL, server_chat, NULL);
  // Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
  }
  char message[1024];
  while (users_no < MAX_USERS) {
    users[users_no].sockfd = (int)0;
    users[users_no].client_addrlen = sizeof(users[users_no].client_addr);
    int s;
    if ((s = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    users[users_no].sockfd = s;
    memset(message, 0, strlen(message));

    if ((read(users[users_no].sockfd, message, 255)) < 0) {
      perror("registration");
      exit(EXIT_FAILURE);
    }
    strcpy(users[users_no].author, message);
    if ((send(users[users_no].sockfd, message, strlen(message), 0)) < 0) {
      perror("response");
      exit(EXIT_FAILURE);
    }
    if (pthread_create(&threads[users_no], NULL, server_chat, (void *)users_no)) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
    char reply [2100];
    snprintf(reply, sizeof(reply), "%s joined the server\n", users[users_no].author);
    printf("%s", reply);
    users_no++;
    for (int i = 0; i < users_no; i++) {
      send(users[i].sockfd, reply, strlen(reply), 0);
    }
  }
  pthread_exit(NULL);
}

