// #define _POSIX_C_SOURCE 200809
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// take param as string
// parent sends string to child
// child returns strign to parent in upper case
// parent prints string

int main(int argc, char *argv[]) {

  int ppipefd[2]; // pipe for parent send to child
  int cpipefd[2]; // pipe for child send to parent
  char buf;
  pid_t cpid;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <string>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (pipe(ppipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (pipe(cpipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  cpid = fork();
  if (cpid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0) {     // in child proc, write from parent to child
    close(ppipefd[1]); /* Close unused read end for parent*/
    close(cpipefd[0]); // close unuse write end for child
    while (read(ppipefd[0], &buf, 1) > 0) { // parent write to child
      buf = toupper(buf);
      write(cpipefd[1], &buf, 1); // write stringto child
    }

    write(cpipefd[1], "\n", 1);
    close(ppipefd[0]); // parent finish writing to child, close write end once
                       // done
    close(cpipefd[1]); // close read end for child once done reading string to
                       // hcild
    _exit(EXIT_SUCCESS); // exit child

  } else {             // in parent proc
    close(ppipefd[0]); // Close write end for parent
    close(cpipefd[1]); // close read end of pipe for child

    write(ppipefd[1], argv[1], strlen(argv[1])); // write from child to parent
    close(ppipefd[1]);                           /* Reader will see EOF */
    while (read(cpipefd[0], &buf, 1) > 0) {
      write(STDOUT_FILENO, &buf, 1);
    }
    wait(NULL); /* Wait for child */
    exit(EXIT_SUCCESS);
  }
}
