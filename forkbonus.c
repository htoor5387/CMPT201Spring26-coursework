#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {

  // pid_t pid=fork() to get PID

  int pid = fork();
  //  fork();
  //  fork();
  //  fork();
  //  fork();

  if (pid > 0) {
    for (int i = 0; i < 10; i++) {
      sleep(1);
      printf("still waiting ...\n");
    }
  } // parent

  for (int i = 0; i < 10; i++) {
    if (pid == 0) {
      printf("child id: %d\n", getppid());
      pid = fork();
    } // creates 10 forks, 10 childs, 10 parents
  }
}
