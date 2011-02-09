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
  
  for (int i = 1; i <= argc; i++) {
    // Check for flags.
    if (argv[i][0] == '-' && argv[i][2] == ' ') {
      switch (argv[i++][1]) {
        case 'o':
          break;
        default:
          // Print usage.
          return 1;
          break;
      }
    }
    
    // Validate input filepath. 
    else if (...) {
    
    } 
    
    // Print usage.
    else {
      return 1;
    }
  }
  
  // Begin.
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
