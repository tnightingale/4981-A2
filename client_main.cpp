#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "ipc.h"
#include "Client.h"

using namespace std;

void print_usage();

int main (int argc, char const *argv[]) {
  key_t key;
  long priority;
  char* validPriority;
  
  if (argc != 3) {
    print_usage();
    return 1;
  }
  
  string filename(argv[1]);    
  priority = strtol(argv[2], &validPriority, 0);
  if (*validPriority != '\0' || priority < 1 || priority > 10) {
    cerr << "client_main.cpp: Error: Invalid priority." << endl;
    print_usage();
    return 2;
  }
  
  if ((key = ftok(MSGQUEUE_TMP, 1)) < 0) {
    perror("client_main.cpp: Error; Could not create key.");
    return 3; // Error.
  }
  Client client(key);
  
  if (!client.Request(filename, priority)) {
    cerr << "client_main.cpp: Error; Request for file failed." << endl;
  }
  
  // Check server's response.
  if (client.WaitForResponse()) {
    client.Receive();
  }
  
  // Error: Server declined service.
  else {
    cerr << "client_main.cpp: Error; Server declined service." << endl;
    return 4;
  }
  
  return 0;
}

void print_usage() {
  cout << "Usage: ./client filepath priority" << endl;
  cout << "\t" << "- filepath: The path to the file requested." << endl;
  cout << "\t" << "- priority: A number representing desired priority (1 - 10),";
  cout << " where 1 is highest priority" << endl;
  cout << "\t" << "\t" << "    ";
  cout << "and 10 is lowest priority." << endl;
}
