#include <iostream>
#include <fstream>
#include <string>

#include "ipc.h"
#include "Client.h"

using namespace std;

int main (int argc, char const *argv[]) {
  key_t key;
  
  if (argc != 2) {
    // Error: Incorrect arguments; print usage.
  }
  
  if ((key = ftok(MSGQUEUE_TMP, 1)) < 0) {
    perror("client_main: ERROR; Could not create key.");
    return 1; // Error.
  }
  
  Client client(key);
  if (!client.Request()) {
    cerr << "client_main: ERROR; Request for file failed." << endl;
  }
  
  client.Listen();
  
  return 0;
}