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
      open("tmp", O_CREAT | O_TRUNC | O_RDWR, 0777); // create file called tmp

  char *string = "Hello world";
  int len = strlen(string);
  printf("write %s to file \n", string);
  write(fd, string, len); // write string to file

  int offs = len / 2;

  lseek(fd, offs, SEEK_SET); // adj offset
  printf("adjust file offset %d\n", offs);

  char buf[100];
  size_t count;
  count = read(fd, buf, sizeof(buf)); // read from fiel at offset to buf

  buf[count] = '\0';
  printf("%s\n", buf);

  close(fd);
}
