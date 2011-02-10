#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>

#include <sys/wait.h>

#include "ipc.h"
#include "Server.h"

using namespace std;

void catch_sigchld(int);
void catch_sigint(int);

int main(int argc, char const *argv[])
{
  key_t key;
  struct sigaction sa;
  struct sigaction old_sigchld_sa;
  struct sigaction old_sigint_sa;

  // Set signal handlers.
  // Handler for SIGCHLD.
  sa.sa_handler = catch_sigchld;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_NOCLDWAIT;
  if (sigaction(SIGCHLD, &sa, &old_sigchld_sa) == -1) {
    perror("server_main: Error calling sigaction; ");
  }

  // Handler for SIGINT.
  sa.sa_handler = catch_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, &old_sigint_sa) == -1) {
    perror("server_main: Error calling sigaction; ");
  }
  
  key = Connection::GetResKey();
  Server server(key);

  // Restore the default signals on close. 
  sigaction(SIGCHLD, &old_sigchld_sa, NULL);
  sigaction(SIGCHLD, &old_sigint_sa, NULL);
  
  return 0;
}

void catch_sigchld(int) {
  // Clean up the child process.
  wait(NULL); 
}

void catch_sigint(int) {
  key_t key = Connection::GetResKey();
  int qid = Connection::ConnectQueue(key);
  cout << "\nStop signal received; stopping client processes..." << endl;

  while (wait(NULL) != -1) ;
  Connection::DestroyQueue(qid);

  cout << "Cleanup finished, goodbye." << endl;
  exit(0);
}
