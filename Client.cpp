#include "ipc.h"
#include "Client.h"

using namespace std;

Client::Client(key_t &key) : key_(key), connection_(key, false) {
  pid_ = getpid();
}

bool Client::Request(string& filename, int priority) {
  if (!this->Write(filename, priority)) {
    return false;
  }

  return true;
}


bool Client::Write(string& messagetext, int priority) {
  MSG msg;
  
  msg.type = 1;
  msg.sender_pid = getpid();
  msg.priority = priority;
  msg.data_len = messagetext.size();
  strncpy(msg.data, messagetext.c_str(), msg.data_len);
  
  if (connection_.Write(msg) < 0) {
    return false;
  }
  
  return true;
}

bool Client::WaitForResponse() {
  MSG msg;
  char* valid;
  
  if (connection_.Listen(pid_, msg) < 0) {
    perror("Client::Listen; Error listening to connection.");
    return false;
  }
  
  // Check for server confirmation
  if (msg.priority == -1) {
    // Server cannot find file.
    cerr << "Client::Receive; Server cannot find/open file." << endl;
    return false;
  }
  
  // Server can read file and returned filesize.
  filesize_ = strtol(msg.data, &valid, 0);
  if (*valid != '\0') {
    // Error.
    return false;
  }

  serverProcPid_ = msg.sender_pid;
  
  return true;
}

bool Client::Receive() {
  MSG msg;
  size_t received = 0;
  
  while (true) {  
    if (msg.priority == -1) { 
      // End of file.
      break;
    }
    
    if (connection_.Listen(pid_, msg) < 0) {
      perror("Client::Listen; Error listening to connection.");
      return false;
    }

    cout << received << ":\t" << msg.data << endl;

    received += msg.data_len;
  }
  
  return true;
}

