#include <stdio.h>
#include <stdlib.h>

int main() {
  char *buffer = NULL;
  size_t size;
  size_t string;

  while (buffer != "Stop") {
    printf("Please enter some text: ");
    string = getline(&buffer, size, stdin);

    char *saveptr;
    char *token = strtok_r(buffer, " ", &saveptr);

    while (token != NULL) {
      printf("%s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }
  }
}
