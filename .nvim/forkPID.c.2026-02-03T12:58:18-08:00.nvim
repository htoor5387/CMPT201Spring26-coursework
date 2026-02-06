#include <stdio.h>
#include <unistd.h>

int main() {

  printf("Start PID=%d, parent pid=%d\n", getpid(), getppid());
  int pid = fork();

  if (pid != 0) {
    printf("Parent PID=%d, parent pid=%d\n", getpid(), getppid());
  } else {
    printf("Child PID=%d, child pid=%d\n", getpid(), getppid());
  }

  printf("DONE");
}
