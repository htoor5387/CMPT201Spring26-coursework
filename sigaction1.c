#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle_sig(int sig) {
  const char *signal = "CTRL-C pressed";
  write(2, signal, strlen(signal));
  return;
}

int main() {

  struct sigaction sig;
  sig.sa_handler = handle_sig;
  sig.sa_flags = 0;
  sigemptyset(&sig.sa_mask);
  int pid = fork();

  if (pid == -1) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    //  sigaction(sig, &handle_sig, SIGINT);
    sigaction(SIGINT, &sig, NULL);

    sleep(3);
  }

  else if (pid == 0) {
    kill(getppid(), SIGINT);
  }
}
