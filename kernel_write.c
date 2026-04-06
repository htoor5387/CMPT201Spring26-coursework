// #define _POSIX_C_SOURCE 200809
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  printf("creating file \n");

  int fd =
      open("tmp1", O_CREAT | O_TRUNC | O_RDWR, 0777); // create file called tmp
  // open() opens takes a file called tmp, creates it and sets permissions to it
  // returns a file descriptor to edit file

  char *string = "Hello world";
  int len = strlen(string);
  printf("write %s to file \n", string);
  write(fd, string, len); // write string to file, takes file descriptor to
                          // edit, string and length of string
  while (1) {
    sleep(10);
  }
  return 0;
}
