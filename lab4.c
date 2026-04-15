// #define _POSIX_C_SOURCE 200809
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
struct header {
  uint64_t size;       // 8bytes
  struct header *next; // 8bytes
}; // header should be 16 bytes

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE]; // stores string of max size 1024to print out
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  // snprintf formats our message ??

  if (len < 0) {
    perror("snprintf error");
    exit(0);
  }
  write(STDOUT_FILENO, buf, len); // write message out to terminal
}

int hSize =
    sizeof(struct header); // global var that tracks size of our struct header
int main() {

  // CREATE BLOCKS
  void *pbreak = sbrk(256); // sbrk increases heap by 256 bytes, returns start
                            // addr of newly allocate mem
  void *nextblock =
      (char *)pbreak +
      128; // stores 128 bytes from the start of the newly alloc mem
  struct header *head =
      (struct header *)pbreak; // sets header at start of newly allocated mem
  struct header *next =
      (struct header *)nextblock; // sets header 128 bytes from start of heap

  // INITIALIZE BLOCKS
  head->size = 128;
  head->next = NULL;
  next->size = 128;
  next->next = head;
  // head points to most recently free block

  int data_seg = 128 - sizeof(struct header); // data segment of a block
  char *head_seg = (char *)head +
                   sizeof(struct header); // start of data segment of head block
  char *next_seg = (char *)next + sizeof(struct header);

  memset(head_seg, 0, data_seg); // fills  block of memory with data
  memset(next_seg, 1, data_seg);

  // PRINT VALS
  print_out("header start address: %p\n", &head, sizeof(head));
  print_out("next start address: %p\n", &next, sizeof(next));
  // should be 128 bytes apart

  // prints size, next block in header of begging onf heap
  print_out("Header block size: %d\n", &head->size, sizeof(&head->size));
  print_out("Header block next: %p\n", &head->next, sizeof(&head->next));

  // prints size, next blcok in header of next block in heap
  print_out("Next block size: %d\n", &next->size, sizeof(&next->size));
  print_out("Next block next: %p\n", &next->next, sizeof(next->next));

  uint64_t byte;
  uint64_t byte2;
  for (int i = 0; i < data_seg; i++) {
    byte = (uint8_t)head_seg[i];
    print_out("Header block bytes: %lu\n", &byte,
              sizeof(uint8_t)); // prints out header block bytes
    byte2 = (uint8_t)next_seg[i];
    print_out("Next block bytes: %lu\n", &byte2,
              sizeof(uint8_t)); // prints out next block bytes
  }
}
