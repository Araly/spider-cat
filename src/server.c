#include "main.h"
#define PORT 8080
#define MAX_USERS 10

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

void *server_chat(void *arg) {
  int user_no = (int)arg;
  char message[2050];
  for (;;) {
    memset(message, 0, strlen(message));
    read(users[user_no].sockfd, message, 2050);
    if (strcmp(message, "") == 0) {
      printf("%s left\n", users[user_no].author);
      break;
    }
    char reply[2100];
    snprintf(reply, sizeof(reply), "%c[1;33m%s:%c[0;00m %s", 27, users[user_no].author, 27, message);
    printf("%s", reply);
    for (int i = 0; i < users_no; i++) {
      send(users[i].sockfd, reply, strlen(reply), 0);
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

