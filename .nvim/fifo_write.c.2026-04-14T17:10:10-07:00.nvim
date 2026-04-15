// #define _POSIX_C_SOURCE 200809
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// take param as string
// parent sends string to child
// child returns strign to parent in upper case
// parent prints string

int main(int argc, char *argv[]) {

  /*  if (argc != 2) {
      fprintf(stderr, "Usage: %s <string>\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  */
  char *path = "fifo";
  mkfifo(path, 0777);

  int fd = open(path, O_WRONLY);

  char *buf = "String sent thru fifo \n";

  write(fd, buf, strlen(buf));

  close(fd);
  return 0;
}
