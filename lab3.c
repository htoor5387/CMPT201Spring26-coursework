#define POSIX_C_SOURCE 200810L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxLines 5
int main() {
  char *array[5];   // history of lines
  int numLines = 0; // tracks array size increase

  while (1) {
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

    if (numLines >= maxLines) {
      free(array[0]);
      for (int i = 1; i < numLines; i++) {
        array[i - 1] = array[i];
      }
      numLines--;
    } // removes oldest line from history

    array[numLines] = buffer; // store user input in arr
    numLines++;

    if (strcmp(buffer, "print") == 0) {
      for (int i = 0; i < numLines; i++) {
        printf("%s\n", array[i]);
      }
    } // prints history of lines if print is inserted
  }
  free(array);
}
