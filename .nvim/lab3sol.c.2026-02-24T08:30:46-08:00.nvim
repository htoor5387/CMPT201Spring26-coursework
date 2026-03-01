#define POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 5

char *arr[max];
int numLines = 0;

void addH(char *inp);
void removeH();
void printH();
char *get_input();

int main() {
  while (1) {
    char *input = get_input();
    addH(input);
    if (strcmp(input, "print") == 0) {
      printH();
    }
  }
  free(arr);
  return 0;
}

char *get_input() {
  char *buffer = NULL;
  size_t size;
  size_t string; // store length of line retrieved from user

  printf("Enter input: ");
  string = getline(&buffer, &size, stdin); // stores user inp to buffer
  if (string == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }

  buffer[string - 1] = '\0'; // adds terminator to user input
  return buffer;
}

void addH(char *input) {
  if (numLines >= max) {
    removeH();
  } // removes oldest line from history

  arr[numLines] = input; // store user input in arr
  numLines++;
}

void removeH() {
  free(arr[0]);
  for (int i = 1; i < numLines; i++) {
    arr[i - 1] = arr[i];
  }
  numLines--;
} // removes oldest line from history

void printH() {
  for (int i = 0; i < numLines; i++) {
    printf("%s\n", arr[i]);
  }
}
