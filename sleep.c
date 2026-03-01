#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  // char array to print
  char *msg = "Hello world!\n";
  int i = 0;
  while (i < strlen(msg)) {

    printf("%c", msg[i]);
    i++;
    fflush(stdout);
    //    printf("sleeping 3s\n");
    sleep(2);
  }
  printf("\nDONE");
  return 0;
}
