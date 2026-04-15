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

// write to file, takes name to file to write
void do_writer(const char *shmname) {
  printf("writer: %s\n", shmname);
  int shmfd = shm_open(shmname, O_CREAT,
                       0700); // opens shared mem, creates it, sets 0700
                              // permissin return fd of shared mem space
  ftruncate(
      shmfd,
      sizeof(struct mapped_struct)); // set size of mem space to size of message
  struct mapped_struct *m;
  m = mmap(NULL, sizeof(struct mapped_struct), PROT_READ | PROT_WRITE,
           MAP_SHARED, shmfd,
           0); // links shared mem to memory
               // MAP_SHARED - sets mmap to share this memory with other proc
               // PROT_READ | PROT WRITE - enables read and write to mem map
               // space returns a struct pointer m to shared memthat all
               // processes share/ can edit
  if (m == NULL) {
    perror("mmap");
    exit(2);
  }

  void (*f)(void) = (void (*)(void))m; // conv m to function ptr
  f();
  m->count = 50; // edits value of ptr to shared mem
  for (int i = 0; i < 10; i++) {
    m->count++;
    sleep(1);
  }
  close(shmfd);        // close shared mem fd
  shm_unlink(shmname); // closed shared mem
}

// read from file
void do_reader(const char *shmname) {
  printf("reader: %s\n", shmname); // print name of shared mem space
  int shmfd = shm_open(shmname, O_RDONLY, 0700); // opens shared mem in read
                                                 // only
  ftruncate(shmfd, sizeof(struct mapped_struct)); // set size of shared mem
  struct mapped_struct *m;
  m = mmap(NULL, sizeof(struct mapped_struct), PROT_READ, MAP_SHARED, shmfd, 0);
  // map shared mem to memory mapping region
  // size of shared mem <= size of mmap region = size of mapped struct
  // set protection to PROT_READ - read only from mapped mem
  // MAP_SHARED - always for shared memory mapping - allows proc to comm
  // return pointer m to mem mapped region
  printf("count: %d\n", m->count); // read form the pointer in shared memory
  close(shmfd);
  shm_unlink(shmname);
}

int main(int argc, char *argv[]) {

  if (argc != 3 || strcmp(argv[1], "-r") != 0 && strcmp(argv[1], "-w") != 0) {
    fprintf(stderr, "%s [-r | -w] <SHM_NAME>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "-w") == 0) {
    do_writer(argv[2]); // open/write to shared mem
  } // if argument passed is -w, do writer() func
  else {
    do_reader(argv[2]); // read form shared mem
  } // else is option is -r do reader()
  exit(EXIT_SUCCESS);
}
