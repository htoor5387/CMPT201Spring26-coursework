#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

// rec only 1 commmant line argument - file name
// create file mem mapping for entire file
// print out content of entire memory mapping

int main(int argc, char *argv[]) {
  int fd;
  char *addr;
  off_t offset, pa_offset;
  size_t length;
  ssize_t s;
  struct stat sb;

  if (argc != 2) {
    fprintf(stderr, "%s [FILE]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fd = open(argv[1], O_RDONLY); // open file
  if (fd == -1)
    handle_error("open");

  if (fstat(fd, &sb) == -1) // To obtain file size, store struct with info about file in sb
    handle_error("fstat");

  length = sb.st_size; //store file size in lenght from struct sb.st_size

  addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);//create file mem mapping
 //memory mapped between heap and stack
  if (addr == MAP_FAILED)
    handle_error("mmap");

  s = write(STDOUT_FILENO, addr, length); //prints out content of file
  if (s != length) {

    if (s == -1)
      handle_error("write");

    fprintf(stderr, "partial write");
    exit(EXIT_FAILURE);
  }

  munmap(addr, length);
  close(fd);

  exit(EXIT_SUCCESS);
}
