#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// integer to read/write to mem, 4 byte
struct mapped_struct {
  uint32_t count;
};

// write to file
void do_writer(const char *shmname) {
  printf("writer: %s\n", shmname);
  int shmfd = shm_open(shmname, O_CREAT, 0700);
  ftruncate(shmfd, sizeof(struct mapped_struct));
  struct mapped_struct *m;
  m = mmap(NULL, sizeof(struct mapped_struct), PROT_READ | PROT_WRITE,
           MAP_SHARED, shmfd, 0);
  if (m == NULL) {
    perror("mmap");
    exit(2);
  }

  void (*f)(void) = (void (*)(void))m; // conv m to function ptr
  f();
  m->count = 50;
  for (int i = 0; i < 10; i++) {
    m->count++;
    sleep(1);
  }
  close(shmfd);
  shm_unlink(shmname);
}

// read from file
void do_reader(const char *shmname) {
  printf("reader: %s\n", shmname);
  int shmfd = shm_open(shmname, O_RDONLY, 0700);
  ftruncate(shmfd, sizeof(struct mapped_struct));
  struct mapped_struct *m;
  m = mmap(NULL, sizeof(struct mapped_struct), PROT_READ, MAP_SHARED, shmfd, 0);
  printf("count: %d\n", m->count);
  close(shmfd);
  shm_unlink(shmname);
}

int main(int argc, char *argv[]) {

  if (argc != 3 || strcmp(argv[1], "-r") != 0 && strcmp(argv[1], "-w") != 0) {
    fprintf(stderr, "%s [-r | -w] <SHM_NAME>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "-w") == 0) {
    do_writer(argv[2]);
  } // if argument passed is -w, do writer() func
  else {
    do_reader(argv[2]);
  } // else is option is -r do reader()
  exit(EXIT_SUCCESS);
}
