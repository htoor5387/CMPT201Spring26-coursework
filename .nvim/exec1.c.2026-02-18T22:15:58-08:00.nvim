#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int pid = fork();
  if (pid == -1) {
    perror("fail");
    exit(EXIT_FAILURE);
  } // fork fail

  char *args[] = {"ls", "-a", NULL};
  if (pid > 0) {

    printf("parent exec\n");
    execlp("ls", "ls", "-alh", (char *)NULL);
    
  //doesn't print
    printf("parent id %d\n", getpid());
  } // parent
  else {
    printf("child exec\n");

    execvp(args[0], args);

    //doesn't print
    printf("child id %d\n", getpid());
  } // child

  return 0;
}
