#include <unistd.h>
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
  char username[16];
};
struct user users[MAX_USERS], tmp_user;
int users_no = 0;

int server() {
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
  pthread_t threads[MAX_USERS];
  while (users_no < MAX_USERS) {
    users[users_no].sockfd = (int)0;
    users[users_no].client_addrlen = sizeof(users[users_no].client_addr);
    int s;
    if ((s = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    sleep(1);
    users[users_no].sockfd = s;
    memset(message, 0, strlen(message));

    if ((read(users[users_no].sockfd, message, 255)) < 0) {
      perror("registration");
      exit(EXIT_FAILURE);
    }
    printf("registration success\n");
    sleep(1);
    strcpy(users[users_no].username, message);
    sleep(1);
    printf("hum\n");
    printf("hum\n");
    printf("hum\n");
    printf("hum\n");
    printf("user %s registrated\n", users[users_no].username);
    printf("user\n");
    printf("user\n");
    printf("user\n");
    printf("success\n");
    for (int i = 0; i < 10; i++) {
      printf("%d\n", i);
    }
    if ((send(users[users_no].sockfd, message, strlen(message), 0)) < 0) {
      perror("send");
      exit(EXIT_FAILURE);
    }

    pthread_create(&threads[users_no], NULL, server_chat, &users_no);
    users_no++;
    sleep(1);
    printf("done");
  }

	return 0;
}

void *server_chat(void *vargp) {
  char message[2048];
  for (;;) {
    memset(message, 0, strlen(message));
    read(users[(int)vargp].sockfd, message, 2048);
    printf("%s", message);
  }
  return NULL;
}
