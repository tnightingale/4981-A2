#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "ipc.h"
#include "Client.h"

using namespace std;

int main (int argc, char const *argv[]) {
  key_t key;
  string filename;
  
  if (argc > 3) {
    // Error: Incorrect arguments; print usage.
    return 1;
  }
  
  if ((key = ftok(MSGQUEUE_TMP, 1)) < 0) {
    perror("client_main: ERROR; Could not create key.");
    return 2; // Error.
  }
  Client client(key);
  
  // File path passed.
  switch (argc) {    
    // Only filename.
    case OPT_FILENAME:
      filename.insert(0, argv[1]);
    
      if (!client.Request(filename)) {
        cerr << "client_main: ERROR; Request for file failed." << endl;
      }
      break;
    
    // Filename and priority.
    case OPT_FILENAME_PRIORITY:
      filename.insert(0, argv[1]);
    
      if (!client.Request(filename, atoi(argv[2]))) {
        cerr << "client_main: ERROR; Request for file failed." << endl;
      }
      break;
  }
  
  client.Receive();
  
  return 0;
}
