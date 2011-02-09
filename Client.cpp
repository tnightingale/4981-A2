#include "ipc.h"
#include "Client.h"

using namespace std;

Client::Client(key_t &key) : key_(key), connection_(key) {}

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
  
  if (!connection_.Write(msg)) {
    return false;
  }
  
  return true;
}

bool Client::Receive(ofstream& output) {
  MSG msg;
  int pid = getpid();
  size_t received = 0;
  
  
  while (true) {  
    if (msg.priority == -1) break;
    
    if (connection_.Listen(pid, msg) < 0) {
      perror("Client::Listen; Error listening to connection.");
      return false;
    }

    received += msg.data_len;
    
    //cout << "Client::Listen(); Msg received." << endl;
    //cout << "\tType:\n\t\t" << msg.type << endl;
    //cout << "\tSender's PID:\n\t\t" << msg.sender_pid << endl;
    //cout << "\tMessage priority:\n\t\t" << msg.priority << endl;
    //cout << "\tMessage: (Length: " << msg.data_len << ")\n\t\t" << msg.data << endl;
    //cout << "\n\t Total received: " << received << endl << endl << endl;
  }
  
  return true;
}

