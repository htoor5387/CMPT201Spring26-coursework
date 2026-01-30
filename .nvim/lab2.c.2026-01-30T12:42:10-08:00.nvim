#define POSIX_C_SOURCE 200809L // for getline
#include <stdio.h>             //printf, perror, getline
#include <stdlib.h>            //free, exit
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> //fork, exec

int main() {

  // int pid = fork();

  char *buffer = NULL;
  size_t size;
  ssize_t string;
  while (1) {
    printf("Enter programs to run. \n");
    string = getline(&buffer, &size, stdin);
    if (string == -1) {
      perror("getline failed");
      exit(EXIT_FAILURE);
    } else if (string > 0) {
      buffer[string - 1] = '\0';
    }

    pid_t pid = fork();

    if (pid) {

      int wstatus = 0;

      if (waitpid(pid, &wstatus, 0) ==
          -1) { // waitpid waits for child to finish
        perror("waitpid");
        exit(EXIT_FAILURE);
      }

      if (WIFEXITED(wstatus)) {
        printf("Child exited.\n");
      }
      free(buffer); // must free up mem used by buffer
      buffer = NULL;
    } // parent gets input from user, waits for child to execute

    else {
      if (execl(buffer, buffer, (char *)NULL) == -1) {
        perror("execl failed \n");
        exit(EXIT_FAILURE);
      }
    } // child gets user input and executes it using exec
  }
  return 0;
}
