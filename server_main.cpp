#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>

#include <sys/wait.h>

#include "ipc.h"
#include "Server.h"

using namespace std;

void NoZombie(int);

int main(int argc, char const *argv[])
{
  key_t key;
  struct sigaction sa;
  struct sigaction oldsa;

  // Set up handler for SIGCHLD.
  sa.sa_handler = NoZombie;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDWAIT;
  if (sigaction(SIGCHLD, &sa, &oldsa) == -1) {
    perror("server_main: ERROR; ");
  }
  
  if ((key = ftok(MSGQUEUE_TMP, 1)) < 0) {
    perror("server_main: ERROR; Could not create key.");
    return 1; // Error.
  }
  
  Server server(key);

  // Restore the SIGCHLD
  sigaction(SIGCHLD, &oldsa, NULL);
  
  return 0;
}

void NoZombie(int) {
  // Clean up the child process.
  wait(NULL); 
}
