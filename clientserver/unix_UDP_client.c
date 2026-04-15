#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int cfd;

  char *pathname = "tmp";               // a socket file for communication
  cfd = socket(AF_UNIX, SOCK_DGRAM, 0); // local unix connection using UDP

  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path,
         pathname); // sets path of socket file, max size of filename is 108

  unlink(pathname);
  bind(cfd, (struct sockaddr *)&addr, sizeof(addr));

  struct sockaddr_un server; // sets server addr to send to
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path,
         pathname); // sets path of socket file, max size of filename is 108

  //  socklen_t s_len = sizeof(server);
  char buf[100] = "Sending message to server";
  int len = strlen(buf);
  buf[len] = '\0';
  printf("%s\n", buf);
  sendto(cfd, "Hello from client", 18, 0, (struct sockaddr *)&server,
         sizeof(server));

  close(cfd); // close client connection
  unlink(pathname);
  return 0;
}
