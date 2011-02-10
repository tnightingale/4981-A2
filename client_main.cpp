#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "ipc.h"
#include "Client.h"

using namespace std;

/**
 * Helper function to print usage instructions to users.
 *
 * @author Tom Nightingale.
 */
void print_usage();

/**
 * Client's entry point.
 * Parses user's input, requested file and priority and notifies on validation 
 * error.
 *
 * Then creates a Client object which sends request for file.
 *
 * @author Tom Nightingale
 */
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
  
  key = Connection::GetResKey();
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
