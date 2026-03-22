#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

/*
Questions to answer at top of client.c:
(You should not need to change the code in client.c)
1. What is the address of the server it is trying to connect to (IP address and
port number). addre 127.0.0.1, port 8000
2. Is it UDP or TCP? How do you know?
TCP ince we use SOCK_STREAM in socket( ..) , UDP = SOCK_DGRAM
3. The client is going to send some data to the server. Where does it get this
data from? How can you tell in the code? We get it from stdin, because we use
read(..) to stread from stdin

4. How does the client program end? How can you tell that in the code?
We close socket (close(sfd) and exit program exit(EXIT_SUCCESS)

*/

int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}


#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};
// struct to store clients id and socket file descriptor

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  // TODO: print the message received from client

  int cfd = client->cfd;
  int cid = client->client_id;
  free(client);

  ssize_t size; // size of string read
  char buf[BUF_SIZE];

  while ((size = read(cfd, buf, BUF_SIZE)) > 0) {

    buf[size] = '\0'; // null terminate string read
    printf("Client ID %d: %s", cid, buf);

    // TODO: increase total_message_count per message
    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    printf("Total messages count: %d\n", total_message_count);
    pthread_mutex_unlock(&count_mutex);
  }

  return NULL;
}

int main() {
  // declare socket file desc, socket addr
  struct sockaddr_in addr;
  int sfd;

  // create socket
  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  // set the addrest struct
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind attaches socket file desc, addr and port
  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  // listen to incoming client connections
  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  // inf loop to accept clients
  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    struct client_info *c =
        (struct client_info *)malloc(sizeof(struct client_info));

    // wait for connection
    c->cfd = accept(sfd, NULL, NULL);

    // assign client id within a lock
    pthread_mutex_lock(&client_id_mutex);
    c->client_id = client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    pthread_t th;
    if (pthread_create(&th, NULL, handle_client, c) != 0) {
      free(c);
      perror("pthread_create fail");
    }

    pthread_join(th, NULL);
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
