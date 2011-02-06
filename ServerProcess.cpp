#include "ServerProcess.h"

using namespace std;

ServerProcess::ServerProcess(Connection& connection, int client_pid, size_t num_queue_shares)
: connection_(connection), client_pid_(client_pid), qShare_(num_queue_shares) {}

// Open file for reading.
// Pass contents to Write() for sending.
bool ServerProcess::Respond(char* filepath) {
  char msg_str[MAXMESGDATA];
  ifstream file(filepath, ios_base::in | ios_base::binary);
  int msg_flag;
  int remaining = qShare_;
  
  if (!file) {
    // Failed to open file.
    return false;
  }
  
  while (!file.eof()) {
    remaining = qShare_;
    while (remaining-- > 0 && !file.eof()) {
      file.read(msg_str, MAXMESGDATA);
      
      msg_flag = (file.eof()) ? -1 : 0;
      if (!this->Write(msg_str, file.gcount(), msg_flag)) {
        return false;
      }
    }
    sleep(0);
  }
  return true;
}

// Prepare message object.
// Write X message segments to queue then yeild (where X is qShare).
// Repeat until entire message has been sent.
bool ServerProcess::Write(char* msg_text, size_t msg_text_len, int msg_flag) {
  MSG msg;
  
  msg.type = client_pid_;
  msg.sender_pid = 0;
  msg.priority = msg_flag;
  msg.data_len = msg_text_len;
  strncpy(msg.data, msg_text, msg_text_len);
  
  if (!connection_.Write(msg)) {
    return false;
  }
  
  return true;
}